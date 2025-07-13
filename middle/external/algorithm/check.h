/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  check.h
 *         \unit  check
 *        \brief  This is a C language version of commonly used check code algorithms
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __check_H
#define __check_H

#include <stdint.h>

/* Version infomation */

#define CHECK_V_MAJOR                       1
#define CHECK_V_MINOR                       0
#define CHECK_V_PATCH                       0

uint8_t check_sum(uint8_t* data, uint32_t len);
uint8_t check_parity(uint8_t* data, uint32_t len);
uint8_t check_lrc(uint8_t* data, uint32_t len);
uint8_t check_xor(uint8_t* data, uint32_t len);

#endif 
