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

/**
 * @file   main.c
 * @brief  This example demonstrates the Modular Arithmetic Accelerator feature of the Trust Protection Unit.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_device.h"
#include "mxc_errors.h"
#include "board.h"
#include "tpu.h"

/***** Definitions *****/
#define ARR_SIZE 512

/***** Globals *****/
char temp[] = { 0x00, 0x00, 0x00 };

char multiplier_data[] = { 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe,
                           0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12 };

char multiplicand_data[] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

char exponent_data[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                         0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f };

char modulus_data[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f };

char result_data[] = { 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe,
                       0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12 };

/***** Functions *****/
void ascii_to_byte(const char *src, char *dst, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        int val;
        temp[0] = *src;
        src++;
        temp[1] = *src;
        src++;
        sscanf(temp, "%x", &val);
        dst[i] = val;
    }
    return;
}

unsigned int findLength(int num)
{
    unsigned int len = 0;
    len = num / 8;
    if (num % 8 != 0) {
        len++; //rounding up
    }
    return len;
}

int verify(int *result)
{
    printf("0x%08x\n", *(result + 0));
    printf("0x%08x\n", *(result + 1));
    printf("0x%08x\n", *(result + 2));
    printf("0x%08x\n", *(result + 3));

    return 0;
}

// *****************************************************************************
int main(void)
{
    printf("\n***** MAA Example *****\n");

    int result[ARR_SIZE];
    int len = 127;
    int retval, i, j;
    int fail = 0;

    retval = MXC_TPU_MAA_Init(len);
    if (retval != E_SUCCESS) {
        printf("Failed MAA_Init().\n");
        printf("Example Failed\n");
        return retval;
    }

    len = findLength(len);

    retval = MXC_TPU_MAA_Compute(MXC_TPU_MAA_EXP, multiplier_data, multiplicand_data, exponent_data,
                                 modulus_data, result, len);
    if (retval != E_SUCCESS) {
        printf("Failed MAA_Compute().\n");
        printf("Example Failed\n");
        return retval;
    }

    //Fit into four byte array
    printf("Computed:\n");
    for (i = 0; i < len / 4; ++i) {
        printf("result[%d] = 0x%x\n", i, *(result + i));
    }

    //Print out expected array to compare to calculated result
    printf("\nExpected:\n");
    for (i = 1; i <= len / 4; ++i) {
        printf("expected[%d] = 0x", i - 1);
        for (j = 0; j < 4; ++j) {
            printf("%02x", result_data[i * len / 4 - 1 - j]);
        }
        printf("\n");
    }

    fail += memcmp(result, result_data, sizeof(result_data));

    if (fail != 0) {
        printf("\nExample Failed\n");
        return E_FAIL;
    }

    printf("\nExample Succeeded\n");
    return E_NO_ERROR;
}
