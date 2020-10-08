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

#ifndef _KeccakSponge_h_
#define _KeccakSponge_h_

#define KeccakPermutationSize 1600
#define KeccakPermutationSizeInBytes (KeccakPermutationSize/8)
#define KeccakMaximumRate 1536
#define KeccakMaximumRateInBytes (KeccakMaximumRate/8)

#if defined(__GNUC__)
#define ALIGN __attribute__ ((aligned(32)))
#elif defined(_MSC_VER)
#define ALIGN __declspec(align(32))
#else
#define ALIGN
#endif

/* hash output length in bytes */
#define HASH_LENGTH_KECCAK_224 28
#define HASH_LENGTH_KECCAK_256 32
#define HASH_LENGTH_KECCAK_384 48
#define HASH_LENGTH_KECCAK_512 64

/* hash output length in bits */
#define HASH_BITLENGTH_KECCAK_224  224
#define HASH_BITLENGTH_KECCAK_256  256
#define HASH_BITLENGTH_KECCAK_384  384
#define HASH_BITLENGTH_KECCAK_512  512

/* hash input buffer length in bytes */
#define HASH_INPUTBUFFER_KECCAK_224    144
#define HASH_INPUTBUFFER_KECCAK_256    136
#define HASH_INPUTBUFFER_KECCAK_384    104
#define HASH_INPUTBUFFER_KECCAK_512    72

/* hash input buffer length in bits */
#define HASH_INPUTBUFFER_BITS_KECCAK_224   1152
#define HASH_INPUTBUFFER_BITS_KECCAK_256   1088
#define HASH_INPUTBUFFER_BITS_KECCAK_384   832
#define HASH_INPUTBUFFER_BITS_KECCAK_512   576

/* hash magic values - KECCAKxxx etc in little endian notation */
#define HASH_MAGIC_KECCAK_224  0x343232414343454bULL         /* KECCA224   */
#define HASH_MAGIC_KECCAK_256  0x363532414343454bULL         /* KECCA256   */
#define HASH_MAGIC_KECCAK_384  0x343833414343454bULL         /* KECCA384   */
#define HASH_MAGIC_KECCAK_512  0x323135414343454bULL         /* KECCA512   */



ALIGN typedef struct spongeStateStruct {
    ALIGN unsigned char state[KeccakPermutationSizeInBytes];
    ALIGN unsigned char dataQueue[KeccakMaximumRateInBytes];
    int hashbitlen;                   /*the message digest size*/
	/* magic token - must be HASH_MAGIC_KECCAK_xxx */
	DataLength	magic;
    unsigned int rate;
    unsigned int capacity;
    unsigned int bitsInQueue;
    unsigned int fixedOutputLength;
    int squeezing;
    unsigned int bitsAvailableForSqueezing;
    /* output buffer of hash, 512 bits  */
    BitSequence     out[HASH_LENGTH_KECCAK_512];
} spongeState;

typedef spongeState KECCAK_CTX;

/**
  * Function to initialize the state of the Keccak[r, c] sponge function.
  * The sponge function is set to the absorbing phase.
  * @param  state       Pointer to the state of the sponge function to be initialized.
  * @param  rate        The value of the rate r.
  * @param  capacity    The value of the capacity c.
  * @pre    One must have r+c=1600 and the rate a multiple of 64 bits in this implementation.
  * @return Zero if successful, 1 otherwise.
  */
int KECCAK_InitSponge(spongeState *state, unsigned int rate, unsigned int capacity);
/**
  * Function to give input data for the sponge function to absorb.
  * @param  state       Pointer to the state of the sponge function initialized by InitSponge().
  * @param  data        Pointer to the input data. 
  *                     When @a databitLen is not a multiple of 8, the last bits of data must be
  *                     in the least significant bits of the last byte.
  * @param  databitLen  The number of input bits provided in the input data.
  * @pre    In the previous call to Absorb(), databitLen was a multiple of 8.
  * @pre    The sponge function must be in the absorbing phase,
  *         i.e., Squeeze() must not have been called before.
  * @return Zero if successful, 1 otherwise.
  */
int KECCAK_Absorb(spongeState *state, const unsigned char *data, unsigned long long databitlen);
/**
  * Function to squeeze output data from the sponge function.
  * If the sponge function was in the absorbing phase, this function 
  * switches it to the squeezing phase.
  * @param  state       Pointer to the state of the sponge function initialized by InitSponge().
  * @param  output      Pointer to the buffer where to store the output data.
  * @param  outputLength    The number of output bits desired.
  *                     It must be a multiple of 8.
  * @return Zero if successful, 1 otherwise.
  */
int KECCAK_Squeeze(spongeState *state, unsigned char *output, unsigned long long outputLength);

#endif
