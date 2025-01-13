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
 * @file        main.c
 * @brief       AES Example
 * @details     Encryption and decryption of AES on different modes (ECB and OFB) with different bit sizes (128, 192, and 256)
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_device.h"
#include "ctb.h"
#include "trng.h"

/***** Definitions *****/

#define MXC_AES_DATA_LENGTH 32 //4 words
#define MXC_AES_128_KEY_LEN (128 / 8)
#define MXC_AES_192_KEY_LEN (192 / 8)
#define MXC_AES_256_KEY_LEN (256 / 8)

/***** Globals *****/
uint8_t inputData[MXC_AES_DATA_LENGTH] = { 0x87, 0x3A, 0xC1, 0x25, 0x2F, 0x45, 0xA7, 0xC8,
                                           0x3E, 0xcB, 0x71, 0x90, 0x48, 0x6F, 0xA9, 0x31,
                                           0x94, 0xAE, 0x56, 0xF2, 0x89, 0xB4, 0xD0, 0xC1,
                                           0x2F, 0x45, 0xA7, 0xC8, 0x03, 0xEB, 0x71, 0x90 };
volatile uint8_t encryptedData[MXC_AES_DATA_LENGTH] = { 0 };
volatile uint8_t decryptedData[MXC_AES_DATA_LENGTH] = { 0 };

/***** Functions *****/
int AES_Test(mxc_ctb_cipher_t aesKey)
{
    uint8_t key[MXC_AES_256_KEY_LEN] = { 0 };
    uint32_t stat;

    // Set encryption/decryption key
    MXC_CTB_Cipher_SetKeySource(MXC_CTB_CIPHER_KEY_SOFTWARE);
    switch (aesKey) {
    case MXC_CTB_CIPHER_AES128:
        MXC_TRNG_Random(key, MXC_AES_128_KEY_LEN);
        MXC_CTB_Cipher_SetKey(key, MXC_AES_128_KEY_LEN);
        break;
    case MXC_CTB_CIPHER_AES192:
        MXC_TRNG_Random(key, MXC_AES_192_KEY_LEN);
        MXC_CTB_Cipher_SetKey(key, MXC_AES_192_KEY_LEN);
        break;
    case MXC_CTB_CIPHER_AES256:
        MXC_TRNG_Random(key, MXC_AES_256_KEY_LEN);
        MXC_CTB_Cipher_SetKey(key, MXC_AES_256_KEY_LEN);
        break;
    default:
        return E_BAD_PARAM;
    }

    // Set to appropriate cipher modes
    MXC_CTB_Cipher_SetMode(MXC_CTB_MODE_ECB);
    MXC_CTB_Cipher_SetCipher(aesKey);

    // Prepare and execute encryption
    mxc_ctb_cipher_req_t aesReq;
    aesReq.plaintext = inputData;
    aesReq.ptLen = MXC_AES_DATA_LENGTH;
    aesReq.iv = NULL;
    aesReq.ciphertext = (uint8_t *)encryptedData;
    MXC_CTB_Cipher_Encrypt(&aesReq);
    do {
        stat = MXC_CTB->ctrl;
    } while (!(stat & MXC_F_CTB_CTRL_CPH_DONE) && !(stat & MXC_F_CTB_CTRL_DMA_DONE) &&
             !(stat & MXC_F_CTB_CTRL_DONE));

    // Prepare and execute decryption
    aesReq.plaintext = (uint8_t *)encryptedData;
    aesReq.ciphertext = (uint8_t *)decryptedData;
    MXC_CTB_Cipher_Decrypt(&aesReq);
    do {
        stat = MXC_CTB->ctrl;
    } while (!(stat & MXC_F_CTB_CTRL_CPH_DONE) && !(stat & MXC_F_CTB_CTRL_DMA_DONE) &&
             !(stat & MXC_F_CTB_CTRL_DONE));

    // Compare result of encryption-decryption cycle to original data
    if (memcmp(inputData, (void *)decryptedData, MXC_AES_DATA_LENGTH) == 0) {
        return E_NO_ERROR;
    }
    return E_INVALID;
}

// *****************************************************************************
int main(void)
{
    printf("\n***** AES Example *****\n");
    int fail = 0;
    int ret = 0;
    // Initialize CTB and TRNG
    MXC_CTB_Init(MXC_CTB_FEATURE_CIPHER | MXC_CTB_FEATURE_DMA);
    MXC_TRNG_Init();

    // Run AES Test with 128-Bit encryption key
    printf("AES 128-bit Key Test");
    ret = AES_Test(MXC_CTB_CIPHER_AES128);
    fail += ret;
    printf(" --> %s\n", ((ret == E_NO_ERROR) ? "PASS" : "FAIL"));

    // Run AES Test with 192-Bit encryption key
    printf("AES 192-bit Key Test");
    ret = AES_Test(MXC_CTB_CIPHER_AES192);
    fail += ret;
    printf(" --> %s\n", ((ret == E_NO_ERROR) ? "PASS" : "FAIL"));

    // Run AES Test with 256-Bit encryption key
    printf("AES 256-bit Key Test");
    ret = AES_Test(MXC_CTB_CIPHER_AES256);
    fail += ret;
    printf(" --> %s\n", ((ret == E_NO_ERROR) ? "PASS" : "FAIL"));

    if (fail != 0) {
        printf("\nExample Failed\n");
        return E_FAIL;
    }

    printf("\nExample Succeeded\n");
    return E_NO_ERROR;
}
