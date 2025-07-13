/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  crc.h
 *         \unit  crc
 *        \brief  This is a C language version of commonly used crc algorithms
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __crc_H
#define __crc_H

#include <stdint.h>

/* Version infomation */

#define CRC_V_MAJOR                         1
#define CRC_V_MINOR                         0
#define CRC_V_PATCH                         0

/* CRC algorithm customization options define */
typedef struct 
{
    /**< The width of data validation can range from 1 to 32 */
    uint8_t width;

    /**< Flag for reversing input data */
    uint8_t refin; 

    /**< Flag for reversing output data */
    uint8_t refout; 

    /**< Calculating polynomials, the effective width must be consistent with the width, and any excess parts will be invalid */
    uint32_t poly;

    /**< Calculated initial value */
    uint32_t init; 

    /**< Result XOR output value */
    uint32_t xorout;
} crcOptType;

/*
|-------------------------------------------------------------------------------------------------------|
| index     |CRC name           | width     | refin     | refout    | poly      | init      | xorout    |
|-------------------------------------------------------------------------------------------------------|
| 0         |CRC-4/ITU          | 4         | true      | true      | 03        | 00        | 00        |
| 1         |CRC-5/EPC          | 5         | false     | false     | 9         | 09        | 00        |
| 2         |CRC-5/ITU          | 5         | true      | true      | 15        | 00        | 00        |
| 3         |CRC-5/USB          | 5         | true      | true      | 5         | 1F        | 1F        |
| 4         |CRC-6/ITU          | 6         | true      | true      | 3         | 00        | 00        |
| 5         |CRC-7/MMC          | 7         | false     | false     | 9         | 00        | 00        |
| 6         |CRC-8              | 8         | false     | false     | 7         | 00        | 00        |
| 7         |CRC-8/ITU          | 8         | false     | false     | 7         | 00        | 55        |
| 8         |CRC-8/ROHC         | 8         | true      | true      | 7         | FF        | 00        |
| 9         |CRC-8/MAXIM        | 8         | true      | true      | 1         | 00        | 00        |
| 10        |CRC-16/IBM         | 16        | true      | true      | 005       | 0000      | 0000      |
| 11        |CRC-16/MAXIM       | 16        | true      | true      | 005       | 0000      | FFFF      |
| 12        |CRC-16/USB         | 16        | true      | true      | 005       | FFFF      | FFFF      |
| 13        |CRC-16/MODBUS      | 16        | true      | true      | 005       | FFFF      | 0000      |
| 14        |CRC-16/CCITT       | 16        | true      | true      | 021       | 0000      | 0000      |
| 15        |CRC-16/CCITT-FALSE | 16        | false     | false     | 021       | FFFF      | 0000      |
| 16        |CRC-16/X25         | 16        | true      | true      | 021       | FFFF      | FFFF      |
| 17        |CRC-16/XMODEM      | 16        | false     | false     | 021       | 0000      | 0000      |
| 18        |CRC-16/DNP         | 16        | true      | true      | D65       | 0000      | FFFF      |
| 19        |CRC-32             | 32        | true      | true      | 4C11DB7   | FFFFFFFF  | FFFFFFFF  |
| 20        |CRC-32/MPEG-2      | 32        | false     | false     | 4C11DB7   | FFFFFFFF  | 00000000  |
|-------------------------------------------------------------------------------------------------------|
 */
extern crcOptType crcParaModelTable[21];

/* Custom universal model, within 32 bits, any customization is possible */

uint32_t crc(uint8_t* data, uint32_t len, const crcOptType * const opt);

/* Standard reference model */

uint8_t crc4_itu(uint8_t* data, uint32_t len);
uint8_t crc5_epc(uint8_t* data, uint32_t len);
uint8_t crc5_itu(uint8_t* data, uint32_t len);
uint8_t crc5_usb(uint8_t* data, uint32_t len);
uint8_t crc6_itu(uint8_t* data, uint32_t len);
uint8_t crc7_mmc(uint8_t* data, uint32_t len);
uint8_t crc8(uint8_t* data, uint32_t len);
uint8_t crc8_itu(uint8_t* data, uint32_t len);
uint8_t crc8_rohc(uint8_t* data, uint32_t len);
uint8_t crc8_maxim(uint8_t* data, uint32_t len);
uint16_t crc16_ibm(uint8_t* data, uint32_t len);
uint16_t crc16_maxim(uint8_t* data, uint32_t len);
uint16_t crc16_usb(uint8_t* data, uint32_t len);
uint16_t crc16_modbus(uint8_t* data, uint32_t len);
uint16_t crc16_ccitt(uint8_t* data, uint32_t len);
uint16_t crc16_ccitt_false(uint8_t* data, uint32_t len);
uint16_t crc16_x25(uint8_t* data, uint32_t len);
uint16_t crc16_xmodem(uint8_t* data, uint32_t len);
uint16_t crc16_dnp(uint8_t* data, uint32_t len);
uint32_t crc32(uint8_t* data, uint32_t len);
uint32_t crc32_mpeg_2(uint8_t* data, uint32_t len);

#endif 
