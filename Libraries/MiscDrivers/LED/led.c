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

#include "board.h"
#include "led.h"
#include "mxc_device.h"

/******************************************************************************/
int LED_Init(void)
{
    int retval = E_NO_ERROR;
    unsigned int i;

    /* setup GPIO for the LED */
    for (i = 0; i < num_leds; i++) {
        LED_Off(i); // Set the output value

        if (MXC_GPIO_Config(&led_pin[i]) != E_NO_ERROR) {
            retval = E_UNKNOWN;
        }
    }

    return retval;
}

//******************************************************************************
void LED_On(unsigned int idx)
{
    MXC_ASSERT(idx < num_leds);
#if (LED_ON == 0)
    MXC_GPIO_OutClr(led_pin[idx].port, led_pin[idx].mask);
#else
    MXC_GPIO_OutSet(led_pin[idx].port, led_pin[idx].mask);
#endif
}

//******************************************************************************
void LED_Off(unsigned int idx)
{
    MXC_ASSERT(idx < num_leds);
#if (LED_ON == 0)
    MXC_GPIO_OutSet(led_pin[idx].port, led_pin[idx].mask);
#else
    MXC_GPIO_OutClr(led_pin[idx].port, led_pin[idx].mask);
#endif
}

//******************************************************************************
void LED_Toggle(unsigned int idx)
{
    MXC_ASSERT(idx < num_leds);
    MXC_GPIO_OutToggle(led_pin[idx].port, led_pin[idx].mask);
}
