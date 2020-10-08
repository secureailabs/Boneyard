/*
 * Generic Hash and HMAC Program
 *
 * Copyright (C) 2009 2011 Harald von Fellenberg <hvf@hvf.ch>
 *
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 3 of the License, or 
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

/*
 * The files keccakF-1600-opt32.c  keccakNISTInterface.c  keccakSponge.c
 * keccakF-1600-32.macros      keccakF-1600-opt32-settings.h
 * keccakF-1600-32-rvk.macros  keccakF-1600-unrolling.macros
 * keccakF-1600-interface.h    keccakNISTInterface.h
 * keccakF-1600-int-set.h      keccakSponge.h
 * have been taken from the Keccak submission to the SHA3 contest, see
 * http://csrc.nist.gov/groups/ST/hash/sha-3/Round3/submissions_rnd3.html
 * The Keccak homepage is http://keccak.noekeon.org/
 * The authors of Keccak are
 * Guido Bertoni, Joan Daemen, Michaël Peeters, Gilles Van Assche
 *
 * integration into fehashmac by hvf 10.04.2011
 */

/*
The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
Michaël Peeters and Gilles Van Assche. For more information, feedback or
questions, please refer to our website: http://keccak.noekeon.org/

Implementation by the designers,
hereby denoted as "the implementer".

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "generic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "keccakNISTInterface.h"
#include "keccakF-1600-interface.h"

HashReturn KECCAK_Init(spongeState *state, int hashbitlen)
{
    switch(hashbitlen) {
        case 0: // Default parameters, arbitrary length output
            KECCAK_InitSponge((spongeState*)state, 1024, 576);
            break;
        case 224:
            KECCAK_InitSponge((spongeState*)state, 1152, 448);
            break;
        case 256:
            KECCAK_InitSponge((spongeState*)state, 1088, 512);
            break;
        case 384:
            KECCAK_InitSponge((spongeState*)state, 832, 768);
            break;
        case 512:
            KECCAK_InitSponge((spongeState*)state, 576, 1024);
            break;
        default:
            return BAD_HASHBITLEN;
    }
    state->fixedOutputLength = hashbitlen;
    state->hashbitlen = hashbitlen;
    return SUCCESS;
}

HashReturn KECCAK_Update(spongeState *state, const BitSequence *data, DataLength databitlen)
{
    if ((databitlen % 8) == 0)
        return KECCAK_Absorb((spongeState*)state, data, databitlen);
    else {
        HashReturn ret = KECCAK_Absorb((spongeState*)state, data, databitlen - (databitlen % 8));
        if (ret == SUCCESS) {
            unsigned char lastByte; 
            // Align the last partial byte to the least significant bits
            lastByte = data[databitlen/8] >> (8 - (databitlen % 8));
            return KECCAK_Absorb((spongeState*)state, &lastByte, databitlen % 8);
        }
        else
            return ret;
    }
}

HashReturn KECCAK_Final(spongeState *state, BitSequence *hashval)
{
    return KECCAK_Squeeze(state, hashval, state->fixedOutputLength);
}

HashReturn KECCAK_Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{
    spongeState state;
    HashReturn result;

    if ((hashbitlen != 224) && (hashbitlen != 256) && (hashbitlen != 384) && (hashbitlen != 512))
        return BAD_HASHBITLEN; // Only the four fixed output lengths available through this API
    result = KECCAK_Init(&state, hashbitlen);
    if (result != SUCCESS)
        return result;
    result = KECCAK_Update(&state, data, databitlen);
    if (result != SUCCESS)
        return result;
    result = KECCAK_Final(&state, hashval);
    return result;
}

#ifdef GT_UNDEFINED
/* 
 * parameter safe wrappers for KECCAK routines for each hash length
 */

 /*************************** 224 ************************************/

HashReturn KECCAK224_init (hashState  *state, int hashbitlen)
{
    /* verify correct hash length   */
    if (hashbitlen != HASH_BITLENGTH_KECCAK_224)
        return BAD_HASHBITLEN;

    /* allocate context and fill it */
    KECCAK_CTX *context = (KECCAK_CTX *)malloc (sizeof (KECCAK_CTX));
    memset (context, 0, sizeof (KECCAK_CTX));
    context->hashbitlen = HASH_BITLENGTH_KECCAK_224;
    context->magic = HASH_MAGIC_KECCAK_224;
	*state = (hashState *) context;
	return KECCAK_Init (context, hashbitlen);
}

