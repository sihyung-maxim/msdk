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

/*
 * @file    main.c
 * @brief   Demonstrates a watchdog timer in run mode
 *
 * @details When the program starts LED1 blinks three times and stops.
 *          Then LED1 starts blinking continuously.
 *          Open a terminal program to see interrupt messages.
 *
 *          SW2: Push SW2 to trigger a watchdog reset. This will trigger either
 *          an underflow or overflow reset depending on whether OVERFLOW or
 *          UNDERFLOW is defined.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_device.h"
#include "nvic_table.h"
#include "board.h"
#include "mxc_sys.h"
#include "wdt.h"
#include "mxc_delay.h"
#include "led.h"
#include "pb.h"

/***** Definitions *****/
#define OVERFLOW //Test Windowed timer
//OVERFLOW
//UNDERFLOW

/***** Globals *****/
static mxc_wdt_cfg_t cfg;

/***** Functions *****/

// *****************************************************************************
void watchdogHandler(void)
{
    MXC_WDT_ClearIntFlag(MXC_WDT0);
    printf("\nTIMEOUT! \n");
}

// *****************************************************************************
void WDT0_IRQHandler(void)
{
    watchdogHandler();
}
// *****************************************************************************
void MXC_WDT_Setup(void)
{
    MXC_WDT_Disable(MXC_WDT0);
    MXC_WDT_ResetTimer(MXC_WDT0);
    MXC_WDT_Enable(MXC_WDT0);
}

void SW_Callback(void)
{
    printf("\nEnabling Timeout Interrupt...\n");
    MXC_WDT_Disable(MXC_WDT0);
    cfg.upperResetPeriod = MXC_WDT_PERIOD_2_28;
    cfg.upperIntPeriod = MXC_WDT_PERIOD_2_27;
    cfg.lowerResetPeriod = MXC_WDT_PERIOD_2_24;
    cfg.lowerIntPeriod = MXC_WDT_PERIOD_2_23;
    MXC_WDT_SetResetPeriod(MXC_WDT0, &cfg);
    MXC_WDT_SetIntPeriod(MXC_WDT0, &cfg);
    MXC_WDT_ResetTimer(MXC_WDT0);
    MXC_WDT_EnableReset(MXC_WDT0);
    MXC_WDT_EnableInt(MXC_WDT0);
    MXC_NVIC_SetVector(WDT0_IRQn, WDT0_IRQHandler);
    NVIC_EnableIRQ(WDT0_IRQn);
    MXC_WDT_Enable(MXC_WDT0);
}

void blinkled(int led, int num_of_blink, unsigned int ms_delay)
{
    for (int i = 0; i < num_of_blink; i++) {
        LED_On(led);
        MXC_Delay(MXC_DELAY_MSEC(ms_delay));
        LED_Off(led);
        MXC_Delay(MXC_DELAY_MSEC(ms_delay));
    }
}

// *****************************************************************************
int main(void)
{
    cfg.mode = MXC_WDT_WINDOWED;
    MXC_WDT_Init(MXC_WDT0, &cfg);

    if (MXC_WDT_GetResetFlag(MXC_WDT0)) {
        uint32_t resetFlags = MXC_WDT_GetResetFlag(MXC_WDT0);

        if (resetFlags == MXC_F_WDT_CTRL_RST_LATE) {
            printf("\nWatchdog Reset occured too late (OVERFLOW)\n");
        } else if (resetFlags == MXC_F_WDT_CTRL_RST_EARLY) {
            printf("\nWatchdog Reset occured too soon (UNDERFLOW)\n");
        }

        MXC_WDT_ClearResetFlag(MXC_WDT0);
        MXC_WDT_ClearIntFlag(MXC_WDT0);
        MXC_WDT_EnableReset(MXC_WDT0);
        MXC_WDT_Enable(MXC_WDT0);
    }

    printf("\n************** Watchdog Timer Demo ****************\n");
    printf("Watchdog timer is configured in Windowed mode. You can\n");
    printf("select between two tests: Timer Overflow and Underflow.\n");
    printf("\nPress a button to create watchdog interrupt and reset:\n");
#if defined(EvKit_V1)
    printf("SW2 (P4.0) = timeout and reset program\n\n");
#else
    printf("SW3 (P1.14) = timeout and reset program\n\n");
#endif

    //Blink LED0 three times at startup
    blinkled(0, 3, 100);

    //Setup watchdog
    MXC_WDT_Setup();

    //Push SW2/SW3 to start longer delay - shows Interrupt before the reset happens

    while (1) {
        //Push SW2/SW3 to reset watchdog
        if (PB_Get(0)) {
            SW_Callback();
#ifdef OVERFLOW

            while (1) {}

#else
            MXC_Delay(MXC_DELAY_MSEC(200));
            MXC_WDT_ResetTimer(MXC_WDT0);
#endif
        }

        //blink LED0
        blinkled(0, 1, 500);

        //Reset watchdog
        MXC_WDT_ResetTimer(MXC_WDT0);
    }
}
