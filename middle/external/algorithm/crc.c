/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  crc.c
 *         \unit  crc
 *        \brief  This is a C language version of commonly used crc algorithms
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "crc.h"

static uint8_t rf8(uint8_t data)
{
    uint8_t xor = 0;
    uint8_t i = 4;
    while (i--)
    {
        xor |= ((data >> ((i << 1) + 1)) ^ data) & (1 << (3 - i));
        xor |= ((xor << ((i << 1) + 1)) & (1 << (4 + i)));
    }
    return xor ^ data;
}

static uint16_t rf16(uint16_t data)
{
    uint16_t xor = 0;
    uint16_t i = 8;
    while (i--)
    {
        xor |= ((data >> ((i << 1) + 1)) ^ data) & (1 << (7 - i));
        xor |= ((xor << ((i << 1) + 1)) & (1 << (8 + i)));
    }
    return xor ^ data;
}

static uint32_t rf32(uint32_t data)
{
    uint32_t xor = 0;
    uint32_t i = 16;
    while (i--)
    {
        xor |= ((data >> ((i << 1) + 1)) ^ data) & (1 << (15 - i));
        xor |= ((xor << ((i << 1) + 1)) & (1 << (16 + i)));
    }
    return xor ^ data;
}

crcOptType crcParaModelTable[21] = {
/*      width   refin   refout  poly            init            xorout              name                 */
/*-------------------------------------------------------------------------------------------------------*/
    {   4,      1,      1,      0x03,           0x00,           0x00        }, /*|  CRC-4/ITU           |*/
    {   5,      0,      0,      0x09,           0x09,           0x00        }, /*|  CRC-5/EPC           |*/
    {   5,      1,      1,      0x15,           0x00,           0x00        }, /*|  CRC-5/ITU           |*/
    {   5,      1,      1,      0x05,           0x1F,           0x1F        }, /*|  CRC-5/USB           |*/
    {   6,      1,      1,      0x03,           0x00,           0x00        }, /*|  CRC-6/ITU           |*/
    {   7,      0,      0,      0x09,           0x00,           0x00        }, /*|  CRC-7/MMC           |*/
    {   8,      0,      0,      0x07,           0x00,           0x00        }, /*|  CRC-8               |*/
    {   8,      0,      0,      0x07,           0x00,           0x55        }, /*|  CRC-8/ITU           |*/
    {   8,      1,      1,      0x07,           0xFF,           0x00        }, /*|  CRC-8/ROHC          |*/
    {   8,      1,      1,      0x31,           0x00,           0x00        }, /*|  CRC-8/MAXIM         |*/
    {   16,     1,      1,      0x8005,         0x0000,         0x0000      }, /*|  CRC-16/IBM          |*/
    {   16,     1,      1,      0x8005,         0x0000,         0xFFFF      }, /*|  CRC-16/MAXIM        |*/
    {   16,     1,      1,      0x8005,         0xFFFF,         0xFFFF      }, /*|  CRC-16/USB          |*/
    {   16,     1,      1,      0x8005,         0xFFFF,         0x0000      }, /*|  CRC-16/MODBUS       |*/
    {   16,     1,      1,      0x1021,         0x0000,         0x0000      }, /*|  CRC-16/CCITT        |*/
    {   16,     0,      0,      0x1021,         0xFFFF,         0x0000      }, /*|  CRC-16/CCITT-FALSE  |*/
    {   16,     1,      1,      0x1021,         0xFFFF,         0xFFFF      }, /*|  CRC-16/X25          |*/
    {   16,     0,      0,      0x1021,         0x0000,         0x0000      }, /*|  CRC-16/XMODEM       |*/
    {   16,     1,      1,      0x3D65,         0x0000,         0xFFFF      }, /*|  CRC-16/DNP          |*/
    {   32,     1,      1,      0x04C11DB7,     0xFFFFFFFF,     0xFFFFFFFF  }, /*|  CRC-32              |*/
    {   32,     0,      0,      0x04C11DB7,     0xFFFFFFFF,     0x00000000  }, /*|CRC-32/MPEG-2         |*/
};

/**
 *  \brief the general crc algorithm
 *  \param[in] data: data address 
 *  \param[in] len: length of data
 *  \param[in] opt: crc algorithm customization options
 *  \return crc code
 */