HashReturn  KECCAK224_update (
    hashState state,          /* previously initialized context */
    const BitSequence *buffer,  /* bit buffer, first bit is MSB in [0] */
    DataLength databitlen)      /* number of bits to process from buffer */
{
    /* can be called once or many times */
    /* verify correct hashbitlen and magic  */

    KECCAK_CTX *context = (KECCAK_CTX *) state;
    if (context->hashbitlen != HASH_BITLENGTH_KECCAK_224)
        return BAD_HASHBITLEN;

    if (context->magic != HASH_MAGIC_KECCAK_224)
        return BAD_ALGORITHM;

	return KECCAK_Update (context, buffer, databitlen);
}

HashReturn  KECCAK224_final (hashState state, BitSequence *hashval)
{
    KECCAK_CTX *context = (KECCAK_CTX *) state;
    if (context->hashbitlen != HASH_BITLENGTH_KECCAK_224)
        return BAD_HASHBITLEN;

    if (context->magic != HASH_MAGIC_KECCAK_224)
        return BAD_ALGORITHM;

	return KECCAK_Final (context, hashval);
}

HashReturn KECCAK224_hash (int hashbitlen, const BitSequence *data,
                      DataLength databitlen, BitSequence *hashval)
{
    hashState   state;
    HashReturn  retval;

    retval = KECCAK224_init (&state, HASH_BITLENGTH_KECCAK_224);
    if (retval != SUCCESS) {
        fprintf (stderr, "KECCAK224_init failed, reason %d, hash length %d\n",
                 retval, HASH_BITLENGTH_KECCAK_224);
        exit (1);
    }

    retval = KECCAK224_update (state, data, databitlen);
    if (retval != SUCCESS) {
        fprintf (stderr, "KECCAK224_update failed, reason %d\n", retval);
        exit (1);
    }

    retval = KECCAK224_final (state, hashval);
    if (retval != SUCCESS) {
        fprintf (stderr, "KECCAK224_final failed, reason %d\n", retval);
        exit (1);
    }
    free (state);
    return retval;
}

/*
 * three functions in MD5 style for each hash length
 */

/* Digests a file and prints the result.
 */
HashReturn KECCAK224_File (hashState state, FILE *in)
{
	KECCAK_CTX *context = (KECCAK_CTX *) state;
	int len;
	unsigned char buffer[BUFFERSIZE];
	HashReturn retval;

	/* verify correct hashbitlen and magic	*/
	if (context->hashbitlen != HASH_BITLENGTH_KECCAK_224)
		return BAD_HASHBITLEN;

	if (context->magic != HASH_MAGIC_KECCAK_224)
		return BAD_ALGORITHM;

	while ((len = fread (buffer, 1, BUFFERSIZE, in))) {
		retval = KECCAK224_update (context, buffer, (DataLength)len << 3);
		if (retval != SUCCESS) return retval;
	}
	retval = KECCAK224_final (context, NULL);

	fclose (in);
	return retval;
}

/* Prints a message digest in hexadecimal.
 */
void KECCAK224_Print (KECCAK_CTX *context)
{
    int i;
    for (i=0; i<HASH_LENGTH_KECCAK_224; i++) {
        printf ("%02x", context->out[i]);
    }
}

HashReturn KECCAK224_HashToByte (hashState state, BYTE *out) 
{
	KECCAK_CTX *context = (KECCAK_CTX *) state;

	/* verify correct hashbitlen and magic	*/
	if (context->hashbitlen != HASH_BITLENGTH_KECCAK_224)
		return BAD_HASHBITLEN;

	if (context->magic != HASH_MAGIC_KECCAK_224)
		return BAD_ALGORITHM;

	memcpy (out, context->out, HASH_LENGTH_KECCAK_224);
	return SUCCESS;
}

#endif

 /*************************** 256 ************************************/

HashReturn KECCAK256_init (hashState  *state, int hashbitlen)
{
    /* verify correct hash length   */
    if (hashbitlen != HASH_BITLENGTH_KECCAK_256)
        return BAD_HASHBITLEN;

    /* allocate context and fill it */
    KECCAK_CTX *context = (KECCAK_CTX *)malloc (sizeof (KECCAK_CTX));
    memset (context, 0, sizeof (KECCAK_CTX));
    context->hashbitlen = HASH_BITLENGTH_KECCAK_256;
    context->magic = HASH_MAGIC_KECCAK_256;
	*state = (hashState *) context;
	return KECCAK_Init (context, hashbitlen);
}

