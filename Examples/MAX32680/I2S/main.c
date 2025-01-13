/**
 * @file        main.c
 * @brief       I2S Loopback Example
 * @details
 * @note
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

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "nvic_table.h"
#include "board.h"
#include "icc.h"
#include "i2s.h"
#include "tmr.h"
#include "dma.h"

#define DMA_CALLBACK 0

/***** Global Data *****/
uint16_t tone[64] = { 0x8000, 0x8c8b, 0x98f8, 0xa527, 0xb0fb, 0xbc56, 0xc71c, 0xd133,
                      0xda82, 0xe2f1, 0xea6d, 0xf0e2, 0xf641, 0xfa7c, 0xfd89, 0xff61,
                      0xffff, 0xff61, 0xfd89, 0xfa7c, 0xf641, 0xf0e2, 0xea6d, 0xe2f1,
                      0xda82, 0xd133, 0xc71c, 0xbc56, 0xb0fb, 0xa527, 0x98f8, 0x8c8b,
                      0x8000, 0x7374, 0x6707, 0x5ad8, 0x4f04, 0x43a9, 0x38e3, 0x2ecc,
                      0x257d, 0x1d0e, 0x1592, 0x0f1d, 0x09be, 0x0583, 0x0276, 0x009e,
                      0x0000, 0x009e, 0x0276, 0x0583, 0x09be, 0x0f1d, 0x1592, 0x1d0e,
                      0x257d, 0x2ecc, 0x38e3, 0x43a9, 0x4f04, 0x5ad8, 0x6707, 0x7374 };

uint16_t toneTX[64] = { 0 };
volatile uint8_t dma_flag;

/***** Functions *****/
void DMA0_IRQHandler(void)
{
    MXC_DMA_Handler();

#if DMA_CALLBACK == 0
    dma_flag = 0;
#endif
}

#if DMA_CALLBACK
void i2s_dma_cb(int ch, int err)
{
    dma_flag = 0;
}
#endif

/*****************************************************************/
int main(void)
{
    int err;
    mxc_i2s_req_t req;
    printf("\nI2S Transmission Example\n");
    printf("I2S Signals may be viewed on pins P1.2-P1.5.\n");
    printf("\n\n\n\n");

    req.wordSize = MXC_I2S_WSIZE_HALFWORD;
    req.sampleSize = MXC_I2S_SAMPLESIZE_SIXTEEN;
    req.bitsWord = 16;
    req.adjust = MXC_I2S_ADJUST_LEFT;
    req.justify = MXC_I2S_LSB_JUSTIFY;
    req.channelMode = MXC_I2S_INTERNAL_SCK_WS_0;
    req.clkdiv = 100;
    req.rawData = tone;
    req.txData = toneTX;
    req.length = 64;

    Console_Shutdown();

    if ((err = MXC_I2S_Init(&req)) != E_NO_ERROR) {
        Console_Init();
        printf("\nError in I2S_Init: %d\n", err);

        while (1) {}
    }

    MXC_DMA_ReleaseChannel(0);
    NVIC_EnableIRQ(DMA0_IRQn);

#if DMA_CALLBACK
    MXC_I2S_RegisterDMACallback(i2s_dma_cb);
#else
    MXC_I2S_RegisterDMACallback(NULL);
#endif

    dma_flag = 1;
    MXC_I2S_TXDMAConfig(toneTX, 64 * 2);

    /* Wait for DMA transactions to finish */
    while (dma_flag) {}

    /* Wait for I2S TX Buffer to empty */
    while (MXC_I2S->dmach0 & MXC_F_I2S_DMACH0_TX_LVL) {}

    if ((err = MXC_I2S_Shutdown()) != E_NO_ERROR) {
        Console_Init();
        printf("\nCould not shut down I2S driver: %d\n", err);

        while (1) {}
    }

    Console_Init();
    printf("\nI2S Transaction Complete. Ignore any random characters previously");
    printf("\ndisplayed. The I2S and UART are sharing the same pins.\n");

    return 0;
}
