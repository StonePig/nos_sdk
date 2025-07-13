/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  hash.h
 *         \unit  hash
 *        \brief  This is a C language version of commonly used hash algorithms
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __hash_H
#define __hash_H

#include <stdint.h>

/* Hash algorithm declare
 * 
 * All method interface types are consistent, and the external use methods are also consistent.
 *
 * Choose a specific hash algorithm based on specific usage scenarios.
 *
 *  \param[in] data: address of data
 *  \param[in] size: size of data
 *  \return hash value
 */

uint32_t hash_bkdr(void *data, uint32_t size);
uint32_t hash_ap(void *data, uint32_t size);
uint32_t hash_djb(void *data, uint32_t size);
uint32_t hash_js(void *data, uint32_t size);
uint32_t hash_rs(void *data, uint32_t size);
uint32_t hash_sdbm(void *data, uint32_t size);
uint32_t hash_pjw(void *data, uint32_t size);
uint32_t hash_elf(void *data, uint32_t size);
uint32_t hash_dek(void *data, uint32_t size);
uint32_t hash_bp(void *data, uint32_t size);
uint32_t hash_fnv(void *data, uint32_t size);
uint32_t hash_jdk6(void *data, uint32_t size);

#endif