HashReturn  KECCAK256_update (
    hashState state,          /* previously initialized context */
    const BitSequence *buffer,  /* bit buffer, first bit is MSB in [0] */
    DataLength databitlen)      /* number of bits to process from buffer */
{
    /* can be called once or many times */
    /* verify correct hashbitlen and magic  */

    KECCAK_CTX *context = (KECCAK_CTX *) state;
    if (context->hashbitlen != HASH_BITLENGTH_KECCAK_256)
        return BAD_HASHBITLEN;

    if (context->magic != HASH_MAGIC_KECCAK_256)
        return BAD_ALGORITHM;

	return KECCAK_Update (context, buffer, databitlen);
}

HashReturn  KECCAK256_final (hashState state, BitSequence *hashval)
{
    KECCAK_CTX *context = (KECCAK_CTX *) state;
    if (context->hashbitlen != HASH_BITLENGTH_KECCAK_256)
        return BAD_HASHBITLEN;

    if (context->magic != HASH_MAGIC_KECCAK_256)
        return BAD_ALGORITHM;

	return KECCAK_Final (context, hashval);
}

HashReturn KECCAK256_hash (int hashbitlen, const BitSequence *data,
                      DataLength databitlen, BitSequence *hashval)
{
    hashState   state;
    HashReturn  retval;

    retval = KECCAK256_init (&state, HASH_BITLENGTH_KECCAK_256);
    if (retval != SUCCESS) {
    //    fprintf (stderr, "KECCAK256_init failed, reason %d, hash length %d\n",
    //             retval, HASH_BITLENGTH_KECCAK_256);
    //    exit (1);
    }
    else
    retval = KECCAK256_update (state, data, databitlen);

    if (retval != SUCCESS) {
        //fprintf (stderr, "KECCAK256_update failed, reason %d\n", retval);
        //exit (1);
    }
    else
    //retval = KECCAK256_final (state, hashval);
    	retval = KECCAK256_final (state, NULL);

    if (retval != SUCCESS) {
        //fprintf (stderr, "KECCAK256_final failed, reason %d\n", retval);
        //exit (1);
    } else
    retval = KECCAK256_HashToByte (state, hashval);

    free (state);
    return retval;
}

#ifdef GT_UNDEFINED
/*
 * three functions in MD5 style for each hash length
 */

/* Digests a file and prints the result.
 */
HashReturn KECCAK256_File (hashState state, FILE *in)
{
	KECCAK_CTX *context = (KECCAK_CTX *) state;
	int len;
	unsigned char buffer[BUFFERSIZE];
	HashReturn retval;

	/* verify correct hashbitlen and magic	*/
	if (context->hashbitlen != HASH_BITLENGTH_KECCAK_256)
		return BAD_HASHBITLEN;

	if (context->magic != HASH_MAGIC_KECCAK_256)
		return BAD_ALGORITHM;

	while ((len = fread (buffer, 1, BUFFERSIZE, in))) {
		retval = KECCAK256_update (context, buffer, (DataLength)len << 3);
		if (retval != SUCCESS) return retval;
	}
	retval = KECCAK256_final (context, NULL);

	fclose (in);
	return retval;
}

/* Prints a message digest in hexadecimal.
 */
void KECCAK256_Print (KECCAK_CTX *context)
{
    int i;
    for (i=0; i<HASH_LENGTH_KECCAK_256; i++) {
        printf ("%02x", context->out[i]);
	}
}

#endif

HashReturn KECCAK256_HashToByte (hashState state, BYTE *out) 
{
	KECCAK_CTX *context = (KECCAK_CTX *) state;

	/* verify correct hashbitlen and magic	*/
	if (context->hashbitlen != HASH_BITLENGTH_KECCAK_256)
		return BAD_HASHBITLEN;

	if (context->magic != HASH_MAGIC_KECCAK_256)
		return BAD_ALGORITHM;

	memcpy (out, context->out, HASH_LENGTH_KECCAK_256);
	return SUCCESS;
}

#ifdef GT_UNDEFINED
 /*************************** 384 ************************************/

