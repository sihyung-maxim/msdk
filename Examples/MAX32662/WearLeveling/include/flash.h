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
 * @file    flash.h
 * @brief   Flash read/write/erase functions declaration
 */

#ifndef EXAMPLES_MAX32662_WEARLEVELING_INCLUDE_FLASH_H_
#define EXAMPLES_MAX32662_WEARLEVELING_INCLUDE_FLASH_H_

// Flash operations log
//#define FLASH_DEBUG
#ifdef FLASH_DEBUG
#define LOGF(...) printf(__VA_ARGS__)
#else
#define LOGF(...)
#endif

#include <stdint.h>
#include <stdbool.h>

#include "lfs.h"

/**
 * @brief Copies contents of flash into a data buffer.
 *
 * @note LittleFS callback method
 *
 * @param c 		LittleFS config
 * @param block 	Flash memory block number
 * @param off 		Data offset in the block
 * @param buffer 	Buffer to copy flash data into
 * @param size 		Number of bytes to read
 *
 * @return LFS_ERR_OK if successful otherwise and error code.
 */
int flash_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer,
               lfs_size_t size);

/**
 * @brief Writes data to flash memory.
 *
 * @note LittleFS callback method
 *
 * @param c 		LittleFS config
 * @param block 	Flash memory block number
 * @param off		Data offset in the block
 * @param buffer 	Buffer containing data to write to flash.
 * @param size 		Number of bytes to write to flash.
 *
 * @return LFS_ERR_OK if successful otherwise and error code.
 */
int flash_write(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer,
                lfs_size_t size);

/**
 * @brief Erases a flash memory block.
 *
 * @note LittleFS callback method
 *
 * @param c 		LittleFS config
 * @param block 	Number of the flash memory block to erase
 *
 * @return LFS_ERR_OK if successful otherwise and error code.
 */
int flash_erase(const struct lfs_config *c, lfs_block_t block);

/**
 * @brief Performs pending flash operations
 * @note LittleFS callback method. Not supported by Maxim SDK.
 *
 * @param c 	LittleFS config
 *
 * @return LFS_ERR_OK if successful otherwise and error code.
 */
int flash_sync(const struct lfs_config *c);

#endif // EXAMPLES_MAX32662_WEARLEVELING_INCLUDE_FLASH_H_
