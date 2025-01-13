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
 * @details SW2: Push SW2 to trigger a "too-late" watchdog reset. This will stop resetting the
 *               watchdog timer until it generates the "too-late" interrupt.  After that it will
 *               reset the watchdog timer only once, allowing it to pass the reset timeout period.
 *
 *          SW3: Push SW3 to reset the watchdog timer in the "too-early" period.
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

/***** Globals *****/
static mxc_wdt_cfg_t cfg;

volatile int sw1_pressed = 0;
volatile int sw2_pressed = 0;
volatile int interrupt_count = 0;

// refers to array, do not change constants
#define SW1 0

/***** Functions *****/

// *****************************************************************************
void watchdogHandler(void)
{
    MXC_WDT_ClearIntFlag(MXC_WDT0);

    if (interrupt_count == 0) {
        printf("\nWatchdog has tripped!\n");
        printf("This is the first time, so we'll go ahead and reset it\n");
        printf("once it is within the proper window.\n");
        interrupt_count++;
    } else {
        printf("\nWatchdog has tripped!\n");
        printf("This is the not the first time.  What happens if we\n");
        printf("do not reset it?\n");
    }
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

void SW1_Callback(void *pb)
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
    sw1_pressed = 1;
    PB_RegisterCallback(0, NULL);
}

void SW2_Callback(void *pb)
{
    printf("What happens if the watchdog is reset too early?\n");
    sw2_pressed = 1;
    PB_RegisterCallback(1, NULL);
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
    printf("SW2: Push SW2 to trigger a \"too-late\" watchdog reset. This will stop resetting\n");
    printf("     the watchdog timer until it generates the \"too-late\" interrupt.  After that\n");
    printf("     it will reset the watchdog timer only once, allowing it to pass the reset\n");
    printf("     timeout period.\n\n");
    printf("SW3: Push SW3 to reset the watchdog timer in the \"too-early\" period.\n");

    //Blink LED0 three times at startup
    blinkled(0, 3, 100);

    //Setup and start watchdog
    MXC_WDT_Setup();

    // Configure push buttons
    PB_RegisterCallback(0, SW1_Callback);
    PB_IntEnable(0);
    PB_RegisterCallback(1, SW2_Callback);
    PB_IntEnable(1);

    //Push SW1 to start longer delay - shows Interrupt before the reset happens

    while (1) {
        if (sw1_pressed) {
            if (interrupt_count == 0) {
                while (interrupt_count == 0) {}

                MXC_Delay(MXC_DELAY_MSEC(1500));
            } else {
                while (1) {}
            }
        }

        if (sw2_pressed) {
            // Reset the WDT too early.
            MXC_Delay(MXC_DELAY_MSEC(200));
            MXC_WDT_ResetTimer(MXC_WDT0);
            sw2_pressed = 0;
        }

        //blink LED0
        blinkled(0, 1, 500);

        //Reset watchdog
        MXC_WDT_ResetTimer(MXC_WDT0);
    }
}
