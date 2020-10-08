/*
 * crypto.h
 *
 *  Created on: Sep 26, 2019
 *      Author: accuser
 */

#ifndef GT_ENCLAVE_INCLUDE_CRYPTO_H_
#define GT_ENCLAVE_INCLUDE_CRYPTO_H_
#include "../gt_enclave_t.h"
#include "secp256k1.h"

#define CTR_LEN 16

sgx_status_t pubKey2EthAddr(const uint8_t *pubKey, uint8_t *ethAddr);
sgx_status_t verifyEthereumSign(secp256k1_ecdsa_signature *pSig, const uint8_t *ethPubkey, char *message, size_t msgLen);
sgx_status_t genK1KeyPair(sgx_ec256_private_t *enclave_private, uint8_t *genPubkey, size_t *pKeyLen);
sgx_status_t gen_ecdh_key(uint8_t *serializedPubkey, sgx_ec256_private_t &privkey, unsigned char *key_hash);
sgx_status_t ecall_verify(const uint8_t* input_sig, size_t in_sig_len, const uint8_t* input_pk, size_t in_pk_len, uint8_t* msg, size_t msg_len);
uint8_t * decryptData(const uint8_t *buff, size_t inLen, unsigned char *secretKey, uint8_t *iv);
uint8_t * encryptData(const uint8_t *buff, size_t inLen, const unsigned char *secretKey, uint8_t *iv, bool new_iv);

#endif /* GT_ENCLAVE_INCLUDE_CRYPTO_H_ */
