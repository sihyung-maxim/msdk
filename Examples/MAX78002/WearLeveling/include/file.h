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

#ifndef EXAMPLES_MAX78002_WEARLEVELING_INCLUDE_FILE_H_
#define EXAMPLES_MAX78002_WEARLEVELING_INCLUDE_FILE_H_

#include <stdbool.h>
#include <stdint.h>
#include "lfs.h"

/***** Macros *****/
#define MAX_FILE_READ_SIZE 1024

/*
 * @brief Write data to a file.
 *
 * @param filesys 		Pointer to the LittleFS file system instance.
 * @param file 			Pointer to a LittleFS file instance.
 * @param filename 	 	Name of the file to write to.
 * @param write_buf 	Buffer containing the data to write to the file
 * @param len			Number of bytes to write to the file.
 * @param pos			Position within the file to start writing data at.
 * @param create 		Determines behavior if file doesn't already exist, "true" will create the
 * 						file and complete the write, and "false" will return an error.
 *
 * @return The number of bytes written to flash if successful, otherwise an error code.
 */
int file_write(lfs_t *filesys, lfs_file_t *file, const char *filename, char *write_buf,
               uint32_t len, uint32_t pos, bool create);

/*
 * @brief Read data from a file.
 *
 * @param filesys 		Pointer to the LittleFS file system instance.
 * @param file 			Pointer to a LittleFS file instance.
 * @param filename 		Name of the file to read from.
 * @param read_buf		Buffer to store data from the file.
 * @param len 			Number of bytes to read from the file.
 * @param pos			Position within the file to start reading data from.
 *
 * @return The number of bytes read if successful, otherwise an error code.
 */
int file_read(lfs_t *filesys, lfs_file_t *file, const char *filename, char *read_buf, uint32_t len,
              uint32_t pos);

#endif // EXAMPLES_MAX78002_WEARLEVELING_INCLUDE_FILE_H_
