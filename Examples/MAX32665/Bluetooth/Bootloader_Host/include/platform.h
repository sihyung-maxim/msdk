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

#ifndef EXAMPLES_MAX32665_BLUETOOTH_BOOTLOADER_HOST_INCLUDE_PLATFORM_H_
#define EXAMPLES_MAX32665_BLUETOOTH_BOOTLOADER_HOST_INCLUDE_PLATFORM_H_

/*******************************      INCLUDES    ****************************/

/*******************************      DEFINES     ****************************/

/******************************* Type Definitions ****************************/

/******************************* Public Functions ****************************/
// i2c
int plt_i2c_init(void);
int plt_i2c_write(unsigned char *src, unsigned int len);
int plt_i2c_read(unsigned char *dst, unsigned int len);

// spi
int plt_spi_init(void);
int plt_spi_write(unsigned char *src, unsigned int len);
int plt_spi_read(unsigned char *dst, unsigned int len);

// gpio
int plt_gpio_init(void);
void plt_gpio_set(unsigned int idx, int state);
int plt_gpio_get(unsigned int idx);

// delay
void plt_delay_ms(unsigned int ms);

#endif // EXAMPLES_MAX32665_BLUETOOTH_BOOTLOADER_HOST_INCLUDE_PLATFORM_H_
