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
 * @brief   Resource Protection Unit Example
 * @details This example uses the resource protection unit to block DMA from reading the
 *          key out of the crypto module during an operation. The attempt to read will result
 *          in a hardfault which ends the example
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "mxc_errors.h"
#include "rpu.h"
#include "tmr.h"
#include "led.h"

/***** Definitions *****/

/***** Globals *****/

/***** Functions *****/
void HardFault_Handler(void)
{
    printf("\n\nHard Fault reached\n");
    printf("Press reset to run the example again\n");
    printf("Example Complete\n");
    while (1) {}
}

int main_core1(void)
{
    int err;
    // The RPU defaults to all access enabled
    // We need to disallow everything we don't want to access
    // the RPU_Allow function can only be used to selectively
    // allow masters that have already been disallowed
    err = MXC_RPU_Disallow(MXC_RPU_TMR3,
                           (~MXC_RPU_SYS1_ALLOW) & 0x1FF); // Acquire exclusive access to TMR3

    if (err == E_BAD_STATE) {
        LED_On(0);
        while (1) {}
    } else if (err == E_BAD_PARAM) {
        while (1) {}
    }

    LED_On(1);

    while (1) {}
}

// *****************************************************************************
int main(void)
{
    printf("********* Resource Protection Unit Example **********\n");
    printf("This example uses the resource protection unit to prevent\n");
    printf("code running in Core 0 from accessing the timer in use by Core 1\n\n");

    LED_On(1);
    MXC_Delay(500000);
    LED_Off(1);

    Start_Core1();
    MXC_Delay(1000);

    // Try to read TMR3's config register
    uint32_t invalidaddr = MXC_TMR3->cn;

    printf("TMR3 Control Register: 0x%08x\n", invalidaddr);

    printf("Did not fault\n");
    while (1) {}
}