uint32_t crc(uint8_t* data, uint32_t len, const crcOptType * const opt)
{
    uint8_t i;
    uint32_t crc;
    uint32_t poly;

    if (!data) return 0;
    if (!opt) return 0;
    if (opt->width == 0 || opt->width > 32) return 0;

    crc = opt->init;
    poly = opt->poly;

    if (opt->refin)
    {
        poly = rf32(poly) >> (32 - opt->width);
        crc = rf32(crc) >> (32 - opt->width);
        while (len--)
        {
            crc ^= *data++;
            for (i = 0; i < 8; i++)
            {
                if (crc & 0x1) crc = (crc >> 1) ^ poly;
                else crc >>= 1;
            }
        }
        if (!opt->refout)
        {
            crc = rf32(crc);
            crc >>= (32 - opt->width);
        }
    }
    else  
    {
        crc <<= (32 - opt->width);
        poly <<= (32 - opt->width);
        while (len--)
        {
            crc ^= (uint32_t)(*data++) << 24;
            for (i = 0; i < 8; i++)
            {
                if (crc & 0x80000000) crc = (crc << 1) ^ poly;
                else crc <<= 1;
            }
        }
        if (opt->refout) crc = rf32(crc);
        else crc >>= (32 - opt->width);
    }
    crc ^= opt->xorout;
    return crc;
}

/**
 *  \brief crc4_itu.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint8_t crc4_itu(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint8_t crc = 0;
    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 1) crc = (crc >> 1) ^ 0x0C; /* 0x0C = (reflect 0x03) >> (8 - 4) */
            else crc = (crc >> 1);
        }
    }
    return crc;
}

/**
 *  \brief crc5_epc.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint8_t crc5_epc(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint8_t crc = 0x48; /* 0x48 = 0x09 << (8 - 5) */
    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x80) crc = (crc << 1) ^ 0x48; /* 0x48 = 0x09 << (8 - 5) */
            else crc <<= 1;
        }
    }
    return crc >> 3;
}

/**
 *  \brief crc5_itu.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint8_t crc5_itu(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint8_t crc = 0;
    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 1) crc = (crc >> 1) ^ 0x15; /* 0x15 = (reflect 0x15) >> (8 - 5) */
            else crc = (crc >> 1);
        }
    }
    return crc;
}

/**
 *  \brief crc5_usb.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint8_t crc5_usb(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint8_t crc = 0x1F;
    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 1) crc = (crc >> 1) ^ 0x14; /* 0x14 = (reflect 0x05) >> (8 - 5) */
            else crc = (crc >> 1);
        }
    }
    return crc ^ 0x1F;
}

/**
 *  \brief crc6_itu.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint8_t crc6_itu(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint8_t crc = 0;
    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 1) crc = (crc >> 1) ^ 0x30;/* 0x30 = (reflect 0x03) >> (8 - 6) */
            else crc = (crc >> 1);
        }
    }
    return crc;
}

/**
 *  \brief crc7_mmc.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint8_t crc7_mmc(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint8_t crc = 0;
    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x80) crc = (crc << 1) ^ 0x12; /* 0x12 = 0x09 << (8 - 7) */
            else crc <<= 1;
        }
    }
    return crc >> 1;
}

/**
 *  \brief crc8.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint8_t crc8(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint8_t crc = 0;
    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x80) crc = (crc << 1) ^ 0x07;
            else crc <<= 1;
        }
    }
    return crc;
}

/**
 *  \brief crc8_itu.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint8_t crc8_itu(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint8_t crc = 0;
    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x80) crc = (crc << 1) ^ 0x07;
            else crc <<= 1;
        }
    }
    return crc ^ 0x55;
}

/**
 *  \brief crc8_rohc.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint8_t crc8_rohc(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint8_t crc = 0xFF; 
    while (len--)
    {
        crc ^= *data++;    
        for (i = 0; i < 8; i++)
        {
            if (crc & 1)  crc = (crc >> 1) ^ 0xE0; /* 0xE0 = reflect 0x07 */
            else crc = (crc >> 1);
        }
    }
    return crc;
}

/**
 *  \brief crc8_maxim.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint8_t crc8_maxim(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint8_t crc = 0; 
    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 1) crc = (crc >> 1) ^ 0x8C; /* 0x8C = reflect 0x31 */
            else crc >>= 1;
        }
    }
    return crc;
}

