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
 * @brief   This example demonstrates the use of the Analog Comparator to wake up the device from sleep mode.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>

#include "mxc_delay.h"
#include "mxc_errors.h"
#include "mxc_pins.h"
#include "nvic_table.h"
#include "adc.h"
#include "dma.h"
#include "gpio.h"
#include "led.h"
#include "lp.h"
#include "pb.h"
#include "tmr.h"
#include "uart.h"

/***** Definitions *****/

/***** Globals *****/

/***** Functions *****/

void CMP_Handler(void)
{
    MXC_LPCMP_ClearFlags(MXC_LPCMP_CMP0);
    LED_Toggle(1);
}

int main(void)
{
    printf("********** Comparator Example **********\n");
    printf("\nConnect the analog signal used as the positive comparator input to analog pin 1.\n");
    printf("Connect the analog signal used as the negative comparator input to analog pin 0.\n");
    printf("\nThe device will be placed in sleep mode and requires a rising edge of the\n");
    printf("comparator output to wakeup.\n\n");

    printf("Press any user push button to begin.\n");
    while (!PB_IsPressedAny()) {}

    // Enable comparator 0
    MXC_LP_EnableLPCMPWakeup(MXC_LPCMP_CMP0);
    MXC_LPCMP_Init(MXC_LPCMP_CMP0);

    // Enable comparator interrupts
    MXC_LPCMP_EnableInt(MXC_LPCMP_CMP0, MXC_LPCMP_POL_RISE);
    MXC_NVIC_SetVector(LPCMP_IRQn, CMP_Handler);
    NVIC_EnableIRQ(LPCMP_IRQn);

    while (1) {
        printf("\nEntering sleep mode.\n");
        while (MXC_UART_GetActive(MXC_UART_GET_UART(CONSOLE_UART))) {}
        MXC_LP_EnterSleepMode();
        printf("Waking up.\n");
    }
}
