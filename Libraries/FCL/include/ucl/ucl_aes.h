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

#ifndef LIBRARIES_FCL_INCLUDE_UCL_UCL_AES_H_
#define LIBRARIES_FCL_INCLUDE_UCL_UCL_AES_H_

#include "ucl_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

  /** @file ucl_aes.h
   * @defgroup UCL_AES AES
   * The AES, see FIPS-197.
   *
   * @par Header:
   * @link ucl_aes.h ucl_aes.h @endlink
   *
   * <b>AES Descriptor:</b>
   * @li Length of Input/Output Block: 128 bits
   * @li Length of Key: 128, 192 or 256 bits
   *
   * For messages longer than 128 bits use an Operation mode.@n
   * @n
   *
   * @see UCL_OPERATION_MODES
   * @ingroup UCL_BLOCK_CIPHER */

  /** <b>AES Block Size</b>.
   * The byte length of the DES core data block.
   * @ingroup UCL_AES */
#define UCL_AES_BLOCKSIZE 16

  /** <b>128-bits AES Key byte length</b>.
   * @ingroup UCL_AES */
#define UCL_AES_KEYLEN_128 16
  /** <b>192-bits AES Key byte length</b>.
   * @ingroup UCL_AES */
#define UCL_AES_KEYLEN_192 24
  /** <b>256-bits AES Key byte length</b>.
   * @ingroup UCL_AES */
#define UCL_AES_KEYLEN_256 32

  /** <b>AES maximum round number</b>.
   * @ingroup UCL_AES */
#define UCL_AES_MAXNR   14

  /** <b>AES Key</b>.
   * @ingroup UCL_AES */
  struct ucl_aes_key
  {
    u32 rd_key[4 *(UCL_AES_MAXNR + 1)]; /**< Round key.        */
    int rounds;                         /**< Number of rounds. */
  };

  /** <b>AES Key</b>.
   * @ingroup UCL_AES */
  typedef struct ucl_aes_key ucl_aes_key_t;

  typedef struct ucl_aes_key AES_KEY;

  /** <b>AES Context</b>.
   * This structure is involved in the operation modes.
   * @see UCL_OPERATION_MODES
   *
   * @ingroup UCL_AES */
  struct ucl_aes_ctx
  {
    int mode;                     /**< Ciphering Mode.    */
    u8 memory[UCL_AES_BLOCKSIZE]; /**< Intermediate state.*/
    u32 index;                    /**< index.             */
    u8 origin_key[UCL_AES_KEYLEN_256];/**<origin key, w/o modification*/
    int origin_keylen;
    ucl_aes_key_t key;            /**< AES Sub-keys.      */
  };

  /** <b>AES Context</b>.
   * @ingroup UCL_AES */
  typedef struct ucl_aes_ctx ucl_aes_ctx_t;

  /** <b>AES for Single Block</b>.
   * The complete AES for only one block.
   *
   * @param[out] dst    Output Block (encrypted/decrypted)
   * @param[in]  src    Input Block to encrypt/decrypt
   * @param[in]  key    An AES key
   * @param[in]  keylen AES key byte length
   * @param[in]  mode   The AES mode (Encryption/Decryption)
   *
   * @return Error code
   *
   * @retval #UCL_OK             No error occurred
   * @retval #UCL_INVALID_INPUT  One of the input is the pointer #NULL
   * @retval #UCL_INVALID_OUTPUT One of the output is the pointer #NULL
   * @retval #UCL_INVALID_MODE   The mode is not one of those described
   *
   * @note For more than one block use an @link UCL_OPERATION_MODES
   *       Operation Modes @endlink.
   *
   * @ingroup UCL_AES
   */
  int ucl_aes(u8 *dst, u8 *src, u8 *key, u32 keylen, int mode);

  int aes_set_ekey(AES_KEY *key, const u8 *userKey, const int len);
  int aes_set_dkey(AES_KEY *key, const u8 *userKey, const int len);
  int aes_encrypt(u8 *out, const u8 *in, const AES_KEY *key);
  int aes_decrypt(u8 *out, const u8 *in, const AES_KEY *key);
#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif // LIBRARIES_FCL_INCLUDE_UCL_UCL_AES_H_