/**
 *  \brief crc16_ibm.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint16_t crc16_ibm(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint16_t crc = 0;
    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 1) crc = (crc >> 1) ^ 0xA001; /* 0xA001 = reflect 0x8005 */
            else crc = (crc >> 1);
        }
    }
    return crc;
}

/**
 *  \brief crc16_maxim.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint16_t crc16_maxim(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint16_t crc = 0;
    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 1) crc = (crc >> 1) ^ 0xA001; /* 0xA001 = reflect 0x8005 */
            else crc = (crc >> 1);
        }
    }
    return crc ^ 0xFFFF;
}

/**
 *  \brief crc16_usb.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint16_t crc16_usb(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint16_t crc = 0xFFFF;
    while (len--)
    {
        crc ^= *data++;    
        for (i = 0; i < 8; i++)
        {
            if (crc & 1) crc = (crc >> 1) ^ 0xA001; /* 0xA001 = reflect 0x8005 */
            else crc = (crc >> 1);
        }
    }
    return crc ^ 0xFFFF;
}

/**
 *  \brief crc16_modbus.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint16_t crc16_modbus(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint16_t crc = 0xFFFF;
    while (len--)
    {
        crc ^= *data++;    
        for (i = 0; i < 8; i++)
        {
            if (crc & 1) crc = (crc >> 1) ^ 0xA001; /* 0xA001 = reflect 0x8005 */
            else crc = (crc >> 1);
        }
    }
    return crc;
}

/**
 *  \brief crc16_ccitt.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint16_t crc16_ccitt(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint16_t crc = 0;
    while (len--)
    {
        crc ^= *data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 1) crc = (crc >> 1) ^ 0x8408; /* 0x8408 = reflect 0x1021 */
            else crc = (crc >> 1);
        }
    }
    return crc;
}

/**
 *  \brief crc16_ccitt_false.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint16_t crc16_ccitt_false(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint16_t crc = 0xFFFF;
    while (len--)
    {
        crc ^= (uint16_t)(*data++) << 8;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x8000) crc = (crc << 1) ^ 0x1021;
            else crc <<= 1;
        }
    }
    return crc;
}

/**
 *  \brief crc16_x25.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint16_t crc16_x25(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint16_t crc = 0xFFFF;
    while (len--)
    {
        crc ^= *data++;    
        for (i = 0; i < 8; i++)
        {
            if (crc & 1) crc = (crc >> 1) ^ 0x8408; /* 0x8408 = reflect 0x1021 */
            else crc = (crc >> 1);
        }
    }
    return crc ^ 0xFFFF;
}

/**
 *  \brief crc16_xmodem.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint16_t crc16_xmodem(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint16_t crc = 0;    
    while (len--)
    {
        crc ^= (uint16_t)(*data++) << 8;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x8000) crc = (crc << 1) ^ 0x1021;
            else crc <<= 1;
        }
    }
    return crc;
}

/**
 *  \brief crc16_dnp.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint16_t crc16_dnp(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint16_t crc = 0;    
    while (len--)
    {
        crc ^= *data++;    
        for (i = 0; i < 8; i++)
        {
            if (crc & 1) crc = (crc >> 1) ^ 0xA6BC; /* 0xA6BC = reflect 0x3D65 */
            else crc = (crc >> 1);
        }
    }
    return crc ^ 0xFFFF;
}

/**
 *  \brief crc32.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint32_t crc32(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint32_t crc = 0xFFFFFFFF;
    while (len--)
    {
        crc ^= *data++;        
        for (i = 0; i < 8; i++)
        {
            if (crc & 1) crc = (crc >> 1) ^ 0xEDB88320; /* 0xEDB88320 = reflect 0x04C11DB7 */
            else crc = (crc >> 1);
        }
    }
    return crc ^ 0xFFFFFFFF;
}

/**
 *  \brief crc32_mpeg_2.
 *  \param[in] data: address of data
 *  \param[in] len: length of data
 *  \return check value
 */
uint32_t crc32_mpeg_2(uint8_t* data, uint32_t len)
{
    uint8_t i;
    uint32_t crc = 0xFFFFFFFF;
    while (len--)
    {
        crc ^= (uint32_t)(*data++) << 24;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x80000000) crc = (crc << 1) ^ 0x04C11DB7;
            else crc <<= 1;
        }
    }
    return crc;
}