HashReturn KECCAK384_init (hashState  *state, int hashbitlen)
{
    /* verify correct hash length   */
    if (hashbitlen != HASH_BITLENGTH_KECCAK_384)
        return BAD_HASHBITLEN;

    /* allocate context and fill it */
    KECCAK_CTX *context = (KECCAK_CTX *)malloc (sizeof (KECCAK_CTX));
    memset (context, 0, sizeof (KECCAK_CTX));
    context->hashbitlen = HASH_BITLENGTH_KECCAK_384;
    context->magic = HASH_MAGIC_KECCAK_384;
	*state = (hashState *) context;
	return KECCAK_Init (context, hashbitlen);
}

HashReturn  KECCAK384_update (
    hashState state,          /* previously initialized context */
    const BitSequence *buffer,  /* bit buffer, first bit is MSB in [0] */
    DataLength databitlen)      /* number of bits to process from buffer */
{
    /* can be called once or many times */
    /* verify correct hashbitlen and magic  */

    KECCAK_CTX *context = (KECCAK_CTX *) state;
    if (context->hashbitlen != HASH_BITLENGTH_KECCAK_384)
        return BAD_HASHBITLEN;

    if (context->magic != HASH_MAGIC_KECCAK_384)
        return BAD_ALGORITHM;

	return KECCAK_Update (context, buffer, databitlen);
}

HashReturn  KECCAK384_final (hashState state, BitSequence *hashval)
{
    KECCAK_CTX *context = (KECCAK_CTX *) state;
    if (context->hashbitlen != HASH_BITLENGTH_KECCAK_384)
        return BAD_HASHBITLEN;

    if (context->magic != HASH_MAGIC_KECCAK_384)
        return BAD_ALGORITHM;

	return KECCAK_Final (context, hashval);
}

HashReturn KECCAK384_hash (int hashbitlen, const BitSequence *data,
                      DataLength databitlen, BitSequence *hashval)
{
    hashState   state;
    HashReturn  retval;

    retval = KECCAK384_init (&state, HASH_BITLENGTH_KECCAK_384);
    if (retval != SUCCESS) {
        fprintf (stderr, "KECCAK384_init failed, reason %d, hash length %d\n",
                 retval, HASH_BITLENGTH_KECCAK_384);
        exit (1);
    }

    retval = KECCAK384_update (state, data, databitlen);
    if (retval != SUCCESS) {
        fprintf (stderr, "KECCAK384_update failed, reason %d\n", retval);
        exit (1);
    }

    retval = KECCAK384_final (state, hashval);
    if (retval != SUCCESS) {
        fprintf (stderr, "KECCAK384_final failed, reason %d\n", retval);
        exit (1);
    }
    free (state);
    return retval;
}

/*
 * three functions in MD5 style for each hash length
 */

/* Digests a file and prints the result.
 */
HashReturn KECCAK384_File (hashState state, FILE *in)
{
	KECCAK_CTX *context = (KECCAK_CTX *) state;
	int len;
	unsigned char buffer[BUFFERSIZE];
	HashReturn retval;

	/* verify correct hashbitlen and magic	*/
	if (context->hashbitlen != HASH_BITLENGTH_KECCAK_384)
		return BAD_HASHBITLEN;

	if (context->magic != HASH_MAGIC_KECCAK_384)
		return BAD_ALGORITHM;

	while ((len = fread (buffer, 1, BUFFERSIZE, in))) {
		retval = KECCAK384_update (context, buffer, (DataLength)len << 3);
		if (retval != SUCCESS) return retval;
	}
	retval = KECCAK384_final (context, NULL);

	fclose (in);
	return retval;
}

/* Prints a message digest in hexadecimal.
 */
void KECCAK384_Print (KECCAK_CTX *context)
{
    int i;
    for (i=0; i<HASH_LENGTH_KECCAK_384; i++) {
        printf ("%02x", context->out[i]);
    }
}

HashReturn KECCAK384_HashToByte (hashState state, BYTE *out) 
{
	KECCAK_CTX *context = (KECCAK_CTX *) state;

	/* verify correct hashbitlen and magic	*/
	if (context->hashbitlen != HASH_BITLENGTH_KECCAK_384)
		return BAD_HASHBITLEN;

	if (context->magic != HASH_MAGIC_KECCAK_384)
		return BAD_ALGORITHM;

	memcpy (out, context->out, HASH_LENGTH_KECCAK_384);
	return SUCCESS;
}


 /*************************** 512 ************************************/

