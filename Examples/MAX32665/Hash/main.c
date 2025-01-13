/**
 * @file        main.c
 * @brief       Hash Example
 * @details
 */

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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_device.h"
#include "max32665.h"
#include "board.h"
#include "tpu.h"

/***** Globals *****/

char temp[] = { 0x00, 0x00, 0x00 };

void ascii_to_byte(const char *src, char *dst, int len)
{
    int i;
    for (i = 0; i < len; ++i) {
        int val;
        temp[0] = *src;
        src++;
        temp[1] = *src;
        src++;
        sscanf(temp, "%0x", &val);
        dst[i] = val;
    }
}

int Test_Hash(void)
{
    printf("Test Hash\n");

    unsigned char sha256_msg[] =
        "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890!@#$%&*()";

    char _sha256_result[] = "93bfb2299f7427f021ad038cec5054b4db2e935f3ae10d64e4e6a40a77269803";
    char sha256_result[33];
    ascii_to_byte(_sha256_result, sha256_result, 32);

    unsigned char destination[33];
    unsigned int msgLen = sizeof(sha256_msg) - 1;

    memset(destination, 0, sizeof(destination));

    // Reset Crypto Block
    MXC_TPU_Init(MXC_SYS_PERIPH_CLOCK_TPU);

    // Select the Hash Function
    MXC_TPU_Hash_Config(MXC_TPU_HASH_SHA256);

    MXC_TPU_Hash_SHA((char *)sha256_msg, MXC_TPU_HASH_SHA256, msgLen, (char *)destination);

    MXC_TPU_Shutdown(MXC_SYS_PERIPH_CLOCK_TPU);

    if (memcmp(sha256_result, destination, 32)) {
        printf(" * Failed *\n\n");
        return -1;
    }

    printf("   Passed  \n\n");
    return 0;
}

int main(void)
{
    printf("\n\n********** CTB Hash Example **********\n\n");

    int fail = 0;
    fail += Test_Hash();

    if (fail != 0) {
        printf("\nExample Failed\n");
        return E_FAIL;
    }

    printf("\nExample Succeeded\n");
    return E_NO_ERROR;
}
