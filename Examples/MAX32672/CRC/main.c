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
 * @file        main.c
 * @brief       Example showing how to use the CRC module. Covers 16 and 32-bit CRC.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_device.h"
#include "max32672.h"
#include "nvic_table.h"
#include "board.h"
#include "ctb.h"
#include "dma.h"

/***** Definitions *****/
#define POLY 0xEDB88320
#define CHECK 0xDEBB20E3

/***** Globals *****/
volatile int wait;
volatile int callback_result;
volatile int counter;

/***** Functions *****/
void CTB_IRQHandler(void)
{
    MXC_CTB_Handler();
}

void Test_Callback(void *req, int result)
{
    wait = 0;
    callback_result = result;
}

int Test_CRC(int asynchronous)
{
    int i;
    uint32_t array[101];
    mxc_ctb_crc_req_t crc_req = { (uint8_t *)&array, 400, 0, &Test_Callback };

    printf(asynchronous ? "Test CRC Async\n" : "Test CRC Sync\n");

    for (i = 0; i < 100; i++) {
        array[i] = i;
    }

    MXC_CTB_Init(MXC_CTB_FEATURE_CRC | MXC_CTB_FEATURE_DMA);

    // Load CRC polynomial into crc polynomial register
    MXC_CTB_CRC_SetPoly(POLY);

    // Compute CRC
    if (asynchronous) {
        wait = 1;
        MXC_CTB_EnableInt();
        MXC_CTB_CRC_ComputeAsync(&crc_req);

        while (wait) {}
    } else {
        MXC_CTB_CRC_Compute(&crc_req);
    }

    // Add CRC result to end of data array
    array[100] = ~(crc_req.resultCRC);
    crc_req.dataLen += sizeof(array[100]);

    // Compute CRC
    if (asynchronous) {
        wait = 1;
        MXC_CTB_CRC_ComputeAsync(&crc_req);

        while (wait) {}
    } else {
        MXC_CTB_CRC_Compute(&crc_req);
    }

    MXC_CTB_Shutdown(MXC_CTB_FEATURE_CRC | MXC_CTB_FEATURE_DMA);

    // Check CRC result
    if (CHECK != crc_req.resultCRC) {
        printf(" * Failed *\n\n");
        return -1;
    }

    printf("   Passed  \n\n");
    return 0;
}

// *****************************************************************************
int main(void)
{
    printf("\n************ CRC Example ***********\n");
    int fail = 0;

    // Test polling driven CRC compute
    fail += Test_CRC(0);

    // Test interrupt driven CRC compute
    NVIC_EnableIRQ(CRYPTO_IRQn);
    fail += Test_CRC(1);

    if (fail != 0) {
        printf("Example Failed\n");
        return E_FAIL;
    }

    printf("Example Succeeded\n");
    return E_NO_ERROR;
}