HashReturn KECCAK512_init (hashState  *state, int hashbitlen)
{
    /* verify correct hash length   */
    if (hashbitlen != HASH_BITLENGTH_KECCAK_512)
        return BAD_HASHBITLEN;

    /* allocate context and fill it */
    KECCAK_CTX *context = (KECCAK_CTX *)malloc (sizeof (KECCAK_CTX));
    memset (context, 0, sizeof (KECCAK_CTX));
    context->hashbitlen = HASH_BITLENGTH_KECCAK_512;
    context->magic = HASH_MAGIC_KECCAK_512;
	*state = (hashState *) context;
	return KECCAK_Init (context, hashbitlen);
}

HashReturn  KECCAK512_update (
    hashState state,          /* previously initialized context */
    const BitSequence *buffer,  /* bit buffer, first bit is MSB in [0] */
    DataLength databitlen)      /* number of bits to process from buffer */
{
    /* can be called once or many times */
    /* verify correct hashbitlen and magic  */

    KECCAK_CTX *context = (KECCAK_CTX *) state;
    if (context->hashbitlen != HASH_BITLENGTH_KECCAK_512)
        return BAD_HASHBITLEN;

    if (context->magic != HASH_MAGIC_KECCAK_512)
        return BAD_ALGORITHM;

	return KECCAK_Update (context, buffer, databitlen);
}

HashReturn  KECCAK512_final (hashState state, BitSequence *hashval)
{
    KECCAK_CTX *context = (KECCAK_CTX *) state;
    if (context->hashbitlen != HASH_BITLENGTH_KECCAK_512)
        return BAD_HASHBITLEN;

    if (context->magic != HASH_MAGIC_KECCAK_512)
        return BAD_ALGORITHM;

	return KECCAK_Final (context, hashval);
}

HashReturn KECCAK512_hash (int hashbitlen, const BitSequence *data,
                      DataLength databitlen, BitSequence *hashval)
{
    hashState   state;
    HashReturn  retval;

    retval = KECCAK512_init (&state, HASH_BITLENGTH_KECCAK_512);
    if (retval != SUCCESS) {
        fprintf (stderr, "KECCAK512_init failed, reason %d, hash length %d\n",
                 retval, HASH_BITLENGTH_KECCAK_512);
        exit (1);
    }

    retval = KECCAK512_update (state, data, databitlen);
    if (retval != SUCCESS) {
        fprintf (stderr, "KECCAK512_update failed, reason %d\n", retval);
        exit (1);
    }

    retval = KECCAK512_final (state, hashval);
    if (retval != SUCCESS) {
        fprintf (stderr, "KECCAK512_final failed, reason %d\n", retval);
        exit (1);
    }
    free (state);
    return retval;
}

/*
 * three functions in MD5 style for each hash length
 */

/* Digests a file and prints the result.
 */
HashReturn KECCAK512_File (hashState state, FILE *in)
{
	KECCAK_CTX *context = (KECCAK_CTX *) state;
	int len;
	unsigned char buffer[BUFFERSIZE];
	HashReturn retval;

	/* verify correct hashbitlen and magic	*/
	if (context->hashbitlen != HASH_BITLENGTH_KECCAK_512)
		return BAD_HASHBITLEN;

	if (context->magic != HASH_MAGIC_KECCAK_512)
		return BAD_ALGORITHM;

	while ((len = fread (buffer, 1, BUFFERSIZE, in))) {
		retval = KECCAK512_update (context, buffer, (DataLength)len << 3);
		if (retval != SUCCESS) return retval;
	}
	retval = KECCAK512_final (context, NULL);

	fclose (in);
	return retval;
}

/* Prints a message digest in hexadecimal.
 */
void KECCAK512_Print (KECCAK_CTX *context)
{
    int i;
    for (i=0; i<HASH_LENGTH_KECCAK_512; i++) {
        printf ("%02x", context->out[i]);
    }
}

HashReturn KECCAK512_HashToByte (hashState state, BYTE *out) 
{
	KECCAK_CTX *context = (KECCAK_CTX *) state;

	/* verify correct hashbitlen and magic	*/
	if (context->hashbitlen != HASH_BITLENGTH_KECCAK_512)
		return BAD_HASHBITLEN;

	if (context->magic != HASH_MAGIC_KECCAK_512)
		return BAD_ALGORITHM;

	memcpy (out, context->out, HASH_LENGTH_KECCAK_512);
	return SUCCESS;
}

#endif
