/*
 * signature_verification.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: aditya
 */
#include "gt_enclave_t.h"
//#include "sgx_tcrypto.h"
#include "secp256k1.h"
#include "secp256k1_ecdh.h"
#include "generic.h"
#include "keccakNISTInterface.h"
#include "keccakF-1600-interface.h"
#include "string"
#include "conversion.h"
#include "crypto.h"
#include "cjson.h"

/*
 * pubKey:64 bytes binary public key
 * ethAddr: 20 bytes Ethereum address output, caller provide the buffer
 */
sgx_status_t pubKey2EthAddr(const uint8_t *pubKey, uint8_t *ethAddr){
	BitSequence hash[32];
	HashReturn hret = KECCAK256_hash(256, pubKey, 64<<3, hash);
	if (SUCCESS != hret)
		return (sgx_status_t)-1;
	memcpy((void *)ethAddr, (const void *)(hash+12), 20);
	return SGX_SUCCESS;
}

sgx_status_t verifyEthereumSign(secp256k1_ecdsa_signature *pSig, const uint8_t *ethPubkey, char *message, size_t msgLen){
	BitSequence hash[32];
	uint8_t msgBuff[256];
	snprintf((char *)msgBuff, 50, "%cEthereum Signed Message:\n%d", '\x19', (int)msgLen);
	size_t len = strnlen((const char *)msgBuff, 255);
	if ((len+msgLen)>255)
		return (sgx_status_t)-1;
	memcpy((void *)(msgBuff+len), message, msgLen);
	KECCAK256_hash(256, msgBuff, (len+msgLen)<<3, hash);

	sgx_status_t retv = ecall_verify((const uint8_t *)pSig, 64, ethPubkey, 65, hash, 32);
	if (SGX_SUCCESS != retv){
		return (sgx_status_t)-1;
	}
	return SGX_SUCCESS;
}

sgx_status_t genK1KeyPair(sgx_ec256_private_t *enclave_private, uint8_t *genPubkey, size_t *pKeyLen){
	sgx_ecc_state_handle_t ecc_handle;
	sgx_ec256_public_t enclave_public;
	sgx_status_t ret = sgx_ecc256_open_context(&ecc_handle);
	if (ret != SGX_SUCCESS)
		return ret;
	ret = sgx_ecc256_create_key_pair(enclave_private, &enclave_public, ecc_handle);
	if (ret != SGX_SUCCESS) {
		sgx_ecc256_close_context(ecc_handle);
		return ret;
	}
	ret = sgx_ecc256_close_context(ecc_handle);
	if (ret != SGX_SUCCESS)
		return ret;

	secp256k1_context* ctx;
	ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
	if (NULL == ctx)
		return (sgx_status_t)-1;

	int valid = secp256k1_ec_seckey_verify(ctx, enclave_private->r);
	secp256k1_context_destroy(ctx);
	if (!valid){
		return (sgx_status_t)-1;
	}

	ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
	if (NULL == ctx)
		return (sgx_status_t)-1;

	secp256k1_pubkey pubkey;
	valid = secp256k1_ec_pubkey_create(ctx, &pubkey, enclave_private->r);
	secp256k1_context_destroy(ctx);
	if (!valid){
		return (sgx_status_t)-1;
	}

	//genPubkey[0] = SECP256K1_TAG_PUBKEY_UNCOMPRESSED;

	ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
	if (NULL == ctx)
		return (sgx_status_t)-1;

	valid = secp256k1_ec_pubkey_serialize(ctx, genPubkey, pKeyLen, &pubkey, SECP256K1_EC_UNCOMPRESSED);
	secp256k1_context_destroy(ctx);
	if (!valid){
		return (sgx_status_t)-1;
	}
	return SGX_SUCCESS;
}

sgx_status_t gen_ecdh_key(uint8_t *serializedPubkey, sgx_ec256_private_t &privkey, unsigned char *key_hash){
	secp256k1_pubkey tmpPubkey;
	secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
	int valid = secp256k1_ec_pubkey_parse(ctx, &tmpPubkey, serializedPubkey, 65);
	secp256k1_context_destroy(ctx);
	if (!valid)
		return (sgx_status_t)-1;

	//secp256k1_context* ctx;
	ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);

	unsigned char ecdh_key[32];//, key_hash[32];
	valid = secp256k1_ecdh(ctx, ecdh_key,  &tmpPubkey, privkey.r);
	secp256k1_context_destroy(ctx);
	if (!valid){
		return (sgx_status_t)-1;
	}
	uint8_t keyhex[64],hashhex[64];
	bin2hex(ecdh_key, 32, (char *)keyhex);
	uint8_t hash_buf[HASH_LENGTH_KECCAK_256];
	KECCAK256_hash(256, keyhex, 64<<3, hash_buf);
	memcpy(key_hash, hash_buf, 16);
	//	bin2hex(key_hash, 32, (char *)hashhex);
	//memcpy(node->secretKey, key_hash, 16);
	return SGX_SUCCESS;
}

uint8_t * decryptData(const uint8_t *buff, size_t inLen, unsigned char *secretKey, uint8_t *iv){
	uint8_t * dec_data = (uint8_t *)malloc(inLen);
	if (NULL == dec_data){
		return NULL;
	}
	if (SGX_SUCCESS != sgx_aes_ctr_decrypt((const sgx_aes_ctr_128bit_key_t*)secretKey, buff,
			inLen, iv,
			32, //node->ctr_num_bit_size,
			dec_data)){
		free(dec_data);
		return NULL;
	}
	return dec_data;
}

uint8_t * encryptData(const uint8_t *buff, size_t inLen, const unsigned char *secretKey, uint8_t *iv, bool new_iv){
	int encryptedLen = (inLen/16+1)*16;
	uint8_t * enc_data= (uint8_t *)malloc(encryptedLen);
	if (NULL == enc_data){
		return NULL;
	}
	uint8_t iv_buf[16];
	if (new_iv){
		if (SGX_SUCCESS != sgx_read_rand(iv_buf, 16)){
			free(enc_data);
			return NULL;
		}
		memcpy(iv, iv_buf, CTR_LEN);
	} else
		memcpy (iv_buf, iv, CTR_LEN);
	if (SGX_SUCCESS != sgx_aes_ctr_encrypt((const sgx_aes_ctr_128bit_key_t*)secretKey, buff,
			inLen, iv_buf,
			32, //node->ctr_num_bit_size,
			enc_data)){
		free(enc_data);
		return NULL;
	}
	//*outLen = encryptedLen;
	return enc_data;
}

sgx_status_t ecall_verify(const uint8_t* input_sig, size_t in_sig_len, const uint8_t* input_pk, size_t in_pk_len, uint8_t* msg, size_t msg_len)
{
	sgx_status_t ret=(sgx_status_t)(-1);

	secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
	secp256k1_pubkey pk;
	secp256k1_ecdsa_signature sig;

	do{
		if(1!=secp256k1_ec_pubkey_parse(ctx, &pk, input_pk, 65))
			break;

		if(1!=secp256k1_ecdsa_signature_parse_compact(ctx,&sig,input_sig))
			break;

		int tmp = secp256k1_ecdsa_verify(ctx, &sig, msg, &pk);
		if(tmp==1)
			ret=SGX_SUCCESS;

	}while(0);

	if(SGX_SUCCESS == ret){
		   return SGX_SUCCESS;
	}
	return sgx_status_t(-1);
}


