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

/*******************************      INCLUDES    ****************************/
#include <stdio.h>
#include <string.h>

#include "mxc_delay.h"

#include "spi_config.h"

/*******************************      DEFINES     ****************************/

/******************************* Type Definitions ****************************/

/*******************************     Variables    ****************************/
static unsigned char g_rxBuff[256];
static mxc_spi_req_t g_slave_req = { 0 };

static volatile int is_transaction_done = 0;

/******************************* Static Functions ****************************/
void SPIx_IRQHandler(void)
{
    MXC_SPI_AsyncHandler(SPIx_SLAVE);
}

void spi_transaction_cb(void *req, int error_code)
{
    (void)error_code;
    (void)req;

    is_transaction_done = 1;
}

/******************************* Public Functions ****************************/
int spi_slave_init(void)
{
    int ret = 0;
    int masterMode = 0;
    int quadModeUsed = 0;
    int numSlaves = 1;
    int ssPolarity = 0;

    ret = MXC_SPI_Init(SPIx_SLAVE, masterMode, quadModeUsed, numSlaves, ssPolarity, SPI_BAUD_RATE);
    if (ret) {
        return ret;
    }

    MXC_SPI_SetDataSize(SPIx_SLAVE, 8);
    MXC_SPI_SetWidth(SPIx_SLAVE, SPI_WIDTH_STANDARD);

    NVIC_EnableIRQ(SPIx_IRQn);

    return ret;
}

int spi_slave_send(unsigned char *src, unsigned int srcLen)
{
    int ret = 0;

    g_slave_req.spi = SPIx_SLAVE;
    g_slave_req.ssIdx = 0; // not meaning for slave
    g_slave_req.ssDeassert = 1;
    g_slave_req.txData = src;
    g_slave_req.rxData = g_rxBuff;
    g_slave_req.txLen = srcLen;
    g_slave_req.rxLen = srcLen;
    g_slave_req.txCnt = 0;
    g_slave_req.rxCnt = 0;
    g_slave_req.completeCB = spi_transaction_cb;

    ret = MXC_SPI_SlaveTransactionAsync(&g_slave_req);

    return ret;
}

int spi_slave_rcv(unsigned char *dst, unsigned int maxLen, unsigned int *len)
{
    int ret = 0;
    int counter = 100;

    while (is_transaction_done == 0) {
        if (--counter == 0) {
            break;
        }
        MXC_Delay(MXC_DELAY_MSEC(1));
    }

    memcpy(dst, g_rxBuff, g_slave_req.rxCnt);
    *len = g_slave_req.rxCnt;

    return ret;
}
