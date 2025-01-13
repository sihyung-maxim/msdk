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

#ifndef EXAMPLES_MAX32680_EEPROM_EMULATOR_INCLUDE_EEPROM_H_
#define EXAMPLES_MAX32680_EEPROM_EMULATOR_INCLUDE_EEPROM_H_

/***** Included Files *****/
#include <stdbool.h>
#include "board.h"
#include "gpio.h"
#include "i2c.h"
#include "mxc_device.h"

/***** Definitions *****/
#define EEPROM_ADDR 0x24
#define EEPROM_I2C_IRQN(i2c) MXC_I2C_GET_IRQ(MXC_I2C_GET_IDX(i2c))
#define EEPROM_I2C_FREQ 100000
#define EEPROM_FIFO_DEPTH MXC_I2C_FIFO_DEPTH

#define EEPROM_WRITE_BACK_CMD 0xBEEF

#define EEPROM_NUM_FLASH_PG 4
#define EEPROM_FLASH_SZ (EEPROM_NUM_FLASH_PG * MXC_FLASH_PAGE_SIZE)
#define EEPROM_END_ADDR (MXC_FLASH_MEM_BASE + MXC_FLASH_MEM_SIZE)
#define EEPROM_BASE_ADDR (EEPROM_END_ADDR - EEPROM_FLASH_SZ)
#define EEPROM_RAW_ADDR(read_addr) (EEPROM_BASE_ADDR + read_addr)

#define EEPROM_MAX_DATA_RX 64
#define EEPROM_ADDR_SIZE sizeof(uint16_t)
#define EEPROM_RX_BUF_SIZE (EEPROM_MAX_DATA_RX + EEPROM_ADDR_SIZE)

/***** Global Variables *****/
extern volatile bool eeprom_txn_done;

/***** Functions *****/
/*
 * @brief Initializes the components of the EEPROM emulator
 *
 * @param eeprom_i2c 	Pointer to the I2C instance for EEPROM to communicate with
 * @param rdy_pin    	Struct containing information about the GPIO pin used as
 * 						the ready signal. (Only need port and pin to be initialized
 * 						before calling this function.)
 *
 * @return Success/fail. See \ref MXC_Error_Codes for list of error codes.
 */
int eeprom_init(mxc_i2c_regs_t *eeprom_i2c, mxc_gpio_cfg_t rdy_pin);

/*
 * @brief Prepares the EEPROM for next transaction with the master device.
 */
void eeprom_prep_for_txn(void);

#endif // EXAMPLES_MAX32680_EEPROM_EMULATOR_INCLUDE_EEPROM_H_
