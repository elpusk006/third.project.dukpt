/**
 * @file ripemd160.h
 * @brief RIPEMD-160 hash function
 *
 * @section License
 *
 * Copyright (C) 2010-2014 Oryx Embedded. All rights reserved.
 *
 * This file is part of CycloneCrypto Open.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded (www.oryx-embedded.com)
 * @version 1.5.0
 **/

#ifndef _RIPEMD160_H
#define _RIPEMD160_H

//Dependencies
#include "crypto.h"

//RIPEMD-160 block size
#define RIPEMD160_BLOCK_SIZE 64
//RIPEMD-160 digest size
#define RIPEMD160_DIGEST_SIZE 20
//Common interface for hash algorithms
#define RIPEMD160_HASH_ALGO (&ripemd160HashAlgo)


/**
 * @brief RIPEMD-160 algorithm context
 **/

typedef struct
{
   union
   {
      uint32_t h[5];
      uint8_t digest[20];
   };
   union
   {
      uint32_t x[16];
      uint8_t buffer[64];
   };
   size_t size;
   uint64_t totalSize;
} Ripemd160Context;


//RIPEMD-160 related constants
extern const HashAlgo ripemd160HashAlgo;

//RIPEMD-160 related functions
error_t ripemd160Compute(const void *data, size_t length, uint8_t *digest);
void ripemd160Init(Ripemd160Context *context);
void ripemd160Update(Ripemd160Context *context, const void *data, size_t length);
void ripemd160Final(Ripemd160Context *context, uint8_t *digest);
void ripemd160ProcessBlock(Ripemd160Context *context);

#endif
