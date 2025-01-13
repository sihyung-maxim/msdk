/**
 * @file    aeskeys_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the AES Keys Peripheral Module.
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

#ifndef _AESKEYS_REGS_H_
#define _AESKEYS_REGS_H_

/* **** Includes **** */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @cond
/*
    If types are not defined elsewhere (CMSIS) define them here
*/
#ifndef __IO
#define __IO volatile
#endif
#ifndef __I
#define __I  volatile const
#endif
#ifndef __O
#define __O  volatile
#endif
#ifndef __R
#define __R  volatile const
#endif
/// @endcond

/* **** Definitions **** */

/**
 * @ingroup     aeskeys
 * @defgroup    aeskeys_registers Registers
 * @brief       Registers, Bit Masks and Bit Positions for the AEs Keys Peripheral Module.
 * @description AEs Keys Registers (For Simulation)
 */

/**
 * @ingroup aeskeys_registers
 * Structure type to access the AES Keys Registers.
 */
typedef struct {
    __IO uint32_t key0_tm;              /**< <tt>\b 0x00:<\tt> AES-256 SRAM Encryption Key (MEU) */
    __IO uint32_t key1_tm;              /**< <tt>\b 0x04:<\tt> AES-256 SRAM Encryption Key (MEU) */
    __IO uint32_t key2_tm;              /**< <tt>\b 0x08:<\tt> AES-256 SRAM Encryption Key (MEU) */
    __IO uint32_t key3_tm;              /**< <tt>\b 0x0C:<\tt> AES-256 SRAM Encryption Key (MEU) */
    __IO uint32_t key4_tm;              /**< <tt>\b 0x10:<\tt> AES-256 SRAM Encryption Key (MEU) */
    __IO uint32_t key5_tm;              /**< <tt>\b 0x14:<\tt> AES-256 SRAM Encryption Key (MEU) */
    __IO uint32_t key6_tm;              /**< <tt>\b 0x18:<\tt> AES-256 SRAM Encryption Key (MEU) */
    __IO uint32_t key7_tm;              /**< <tt>\b 0x1C:<\tt> AES-256 SRAM Encryption Key (MEU) */
    __IO uint32_t key0_qspi_dec;        /**< <tt>\b 0x20:<\tt> AES-128 QSPI Decryption Key (MDIU) */
    __IO uint32_t key1_qspi_dec;        /**< <tt>\b 0x24:<\tt> AES-128 QSPI Decryption Key (MDIU) */
    __IO uint32_t key2_qspi_dec;        /**< <tt>\b 0x28:<\tt> AES-128 QSPI Decryption Key (MDIU) */
    __IO uint32_t key3_qspi_dec;        /**< <tt>\b 0x2C:<\tt> AES-128 QSPI Decryption Key (MDIU) */
    __IO uint32_t key0_qspi_auth;       /**< <tt>\b 0x30:<\tt> AES-128 QSPI Authentication Key (MDIU) */
    __IO uint32_t key1_qspi_auth;       /**< <tt>\b 0x34:<\tt> AES-128 QSPI Authentication Key (MDIU) */
    __IO uint32_t key2_qspi_auth;       /**< <tt>\b 0x38:<\tt> AES-128 QSPI Authentication Key (MDIU) */
    __IO uint32_t key3_qspi_auth;       /**< <tt>\b 0x3C:<\tt> AES-128 QSPI Authentication Key (MDIU) */
} mxc_aeskeys_regs_t;

/*******************************************************************************/
/*                                                                     AESKEYS */
#define MXC_BASE_AESKEYS                  ((uint32_t)0x40005000UL)
#define MXC_AESKEYS                       ((mxc_aeskeys_regs_t*)MXC_BASE_AESKEYS)
#define MXC_AESKEYS_INSTANCES             (1)

#ifdef __cplusplus
}
#endif

#endif /* _AESKEYS_REGS_H_ */

