/**
 * @file        main.c
 * @brief       I2S Example
 * @note        I2S is configured to send Audio Data on P2.4
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

/* **** Includes **** */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "mxc_sys.h"
#include "mxc_errors.h"
#include "mxc_delay.h"
#include "i2s.h"
#include "dma.h"
#include "tmr.h"

uint16_t tone[64] = { 0x8000, 0x8c8b, 0x98f8, 0xa527, 0xb0fb, 0xbc56, 0xc71c, 0xd133,
                      0xda82, 0xe2f1, 0xea6d, 0xf0e2, 0xf641, 0xfa7c, 0xfd89, 0xff61,
                      0xffff, 0xff61, 0xfd89, 0xfa7c, 0xf641, 0xf0e2, 0xea6d, 0xe2f1,
                      0xda82, 0xd133, 0xc71c, 0xbc56, 0xb0fb, 0xa527, 0x98f8, 0x8c8b,
                      0x8000, 0x7374, 0x6707, 0x5ad8, 0x4f04, 0x43a9, 0x38e3, 0x2ecc,
                      0x257d, 0x1d0e, 0x1592, 0x0f1d, 0x09be, 0x0583, 0x0276, 0x009e,
                      0x0000, 0x009e, 0x0276, 0x0583, 0x09be, 0x0f1d, 0x1592, 0x1d0e,
                      0x257d, 0x2ecc, 0x38e3, 0x43a9, 0x4f04, 0x5ad8, 0x6707, 0x7374 };

/* **** Definitions **** */

/* **** Global Data **** */
mxc_i2s_config_t cfg;

void DMA0_IRQHandler(void)
{
    MXC_DMA_Handler();
}

/* Reset DMA Reload so we loop the audio sample forever */
void dma_ctz_cb(int ch, int err)
{
    MXC_I2S_DMA_SetReload(cfg.src_addr, cfg.dst_addr, cfg.length);
    MXC_I2S_DMA_ClearFlags();
}

/*****************************************************************/
int main(void)
{
    int err;

    NVIC_EnableIRQ(DMA0_IRQn);

    printf("*********** I2S Example **********\n");

    cfg.justify = LEFT_JUSTIFIED;
    cfg.audio_mode = MONO_MODE;
    cfg.audio_direction = AUDIO_OUT;
    cfg.sample_rate = 16000;
    cfg.start_immediately = 0;
    cfg.src_addr = tone;
    cfg.dst_addr = NULL;
    cfg.length = 128;
    cfg.dma_reload_en = 1;

    if ((err = MXC_I2S_Init(&cfg, dma_ctz_cb)) != E_NO_ERROR) {
        printf("Error in I2S_Init: %d\n", err);
        while (1) {}
    }

    printf("I2S Configured\n");

    MXC_TMR_Delay(MXC_TMR0, MXC_DELAY_MSEC(1000));

    printf("Starting I2S Output\n");
    MXC_I2S_Start();
    MXC_TMR_Delay(MXC_TMR0, MXC_DELAY_MSEC(5000));

    printf("Muting I2S Output\n");
    MXC_I2S_Mute();
    MXC_TMR_Delay(MXC_TMR0, MXC_DELAY_MSEC(1000));

    printf("Unmuting I2S Output\n");
    MXC_I2S_Unmute();
    MXC_TMR_Delay(MXC_TMR0, MXC_DELAY_MSEC(500));

    printf("Pausing I2S Output\n");
    MXC_I2S_Pause();
    MXC_TMR_Delay(MXC_TMR0, MXC_DELAY_MSEC(500));

    printf("Resuming I2S Output\n");
    MXC_I2S_Unpause();
    MXC_TMR_Delay(MXC_TMR0, MXC_DELAY_MSEC(500));

    printf("Stopping I2S Output. Example Complete.\n");
    MXC_I2S_Stop();

    if ((err = MXC_I2S_Shutdown()) != E_NO_ERROR) {
        printf("Could not shut down I2S driver: %d\n", err);
        while (1) {}
    }

    return 0;
}
