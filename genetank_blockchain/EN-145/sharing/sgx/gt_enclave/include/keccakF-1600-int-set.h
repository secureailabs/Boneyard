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

#define ProvideFast576
#define ProvideFast832
#define ProvideFast1024
#define ProvideFast1088
#define ProvideFast1152
#define ProvideFast1344
