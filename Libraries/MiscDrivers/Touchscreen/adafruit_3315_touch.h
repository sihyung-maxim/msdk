/**
 * @file    adafruit_3315_touch.h
 * @brief   Touchscreen driver for adafruit 3315
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

#ifndef LIBRARIES_MISCDRIVERS_TOUCHSCREEN_ADAFRUIT_3315_TOUCH_H_
#define LIBRARIES_MISCDRIVERS_TOUCHSCREEN_ADAFRUIT_3315_TOUCH_H_

#include <stdint.h>
#include <stdbool.h>
#include "spi.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TS_MAX_BUTTONS 16
#define TS_INVALID_KEY_CODE -1

extern volatile bool ts_event;

/************************************************************************************/
/**
 * @brief      Initialize the touchscreen and display
 *
 * @param      ts_spi           The SPI instance the touchscreen controller is connected to
 * @param      ss_idx           The SSEL index to use when communicating with the touchscreen controller
 * @param      reset_ctrl       The GPIO pin configuration for the touchscreen controller's interrupt pin
 * @param      bl_ctrl          The GPIO pin configuration for the touchscreen controller's busy pin
 *
 * @return     See \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_TS_Init(mxc_spi_regs_t *ts_spi, int ss_idx, mxc_gpio_cfg_t *int_pin,
                mxc_gpio_cfg_t *busy_pin);

/**
 * @brief      Enables touch interrupts
 *
 */
void MXC_TS_Start(void);

/**
 * @brief      Disables touch interrupts
 *
 */
void MXC_TS_Stop(void);

/**
 * @brief      Register a button
 *
* @param      x0, y0, x1, y1 location of button
* @param      on_press_expected_code  expected keycode when touch related point area
 */
int MXC_TS_AddButton(int x0, int y0, int x1, int y1, int on_press_expected_code);

/**
 * @brief      Remove a button
 *
 * @param      x0, y0, x1, y1 location of button
 */
void MXC_TS_RemoveButton(int x0, int y0, int x1, int y1);

/**
 * @brief      Remove all registered keys
 *
 */
void MXC_TS_RemoveAllButton(void);

/**
 * @brief      Read pressed key
 *
 */
int MXC_TS_GetKey(void);

/**
 * @brief      Read touch point
 *
 */
void MXC_TS_GetTouch(uint16_t *x, uint16_t *y);

#ifdef __cplusplus
}
#endif

#endif // LIBRARIES_MISCDRIVERS_TOUCHSCREEN_ADAFRUIT_3315_TOUCH_H_
