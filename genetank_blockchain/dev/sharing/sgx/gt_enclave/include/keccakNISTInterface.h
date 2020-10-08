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

#ifndef _KeccakNISTInterface_h_
#define _KeccakNISTInterface_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "keccakSponge.h"

#if 0
typedef unsigned char BitSequence;
typedef unsigned long long DataLength;
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHLEN = 2 } HashReturn;

typedef spongeState hashState;
#endif

/**
  * Function to initialize the state of the Keccak[r, c] sponge function.
  * The rate r and capacity c values are determined from @a hashbitlen.
  * @param  state       Pointer to the state of the sponge function to be initialized.
  * @param  hashbitlen  The desired number of output bits, 
  *                     or 0 for Keccak[] with default parameters
  *                     and arbitrarily-long output.
  * @pre    The value of hashbitlen must be one of 0, 224, 256, 384 and 512.
  * @return SUCCESS if successful, BAD_HASHLEN if the value of hashbitlen is incorrect.
  */
extern HashReturn KECCAK_Init(spongeState *state, int hashbitlen);
/**
  * Function to give input data for the sponge function to absorb.
  * @param  state       Pointer to the state of the sponge function initialized by Init().
  * @param  data        Pointer to the input data. 
  *                     When @a databitLen is not a multiple of 8, the last bits of data must be
  *                     in the most significant bits of the last byte.
  * @param  databitLen  The number of input bits provided in the input data.
  * @pre    In the previous call to Absorb(), databitLen was a multiple of 8.
  * @return SUCCESS if successful, FAIL otherwise.
  */
extern HashReturn KECCAK_Update(spongeState *state, const BitSequence *data, DataLength databitlen);
/**
  * Function to squeeze output data from the sponge function.
  * If @a hashbitlen was not 0 in the call to Init(), the number of output bits is equal to @a hashbitlen.
  * If @a hashbitlen was 0 in the call to Init(), the output bits must be extracted using the Squeeze() function.
  * @param  state       Pointer to the state of the sponge function initialized by Init().
  * @param  hashval     Pointer to the buffer where to store the output data.
  * @return SUCCESS if successful, FAIL otherwise.
  */
extern HashReturn KECCAK_Final(spongeState *state, BitSequence *hashval);
/**
  * Function to compute a hash using the Keccak[r, c] sponge function.
  * The rate r and capacity c values are determined from @a hashbitlen.
  * @param  hashbitlen  The desired number of output bits.
  * @param  data        Pointer to the input data. 
  *                     When @a databitLen is not a multiple of 8, the last bits of data must be
  *                     in the most significant bits of the last byte.
  * @param  databitLen  The number of input bits provided in the input data.
  * @param  hashval     Pointer to the buffer where to store the output data.
  * @pre    The value of hashbitlen must be one of 224, 256, 384 and 512.
  * @return SUCCESS if successful, BAD_HASHLEN if the value of hashbitlen is incorrect.
  */
extern HashReturn KECCAK_Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

/* 
 * parameter safe wrappers for KECCAK routines for each hash length
 */

/*********** KECCAK224 definitions *********/
/* initialize context */
extern HashReturn KECCAK224_init (hashState  *state, int hashbitlen);
/* update context, may be called many times */
extern HashReturn  KECCAK224_update (
    hashState state,          /* previously initialized context */
    const BitSequence *buffer,  /* bit buffer, first bit is MSB in [0] */
    DataLength databitlen);      /* number of bits to process from buffer */
/* produce hash and return in hashval */
extern HashReturn  KECCAK224_final (hashState state, BitSequence *hashval);
/* calculate hash all-in-one */
HashReturn KECCAK224_hash (int hashbitlen, const BitSequence *data,
                      DataLength databitlen, BitSequence *hashval);

//extern HashReturn KECCAK224_File (hashState state, FILE *in);
//extern void KECCAK224_Print (KECCAK_CTX *context);
extern HashReturn KECCAK224_HashToByte (hashState state, BYTE *out);

/*********** KECCAK256 definitions *********/
/* initialize context */
extern HashReturn KECCAK256_init (hashState  *state, int hashbitlen);
/* update context, may be called many times */
extern HashReturn  KECCAK256_update (
    hashState state,          /* previously initialized context */
    const BitSequence *buffer,  /* bit buffer, first bit is MSB in [0] */
    DataLength databitlen);      /* number of bits to process from buffer */
/* produce hash and return in hashval */
extern HashReturn  KECCAK256_final (hashState state, BitSequence *hashval);
/* calculate hash all-in-one */
HashReturn KECCAK256_hash (int hashbitlen, const BitSequence *data,
                      DataLength databitlen, BitSequence *hashval);

//extern HashReturn KECCAK256_File (hashState state, FILE *in);
//extern void KECCAK256_Print (KECCAK_CTX *context);
extern HashReturn KECCAK256_HashToByte (hashState state, BYTE *out);

/*********** KECCAK384 definitions *********/
/* initialize context */
extern HashReturn KECCAK384_init (hashState  *state, int hashbitlen);
/* update context, may be called many times */
extern HashReturn  KECCAK384_update (
    hashState state,          /* previously initialized context */
    const BitSequence *buffer,  /* bit buffer, first bit is MSB in [0] */
    DataLength databitlen);      /* number of bits to process from buffer */
/* produce hash and return in hashval */
extern HashReturn  KECCAK384_final (hashState state, BitSequence *hashval);
/* calculate hash all-in-one */
HashReturn KECCAK384_hash (int hashbitlen, const BitSequence *data,
                      DataLength databitlen, BitSequence *hashval);

//extern HashReturn KECCAK384_File (hashState state, FILE *in);
//extern void KECCAK384_Print (KECCAK_CTX *context);
extern HashReturn KECCAK384_HashToByte (hashState state, BYTE *out);

/*********** KECCAK512 definitions *********/
/* initialize context */
extern HashReturn KECCAK512_init (hashState  *state, int hashbitlen);
/* update context, may be called many times */
extern HashReturn  KECCAK512_update (
    hashState state,          /* previously initialized context */
    const BitSequence *buffer,  /* bit buffer, first bit is MSB in [0] */
    DataLength databitlen);      /* number of bits to process from buffer */
/* produce hash and return in hashval */
extern HashReturn  KECCAK512_final (hashState state, BitSequence *hashval);
/* calculate hash all-in-one */
HashReturn KECCAK512_hash (int hashbitlen, const BitSequence *data,
                      DataLength databitlen, BitSequence *hashval);

//extern HashReturn KECCAK512_File (hashState state, FILE *in);
//extern void KECCAK512_Print (KECCAK_CTX *context);
extern HashReturn KECCAK512_HashToByte (hashState state, BYTE *out);

#ifdef __cplusplus
}
#endif

#endif
