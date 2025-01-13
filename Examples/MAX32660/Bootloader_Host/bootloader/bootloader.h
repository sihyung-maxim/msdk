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

#ifndef EXAMPLES_MAX32660_BOOTLOADER_HOST_BOOTLOADER_BOOTLOADER_H_
#define EXAMPLES_MAX32660_BOOTLOADER_HOST_BOOTLOADER_BOOTLOADER_H_

/*******************************      INCLUDES    ****************************/
#include "bootloader_cmd.h"

/*******************************      DEFINES     ****************************/
#define GPIO_IDX_BL0 0 // RESET PIN
#define GPIO_IDX_BL1 1 // MFIO  PIN

/******************************* Type Definitions ****************************/
typedef int (*comm_read_t)(unsigned char *dst, unsigned int len);
typedef int (*comm_write_t)(unsigned char *src, unsigned int len);

typedef struct {
    comm_read_t read;
    comm_write_t write;
    void (*gpio_set)(unsigned int idx, int state);
    void (*delay_ms)(unsigned int ms);
    int (*printf)(const char *pcFmt, ...);
} bl_conf_struct_t;

/******************************* Public Functions ****************************/
int bl_init(bl_conf_struct_t *plt_funcs);
int bl_update_interface(comm_read_t read_func, comm_write_t write_func);
//
int bl_hard_reset_then_enter_bl_mode(void);
int bl_enter_bl_mode(void);
int bl_exit_bl_mode(void);
int bl_get_partnumber(char *buf, unsigned int maxLen);
int bl_get_version(char *buf, unsigned int maxLen);
int bl_get_usn(char *buf, unsigned int maxLen);
int bl_get_page_size(unsigned int *page_size);
int bl_erase_app(void);
int bl_set_num_pages(int page_num);
int bl_set_iv(unsigned char *iv);
int bl_set_auth(unsigned char *auth);
int bl_write_page(const char *page, unsigned int page_len);
int bl_flash_image(const char *image);

// Bootloader configuration section
int bl_get_target_configure(const char *target_bl_version, void *bl_cfg_struct);
int bl_update_cfg_enter_bl_check(int enable);
int bl_update_cfg_set_ebl_pin(int port, int pin);
int bl_update_cfg_set_ebl_pin_polarity(int polarity);
int bl_update_cfg_set_valid_mark_check(int enable);
int bl_update_cfg_enable_uart(int enable);
int bl_update_cfg_enable_i2c(int enable);
int bl_update_cfg_enable_spi(int enable);
int bl_update_cfg_set_i2c_addr(const char *target_bl_version, unsigned char addr);
int bl_update_cfg_set_crc_check(int enable);
int bl_update_cfg_lock_swd(int enable);
int bl_update_cfg_set_bl_exit_mode(BLExitMode_t mode);
int b_update_cfg_set_bl_exit_timeout(uint16_t timeout);
int bl_flash_bl_cfg(void);

#endif // EXAMPLES_MAX32660_BOOTLOADER_HOST_BOOTLOADER_BOOTLOADER_H_
