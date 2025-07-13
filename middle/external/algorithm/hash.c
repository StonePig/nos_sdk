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
#include "hash.h"

/**
 *  \brief hash_bkdr.
 *  \param[in] data: address of data
 *  \param[in] size: size of data
 *  \return hash value
 */
uint32_t hash_bkdr(void *data, uint32_t size)
{
    int i = 0;
    uint8_t *str = (uint8_t *)data;
    uint32_t seed = 131; // 31 131 1313 13131 131313 etc..
    uint32_t hash = 0;
     
    for (i = 0; i < size; i++)
    {
        hash = hash * seed + (*str++);
    }
     
    return (hash & 0x7FFFFFFF);
}

/**
 *  \brief hash_ap.
 *  \param[in] data: address of data
 *  \param[in] size: size of data
 *  \return hash value
 */
uint32_t hash_ap(void *data, uint32_t size)
{
    int i = 0;
    uint8_t *str = (uint8_t *)data;
    uint32_t hash = 0;
 
    for (i = 0; i < size; i++)
    {
        if ((i & 1) == 0)
        {
            hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
        }
        else
        {
            hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
        }
    }
 
    return (hash & 0x7FFFFFFF);
}

/**
 *  \brief hash_djb.
 *  \param[in] data: address of data
 *  \param[in] size: size of data
 *  \return hash value
 */
uint32_t hash_djb(void *data, uint32_t size)
{
    int i = 0;
    uint8_t *str = (uint8_t *)data;
    uint32_t hash = 5381;
 
    for (i = 0; i < size; i++)
    {
        hash += (hash << 5) + (*str++);
    }
 
    return (hash & 0x7FFFFFFF);
}

/**
 *  \brief hash_js.
 *  \param[in] data: address of data
 *  \param[in] size: size of data
 *  \return hash value
 */
uint32_t hash_js(void *data, uint32_t size)
{
    int i = 0;
    uint8_t *str = (uint8_t *)data;
    uint32_t hash = 1315423911;
 
    for (i = 0; i < size; i++)
    {
        hash ^= ((hash << 5) + (*str++) + (hash >> 2));
    }
 
    return (hash & 0x7FFFFFFF);
}

/**
 *  \brief hash_rs.
 *  \param[in] data: address of data
 *  \param[in] size: size of data
 *  \return hash value
 */
uint32_t hash_rs(void *data, uint32_t size)
{
    int i = 0;
    uint8_t *str = (uint8_t *)data;
    uint32_t b = 378551;
    uint32_t a = 63689;
    uint32_t hash = 0;
 
    for (i = 0; i < size; i++)
    {
        hash = hash * a + (*str++);
        a *= b;
    }
 
    return (hash & 0x7FFFFFFF);
}

/**
 *  \brief hash_sdbm.
 *  \param[in] data: address of data
 *  \param[in] size: size of data
 *  \return hash value
 */
uint32_t hash_sdbm(void *data, uint32_t size)
{
    int i = 0;
    uint8_t *str = (uint8_t *)data;
    uint32_t hash = 0;
 
    for (i = 0; i < size; i++)
    {
        // equivalent to: hash = 65599*hash + (*str++);
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }
 
    return (hash & 0x7FFFFFFF);
}

/**
 *  \brief hash_pjw.
 *  \param[in] data: address of data
 *  \param[in] size: size of data
 *  \return hash value
 */
uint32_t hash_pjw(void *data, uint32_t size)
{
    int i = 0;
    uint8_t *str = (uint8_t *)data;
    uint32_t bits_in_unigned_int = (uint32_t)(sizeof(uint32_t) * 8);
    uint32_t three_quarters      = (uint32_t)((bits_in_unigned_int  * 3) / 4);
    uint32_t one_eighth          = (uint32_t)(bits_in_unigned_int / 8);
    uint32_t high_bits           = (uint32_t)(0xFFFFFFFF) << (bits_in_unigned_int - one_eighth);
    uint32_t hash                = 0;
    uint32_t test                = 0;
 
    for (i = 0; i < size; i++)
    {
        hash = (hash << one_eighth) + (*str++);
        if ((test = hash & high_bits) != 0)
        {
            hash = ((hash ^ (test >> three_quarters)) & (~high_bits));
        }
    }
 
    return (hash & 0x7FFFFFFF);
}

/**
 *  \brief hash_elf.
 *  \param[in] data: address of data
 *  \param[in] size: size of data
 *  \return hash value
 */
uint32_t hash_elf(void *data, uint32_t size)
{
    int i = 0;
    uint8_t *str = (uint8_t *)data;
    uint32_t hash = 0;
    uint32_t x    = 0;
 
    for (i = 0; i < size; i++)
    {
        hash = (hash << 4) + (*str++);
        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }
 
    return (hash & 0x7FFFFFFF);
}

/**
 *  \brief hash_dek.
 *  \param[in] data: address of data
 *  \param[in] size: size of data
 *  \return hash value
 */
uint32_t hash_dek(void *data, uint32_t size)
{
    int i = 0;
    uint8_t *str = (uint8_t *)data;
    uint32_t hash = size;

    for (i = 0; i < size; i++)
    {
        hash = (hash << 5) ^ (hash >> 27) ^ (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

/**
 *  \brief hash_bp.
 *  \param[in] data: address of data
 *  \param[in] size: size of data
 *  \return hash value
 */
uint32_t hash_bp(void *data, uint32_t size)
{
    int i = 0;
    uint8_t *str = (uint8_t *)data;
    uint32_t hash = size;

    for (i = 0; i < size; i++)
    {
        hash = (hash << 7) ^ (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

/**
 *  \brief hash_fnv.
 *  \param[in] data: address of data
 *  \param[in] size: size of data
 *  \return hash value
 */
uint32_t hash_fnv(void *data, uint32_t size)
{
    int i = 0;
    uint8_t *str = (uint8_t *)data;
    uint32_t fnv_prime = 0x811C9DC5;
    uint32_t hash = 0;

    for (i = 0; i < size; i++)
    {
        hash *= fnv_prime;
        hash ^= (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

/**
 *  \brief hash_jdk6.
 *  \param[in] data: address of data
 *  \param[in] size: size of data
 *  \return hash value
 */
uint32_t hash_jdk6(void *data, uint32_t size)
{
    int i = 0;
    uint8_t *str = (uint8_t *)data;
    uint32_t hash = 0;

    for (i = 0; i < size; i++)
    {
        hash = hash * 31 + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

