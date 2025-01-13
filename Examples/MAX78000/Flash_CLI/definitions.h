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

#ifndef EXAMPLES_MAX78000_FLASH_CLI_DEFINITIONS_H_
#define EXAMPLES_MAX78000_FLASH_CLI_DEFINITIONS_H_

#include "flc.h"

#define VERBOSE_LOGGING

#ifdef VERBOSE_LOGGING
#define LOGV(...) printf(__VA_ARGS__)
#else
#define LOGV(...)
#endif

#define FLASH_STORAGE_PAGE_NO                   \
    (MXC_FLASH_MEM_SIZE / MXC_FLASH_PAGE_SIZE - \
     1) ///< Internal storage flash memory page (the last page)
#define FLASH_STORAGE_START_ADDR \
    MXC_FLASH_PAGE_ADDR(FLASH_STORAGE_PAGE_NO) ///< Internal storage start address

int flash_write(uint32_t startaddr, uint32_t length, uint32_t *data);
int flash_read(uint32_t startaddr, uint32_t length, uint8_t *data);
int check_erased(uint32_t startaddr, uint32_t length);
uint32_t calculate_crc(uint32_t *array, uint32_t length);
#endif // EXAMPLES_MAX78000_FLASH_CLI_DEFINITIONS_H_
