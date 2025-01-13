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

#ifndef EXAMPLES_MAX32670_SECUREROM_BL_HOST_BOOTLOADER_BOOTLOADER_H_
#define EXAMPLES_MAX32670_SECUREROM_BL_HOST_BOOTLOADER_BOOTLOADER_H_

/*******************************      INCLUDES    ****************************/
#include <stdint.h>

/*******************************      DEFINES     ****************************/
// RESET pin used to restart target before SCP communication
#define GPIO_IDX_RSTN 0
// Stimulus pin to enable SCP communication
#define GPIO_IDX_STIMULUS_PIN 1

#define STIMULUS_PIN_ACTIVE_STATE 0 // 0 or 1

// Packet Type
#define PT_HELLO_REQ 1
#define PT_HELLO_RSP 2
#define PT_DELETE_MEM 3
#define PT_DELETE_MEM_RSP 4
#define PT_WRITE_MEM 5
#define PT_WRITE_MEM_RSP 6

// MAX wait time for packets
#define PACKET_WAIT_DEL_MEM_RSP 5000 // ms
#define PACKET_WAIT_WRITE_MEM_RSP 5000 // ms

/******************************* Type Definitions ****************************/
typedef int (*comm_read_t)(unsigned char *dst, unsigned int len, unsigned int to);
typedef int (*comm_write_t)(const unsigned char *src, unsigned int len, unsigned int to);

typedef struct {
    comm_read_t read;
    comm_write_t write;
    void (*gpio_set)(unsigned int idx, int state);
    void (*delay_ms)(unsigned int ms);
    int (*printf)(const char *pcFmt, ...);
} bl_conf_struct_t;

typedef struct {
    unsigned char type; // 1:hello_reply, 2:erase/del_mem
    unsigned char is_tx; // 1: From host to target,0: From target to host
    uint16_t len;
    const unsigned char *data;
} scp_packet_struct;

/******************************* Public Functions ****************************/
int sbl_init(bl_conf_struct_t *plt_funcs);
int sbl_load(scp_packet_struct *scp_packets);

#endif // EXAMPLES_MAX32670_SECUREROM_BL_HOST_BOOTLOADER_BOOTLOADER_H_
