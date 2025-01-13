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
 * @brief   LittleFS and wear leveling example.
 * @details This example shows the basic functionality of the LittleFS
 *          file system, including it's ability to distibute wear across
 *          the flash memory.
 */

/***** Includes *****/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "cli.h"
#include "flash.h"
#include "file.h"
#include "lfs.h"
#include "main.h"
#include "mxc_device.h"
#include "ecc_regs.h"

/***** Globals *****/
uint32_t start_block = LFS_START_PAGE;

const struct lfs_config cfg = {
    .context = &start_block,
    // block device operations
    .read = flash_read,
    .prog = flash_write,
    .erase = flash_erase,
    .sync = flash_sync,

    // block device configuration
    .read_size = 1,
    .prog_size = 4,
    .block_size = MXC_FLASH_PAGE_SIZE,
    .block_count = LFS_PAGE_CNT,
    .cache_size = 16,
    .lookahead_size = 16,
    .block_cycles = 500,
};

//******************************************************************************
/**
 * @brief Application entry point
 * @return Exit code
 */
int main(void)
{
    lfs_t lfs; // File system instance
    char cmd_buf[CMD_MAX_SIZE];
    int cmd_len, err;

    printf("\n\n********** Wear Leveling Example **********\n");

    printf("Disable ECC.\n");
    MXC_ECC->en &= ~(MXC_F_ECC_EN_FLASH);

    // mount the filesystem
    printf("Mounting the filesystem...\n");
    err = lfs_mount(&lfs, &cfg);

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) {
        printf("Filesystem is invalid, formatting...\n");
        lfs_format(&lfs, &cfg);
        err = lfs_mount(&lfs, &cfg);
    }

    if (!err) {
        printf("Filesystem is mounted! Ready for commands.\n");
    } else {
        printf("Unable to initialize file system!\n");
        return E_BAD_STATE;
    }

    // Continue to receive and process commands until 'stop' command received
    while (err != E_SHUTDOWN) {
        printf("\ncmd> ");
        fflush(stdout);

        cmd_len = cmd_get(cmd_buf, CMD_MAX_SIZE);
        err = cmd_process(&lfs, cmd_buf, cmd_len);
    }

    // release any resources we were using
    lfs_unmount(&lfs);
    printf("\nFilesystem resources released.\n");

    printf("Example complete!\n");
    return E_NO_ERROR;
}
