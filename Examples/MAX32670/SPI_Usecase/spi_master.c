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

#include "spi_config.h"

/*******************************      DEFINES     ****************************/

/******************************* Type Definitions ****************************/

/*******************************     Variables    ****************************/

/******************************* Static Functions ****************************/

/******************************* Public Functions ****************************/
int spi_master_init(void)
{
    int ret = 0;
    int masterMode = 1;
    int quadModeUsed = 0;
    int numSlaves = 1;
    int ssPolarity = 0;

    ret = MXC_SPI_Init(SPIx_MASTER, masterMode, quadModeUsed, numSlaves, ssPolarity, SPI_BAUD_RATE);
    if (ret) {
        return ret;
    }

    MXC_SPI_SetDataSize(SPIx_MASTER, 8);
    MXC_SPI_SetWidth(SPIx_MASTER, SPI_WIDTH_STANDARD);

    return ret;
}

int spi_master_send_rcv(unsigned char *src, unsigned int srcLen, unsigned char *dst)
{
    int ret = 0;
    mxc_spi_req_t req;

    req.spi = SPIx_MASTER;
    req.txData = (uint8_t *)src;
    req.rxData = (uint8_t *)dst;
    req.txLen = srcLen;
    req.rxLen = srcLen;
    req.ssIdx = 0; // SS0 is connected
    req.ssDeassert = 1;
    req.txCnt = 0;
    req.rxCnt = 0;
    req.completeCB = NULL;

    ret = MXC_SPI_MasterTransaction(&req);

    return ret;
}
