/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by 
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include <ucl/ucl_config.h>
#include <ucl/ucl_types.h>
#include <ucl/ucl_sys.h>
#include <ucl/ucl_defs.h>
#include <ucl/ucl_retdefs.h>
#include <ucl/ucl_rng.h>
#include <ucl/ucl_hash.h>
#ifdef HASH_SHA256
#include <ucl/ucl_sha256.h>
#endif


//this is not secure for ECDSA signatures, as being pseudo random number generator
//this is for test and demo only
int ucl_rng_read(u8 *rand, u32 rand_byteLen)
{
    int msgi, j;
    static u8 pseudo[16] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x00, 0x11, 0x22, 0x33, 0x44};
    u8 output[32], input[16];
    u8 blocksize;
    blocksize = 16;

    for (msgi = 0; msgi < (int)rand_byteLen;) {
        for (j = 0; j < blocksize; j++) {
            input[j] = pseudo[j];
        }

        ucl_sha256(output, input, blocksize);

        for (j = 0; j < blocksize; j++) {
            pseudo[j] = output[j];
        }

        for (j = 0; j < blocksize; j++) {
            if (msgi < (int)rand_byteLen) {
                rand[msgi] = output[j];
                msgi++;
            }
        }
    }

    return(rand_byteLen);
}
