/**
 * @file 	board.h
 * @brief   Board support package API.
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

#include <stdio.h>
#include "spixf.h"

#ifndef LIBRARIES_BOARDS_MAX32650_EVKIT_V1_INCLUDE_BOARD_H_
#define LIBRARIES_BOARDS_MAX32650_EVKIT_V1_INCLUDE_BOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#define BOARD_EVKIT_V1 1 /// Used in examples to control program flow.

#ifndef CONSOLE_UART
#define CONSOLE_UART 0 /// UART instance to use for console
#endif

#ifndef CONSOLE_BAUD
#define CONSOLE_BAUD 115200 /// Console baud rate
#endif

#ifdef LED_OFF
#undef LED_OFF
#endif
#define LED_OFF 1 /// Override inactive state of LEDs

#ifdef LED_ON
#undef LED_ON
#endif
#define LED_ON 0 /// Override active state of LEDs

#ifndef EXT_FLASH_BAUD
#define EXT_FLASH_BAUD 4000000
#endif

#define LED1 0
#define LED_RED LED1
#define LED2 1
#define LED_GREEN LED2

/**
 * @brief   Initialize the BSP and board interfaces.
 * @return  #E_NO_ERROR if everything is successful
 */
int Board_Init(void);

/**
 * @brief   Initialize or reinitialize the console. This may be necessary if the
 *          system clock rate is changed.
 * @return  #E_NO_ERROR if everything is successful
 */
int Console_Init(void);

/**
 * @brief   Attempt to prepare the console for sleep.
 * @return  #E_NO_ERROR if ready to sleep, #E_BUSY if not ready for sleep.
 */
int Console_PrepForSleep(void);

/**
 * @brief   Initialize the PMIC to output correct voltages  only needed for rev 1,2, and 3 boards/
 * @return  #E_NO_ERROR if everything is successful
 */
int MAX77650_Init(void);

#ifdef __cplusplus
}
#endif

#endif // LIBRARIES_BOARDS_MAX32650_EVKIT_V1_INCLUDE_BOARD_H_
