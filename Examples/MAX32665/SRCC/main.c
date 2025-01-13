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
 * @brief       External Memory Cache Controller (SRCC) using SPIXR, writing to external SRAM
 * @details     Writing to External SRAM With Cache Enabled and Cache Disabled.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "mxc_device.h"
#include "mxc_errors.h"
#include "spixr.h"
#include "board.h"
#include "uart.h"
#include "srcc.h"
#include "rtc.h"

/***** Definitions *****/
// RAM Vendor Specific Commands
#define A1024_READ 0x03
#define A1024_WRITE 0x02
#define A1024_EQIO 0x38

// RAM Vendor Specific Values
#define BUFFER_SIZE 512
#define A1024_ADDRESS 0x80000000
#define ITERATIONS 5000

/***** Globals *****/
int fail = 0;
int s, ss;

mxc_spixr_cfg_t init_cfg = {
    0x08, /* Number of bits per character     */
    MXC_SPIXR_QUAD_SDIO, /* SPI Data Width                   */
    0x04, /* num of system clocks between SS active & first serial clock edge     */
    0x08, /* num of system clocks between last serial clock edge and ss inactive  */
    0x10, /* num of system clocks between transactions (read / write)             */
    500000, /* Baud freq                        */
};

/***** Functions *****/

int setup(void)
{
    uint8_t quad_cmd = A1024_EQIO; /* pre-defined command to use quad mode         */

    // Initialize the desired configuration
    if (MXC_SPIXR_Init(&init_cfg) != E_NO_ERROR) {
        printf("FAILED: SPIXR was not initialized properly.\n");
        return E_UNINITIALIZED;
    }

    /* Hide this with function in SPIXR.C later */
    MXC_SPIXR->dma &= ~MXC_F_SPIXR_DMA_RX_DMA_EN;
    MXC_SPIXR->dma |= MXC_F_SPIXR_DMA_TX_FIFO_EN;
    MXC_SPIXR->ctrl3 &= ~MXC_F_SPIXR_CTRL3_DATA_WIDTH;

    // Setup to communicate in quad mode
    MXC_SPIXR_SendCommand(&quad_cmd, 1, 1);
    // Wait until quad cmd is sent
    while (MXC_SPIXR_Busy()) {}

    MXC_SPIXR_SetWidth(MXC_SPIXR_QUAD_SDIO);
    MXC_SPIXR_ThreeWireModeDisable();
    MXC_SPIXR_DmaTXFIFODisable();
    MXC_SPIXR_DmaRXFIFODisable();
    MXC_SPIXR_TXFIFODisable();
    MXC_SPIXR_RXFIFODisable();

    MXC_SPIXR_ExMemUseDummy(0x01);
    MXC_SPIXR_ExMemSetReadCommand(A1024_READ);
    MXC_SPIXR_ExMemSetWriteCommand(A1024_WRITE);
    MXC_SPIXR_ExMemEnable();

    return E_NO_ERROR;
}

void start_timer(void)
{
    if (MXC_RTC_Init(0, 0) != E_NO_ERROR) {
        printf("Failed setup_timer.\n");
        return;
    }
    MXC_RTC_Start();
}

void stop_timer(void)
{
    printf("Time elapsed: %d.%03d \n", MXC_RTC->sec, MXC_RTC->ssec);
    MXC_RTC_Stop();
}

void test_function(void)
{
    // Defining Variable(s) to write & store data to RAM
    uint8_t write_buffer[BUFFER_SIZE], read_buffer[BUFFER_SIZE];
    uint8_t *address = (uint8_t *)A1024_ADDRESS;

    /* Variable to store address of RAM */
    int temp, i;
    unsigned int seed = 0;

    // Configure the SPIXR
    if (E_NO_ERROR != setup()) {
        fail += 1;
    }

    // Initialize & write pseudo-random data to be written to the RAM
    for (i = 0; i < BUFFER_SIZE; i++) {
        temp = rand_r(&seed);
        write_buffer[i] = temp;
        // Write the data to the RAM
        *(address + i) = temp;
    }

    start_timer();
    for (temp = 0; temp < ITERATIONS; temp++) {
        // Read data from RAM
        for (i = 0; i < BUFFER_SIZE; i++) {
            read_buffer[i] = *(address + i);
        }

        // Verify data being read from RAM
        if (memcmp(write_buffer, read_buffer, BUFFER_SIZE)) {
            printf("FAILED: Data was not read properly.\n\n");
            fail += 1;
            break;
        }
    }
    stop_timer();

    // Disable the SPIXR
    MXC_SPIXR_Disable();
}

// *****************************************************************************
int main(void)
{
    printf("***** SRCC Example *****\n");
    printf("Connect the jumper (JP7) to SPIRAM.\n\n");

    //Instruction cache enabled
    printf("Running test reads with data cache enabled.   ");
    MXC_SRCC_Enable();
    test_function();

    //Instruction cache disabled
    printf("Running test reads with data cache disabled.  ");
    MXC_SRCC_Disable();
    test_function();

    if (fail != 0) {
        printf("\nExample Failed\n");
        return E_FAIL;
    }

    printf("\nExample Succeeded\n");
    return E_NO_ERROR;
}
