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
 * @file    main.c
 * @brief   Wake-Up Timer (WUT) example for low power modes.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_device.h"
#include "led.h"
#include "board.h"
#include "mxc_delay.h"
#include "mxc.h"
#include "wut.h"

/***** Definitions *****/
#define MILLISECONDS_WUT 5000

void WUT_IRQHandler(void)
{
    MXC_WUT_ClearFlags(MXC_WUT0);
}

// *****************************************************************************
int main(void)
{
    mxc_wut_cfg_t cfg;
    uint32_t ticks;

    printf("/************** Wakeup timer example ********************/\n");
    printf("This example is to show how the Wakeup timer is used and configured\n");
    printf("Press PB1 to put the chip into sleep and then the wakeup timer will wake up in %d "
           "Miliseconds \n",
           MILLISECONDS_WUT);

    // Get ticks based off of milliseconds
    MXC_WUT_GetTicks(MXC_WUT0, MILLISECONDS_WUT, MXC_WUT_UNIT_MILLISEC, &ticks);

    // config structure for one shot timer to trigger in a number of ticks
    cfg.mode = MXC_WUT_MODE_ONESHOT;
    cfg.cmp_cnt = ticks;

    // Init WUT
    MXC_WUT_Init(MXC_WUT0, MXC_WUT_PRES_1);

    //Config WUT
    MXC_WUT_Config(MXC_WUT0, &cfg);
    MXC_LP_EnableWUTAlarmWakeup();

    NVIC_EnableIRQ(WUT_IRQn);

    while (1) {
        if (PB_Get(0) == TRUE) {
            printf("Entering SLEEP mode.\n");

            MXC_WUT_Enable(MXC_WUT0);

            // wait until UART transmit
            while (MXC_UART_ReadyForSleep(MXC_UART_GET_UART(CONSOLE_UART)) != E_NO_ERROR) {}

            MXC_LP_EnterSleepMode();

            printf("Waking up from SLEEP mode.\n");
        }
    }
}
