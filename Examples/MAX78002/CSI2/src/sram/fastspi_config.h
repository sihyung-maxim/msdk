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
#ifndef EXAMPLES_MAX78002_CSI2_SRC_SRAM_FASTSPI_CONFIG_H_
#define EXAMPLES_MAX78002_CSI2_SRC_SRAM_FASTSPI_CONFIG_H_

#include "spi.h"
#include "gpio.h"

// TODO(Jake): Config struct
#define SPI MXC_SPI0
#define SPI_SPEED 30000000
// #define SPI_SPEED 1000000
#define SPI_PINS_PORT MXC_GPIO0
#define SPI_PINS_MASK \
    (MXC_GPIO_PIN_5 | MXC_GPIO_PIN_6 | MXC_GPIO_PIN_7 | MXC_GPIO_PIN_8 | MXC_GPIO_PIN_9)
#define SPI_SS_PORT MXC_GPIO0
#define SPI_SS_PIN MXC_GPIO_PIN_4

#endif // EXAMPLES_MAX78002_CSI2_SRC_SRAM_FASTSPI_CONFIG_H_
