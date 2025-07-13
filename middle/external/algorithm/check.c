/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  check.c
 *         \unit  check
 *        \brief  This is a C language version of commonly used check code algorithms
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "check.h"

/**
 *  \brief check_sum.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint8_t check_sum(uint8_t* data, uint32_t len)
{
    uint32_t i;
    uint8_t check = 0;
    for (i = 0; i < len; i++)
    {
        check += data[i];
    }
    return check;
}

/**
 *  \brief check_parity.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint8_t check_parity(uint8_t* data, uint32_t len)
{
    uint32_t i, b;
    uint8_t check = 0;
    for (i = 0; i < len; i++)
    {
        for (b = 0; b < 8; b++)
        {
            if ((data[i] >> b) & 1) check++;
        }
    }
    return check % 2;
}

/**
 *  \brief check_lrc.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint8_t check_lrc(uint8_t* data, uint32_t len)
{
    uint32_t i;
    uint8_t check = 0;
    for (i = 0; i < len; i++)
    {
        check += data[i];
    }
    return 0xFF - check + 1;
}

/**
 *  \brief check_xor.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint8_t check_xor(uint8_t* data, uint32_t len)
{
    uint32_t i;
    uint8_t check = 0;
    for (i = 0; i < len; i++)
    {
        check ^= data[i];
    }
    return check;
}
