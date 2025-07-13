/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  encrypt.h
 *         \unit  encrypt
 *        \brief  This is a C language version of common encryption and decryption algorithms
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
 /*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#ifndef __encrypt_H
#define __encrypt_H

#include <stddef.h>
#include <stdint.h>

#define DES_ENCRYPT     0
#define DES_DECRYPT     1

int des_set_key(const uint8_t key[8]);
int des_crypt_ecb(const uint8_t input[8], uint8_t output[8], uint8_t mode);
int des_crypt_cbc(const uint8_t *input, uint8_t *output, uint32_t length, uint8_t mode);

int des3_set_key2(const uint8_t key[16]);
int des3_set_key3(const uint8_t key[24]);
int des3_crypt_ecb(const uint8_t input[8], uint8_t output[8], uint8_t mode);
int des3_crypt_cbc(const uint8_t *input, uint8_t *output, uint32_t length, uint8_t mode);

#endif 
