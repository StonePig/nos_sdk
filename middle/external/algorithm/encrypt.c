/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  encrypt.c
 *         \unit  encrypt
 *        \brief  This is a C language version of common encryption and decryption algorithms
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "encrypt.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* des key */
static uint32_t des_sk[32] = {0};
static uint32_t des3_esk[96], des3_dsk[96];

/* expanded des s-boxes */
static const uint32_t sb[8][64] = {
    { 0x01010400, 0x00000000, 0x00010000, 0x01010404, 0x01010004, 0x00010404, 0x00000004, 0x00010000, 0x00000400, 0x01010400, 0x01010404, 0x00000400, 0x01000404, 0x01010004, 0x01000000, 0x00000004, 0x00000404, 0x01000400, 0x01000400, 0x00010400, 0x00010400, 0x01010000, 0x01010000, 0x01000404, 0x00010004, 0x01000004, 0x01000004, 0x00010004, 0x00000000, 0x00000404, 0x00010404, 0x01000000, 0x00010000, 0x01010404, 0x00000004, 0x01010000, 0x01010400, 0x01000000, 0x01000000, 0x00000400, 0x01010004, 0x00010000, 0x00010400, 0x01000004, 0x00000400, 0x00000004, 0x01000404, 0x00010404, 0x01010404, 0x00010004, 0x01010000, 0x01000404, 0x01000004, 0x00000404, 0x00010404, 0x01010400, 0x00000404, 0x01000400, 0x01000400, 0x00000000, 0x00010004, 0x00010400, 0x00000000, 0x01010004 },
    { 0x80108020, 0x80008000, 0x00008000, 0x00108020, 0x00100000, 0x00000020, 0x80100020, 0x80008020, 0x80000020, 0x80108020, 0x80108000, 0x80000000, 0x80008000, 0x00100000, 0x00000020, 0x80100020, 0x00108000, 0x00100020, 0x80008020, 0x00000000, 0x80000000, 0x00008000, 0x00108020, 0x80100000, 0x00100020, 0x80000020, 0x00000000, 0x00108000, 0x00008020, 0x80108000, 0x80100000, 0x00008020, 0x00000000, 0x00108020, 0x80100020, 0x00100000, 0x80008020, 0x80100000, 0x80108000, 0x00008000, 0x80100000, 0x80008000, 0x00000020, 0x80108020, 0x00108020, 0x00000020, 0x00008000, 0x80000000, 0x00008020, 0x80108000, 0x00100000, 0x80000020, 0x00100020, 0x80008020, 0x80000020, 0x00100020, 0x00108000, 0x00000000, 0x80008000, 0x00008020, 0x80000000, 0x80100020, 0x80108020, 0x00108000 },
    { 0x00000208, 0x08020200, 0x00000000, 0x08020008, 0x08000200, 0x00000000, 0x00020208, 0x08000200, 0x00020008, 0x08000008, 0x08000008, 0x00020000, 0x08020208, 0x00020008, 0x08020000, 0x00000208, 0x08000000, 0x00000008, 0x08020200, 0x00000200, 0x00020200, 0x08020000, 0x08020008, 0x00020208, 0x08000208, 0x00020200, 0x00020000, 0x08000208, 0x00000008, 0x08020208, 0x00000200, 0x08000000, 0x08020200, 0x08000000, 0x00020008, 0x00000208, 0x00020000, 0x08020200, 0x08000200, 0x00000000, 0x00000200, 0x00020008, 0x08020208, 0x08000200, 0x08000008, 0x00000200, 0x00000000, 0x08020008, 0x08000208, 0x00020000, 0x08000000, 0x08020208, 0x00000008, 0x00020208, 0x00020200, 0x08000008, 0x08020000, 0x08000208, 0x00000208, 0x08020000, 0x00020208, 0x00000008, 0x08020008, 0x00020200 },
    { 0x00802001, 0x00002081, 0x00002081, 0x00000080, 0x00802080, 0x00800081, 0x00800001, 0x00002001, 0x00000000, 0x00802000, 0x00802000, 0x00802081, 0x00000081, 0x00000000, 0x00800080, 0x00800001, 0x00000001, 0x00002000, 0x00800000, 0x00802001, 0x00000080, 0x00800000, 0x00002001, 0x00002080, 0x00800081, 0x00000001, 0x00002080, 0x00800080, 0x00002000, 0x00802080, 0x00802081, 0x00000081, 0x00800080, 0x00800001, 0x00802000, 0x00802081, 0x00000081, 0x00000000, 0x00000000, 0x00802000, 0x00002080, 0x00800080, 0x00800081, 0x00000001, 0x00802001, 0x00002081, 0x00002081, 0x00000080, 0x00802081, 0x00000081, 0x00000001, 0x00002000, 0x00800001, 0x00002001, 0x00802080, 0x00800081, 0x00002001, 0x00002080, 0x00800000, 0x00802001, 0x00000080, 0x00800000, 0x00002000, 0x00802080 },
    { 0x00000100, 0x02080100, 0x02080000, 0x42000100, 0x00080000, 0x00000100, 0x40000000, 0x02080000, 0x40080100, 0x00080000, 0x02000100, 0x40080100, 0x42000100, 0x42080000, 0x00080100, 0x40000000, 0x02000000, 0x40080000, 0x40080000, 0x00000000, 0x40000100, 0x42080100, 0x42080100, 0x02000100, 0x42080000, 0x40000100, 0x00000000, 0x42000000, 0x02080100, 0x02000000, 0x42000000, 0x00080100, 0x00080000, 0x42000100, 0x00000100, 0x02000000, 0x40000000, 0x02080000, 0x42000100, 0x40080100, 0x02000100, 0x40000000, 0x42080000, 0x02080100, 0x40080100, 0x00000100, 0x02000000, 0x42080000, 0x42080100, 0x00080100, 0x42000000, 0x42080100, 0x02080000, 0x00000000, 0x40080000, 0x42000000, 0x00080100, 0x02000100, 0x40000100, 0x00080000, 0x00000000, 0x40080000, 0x02080100, 0x40000100 },
    { 0x20000010, 0x20400000, 0x00004000, 0x20404010, 0x20400000, 0x00000010, 0x20404010, 0x00400000, 0x20004000, 0x00404010, 0x00400000, 0x20000010, 0x00400010, 0x20004000, 0x20000000, 0x00004010, 0x00000000, 0x00400010, 0x20004010, 0x00004000, 0x00404000, 0x20004010, 0x00000010, 0x20400010, 0x20400010, 0x00000000, 0x00404010, 0x20404000, 0x00004010, 0x00404000, 0x20404000, 0x20000000, 0x20004000, 0x00000010, 0x20400010, 0x00404000, 0x20404010, 0x00400000, 0x00004010, 0x20000010, 0x00400000, 0x20004000, 0x20000000, 0x00004010, 0x20000010, 0x20404010, 0x00404000, 0x20400000, 0x00404010, 0x20404000, 0x00000000, 0x20400010, 0x00000010, 0x00004000, 0x20400000, 0x00404010, 0x00004000, 0x00400010, 0x20004010, 0x00000000, 0x20404000, 0x20000000, 0x00400010, 0x20004010 },
    { 0x00200000, 0x04200002, 0x04000802, 0x00000000, 0x00000800, 0x04000802, 0x00200802, 0x04200800, 0x04200802, 0x00200000, 0x00000000, 0x04000002, 0x00000002, 0x04000000, 0x04200002, 0x00000802, 0x04000800, 0x00200802, 0x00200002, 0x04000800, 0x04000002, 0x04200000, 0x04200800, 0x00200002, 0x04200000, 0x00000800, 0x00000802, 0x04200802, 0x00200800, 0x00000002, 0x04000000, 0x00200800, 0x04000000, 0x00200800, 0x00200000, 0x04000802, 0x04000802, 0x04200002, 0x04200002, 0x00000002, 0x00200002, 0x04000000, 0x04000800, 0x00200000, 0x04200800, 0x00000802, 0x00200802, 0x04200800, 0x00000802, 0x04000002, 0x04200802, 0x04200000, 0x00200800, 0x00000000, 0x00000002, 0x04200802, 0x00000000, 0x00200802, 0x04200000, 0x00000800, 0x04000002, 0x04000800, 0x00000800, 0x00200002 },
    { 0x10001040, 0x00001000, 0x00040000, 0x10041040, 0x10000000, 0x10001040, 0x00000040, 0x10000000, 0x00040040, 0x10040000, 0x10041040, 0x00041000, 0x10041000, 0x00041040, 0x00001000, 0x00000040, 0x10040000, 0x10000040, 0x10001000, 0x00001040, 0x00041000, 0x00040040, 0x10040040, 0x10041000, 0x00001040, 0x00000000, 0x00000000, 0x10040040, 0x10000040, 0x10001000, 0x00041040, 0x00040000, 0x00041040, 0x00040000, 0x10041000, 0x00001000, 0x00000040, 0x10040040, 0x00001000, 0x00041040, 0x10001000, 0x00000040, 0x10000040, 0x10040000, 0x10040040, 0x10000000, 0x00040000, 0x10001040, 0x00000000, 0x10041040, 0x00040040, 0x10000040, 0x10040000, 0x10001000, 0x10001040, 0x00000000, 0x10041040, 0x00041000, 0x00041000, 0x00001040, 0x00001040, 0x00040040, 0x10000000, 0x10041000 },
};

/* pc1: left and right halves bit-swap */
static const uint32_t lhs[16] = { 0x00000000, 0x00000001, 0x00000100, 0x00000101, 0x00010000, 0x00010001, 0x00010100, 0x00010101, 0x01000000, 0x01000001, 0x01000100, 0x01000101, 0x01010000, 0x01010001, 0x01010100, 0x01010101 };
static const uint32_t rhs[16] = { 0x00000000, 0x01000000, 0x00010000, 0x01010000, 0x00000100, 0x01000100, 0x00010100, 0x01010100, 0x00000001, 0x01000001, 0x00010001, 0x01010001, 0x00000101, 0x01000101, 0x00010101, 0x01010101 };

/* 32-bit integer manipulation macros (big endian) */
#define get_uint32_be(n, b, i) do{(n)=((uint32_t)(b)[(i)]<<24)|((uint32_t)(b)[(i)+1]<<16)|((uint32_t)(b)[(i)+2]<<8)|((uint32_t)(b)[(i)+3]);}while(0)
#define put_uint32_be(n, b, i) do{(b)[(i)]=(uint8_t)((n)>>24);(b)[(i)+1]=(uint8_t)((n)>>16);(b)[(i)+2]=(uint8_t)((n)>>8);(b)[(i)+3]=(uint8_t)(n);}while(0)

/* initial permutation macro */
#define des_ip(x, y)  do                                              \
{                                                                     \
    t = (((x) >>  4) ^ (y)) & 0x0F0F0F0F; (y) ^= t; (x) ^= (t <<  4); \
    t = (((x) >> 16) ^ (y)) & 0x0000FFFF; (y) ^= t; (x) ^= (t << 16); \
    t = (((y) >>  2) ^ (x)) & 0x33333333; (x) ^= t; (y) ^= (t <<  2); \
    t = (((y) >>  8) ^ (x)) & 0x00FF00FF; (x) ^= t; (y) ^= (t <<  8); \
    (y) = (((y) << 1) | ((y) >> 31)) & 0xFFFFFFFF;                    \
    t = ((x) ^ (y)) & 0xAAAAAAAA; (y) ^= t; (x) ^= t;                 \
    (x) = (((x) << 1) | ((x) >> 31)) & 0xFFFFFFFF;                    \
} while (0)

/* final permutation macro */
#define des_fp(x, y) do                                               \
{                                                                     \
    (x) = (((x) << 31) | ((x) >> 1)) & 0xFFFFFFFF;                    \
    t = ((x) ^ (y)) & 0xAAAAAAAA; (x) ^= t; (y) ^= t;                 \
    (y) = (((y) << 31) | ((y) >> 1)) & 0xFFFFFFFF;                    \
    t = (((y) >>  8) ^ (x)) & 0x00FF00FF; (x) ^= t; (y) ^= (t <<  8); \
    t = (((y) >>  2) ^ (x)) & 0x33333333; (x) ^= t; (y) ^= (t <<  2); \
    t = (((x) >> 16) ^ (y)) & 0x0000FFFF; (y) ^= t; (x) ^= (t << 16); \
    t = (((x) >>  4) ^ (y)) & 0x0F0F0F0F; (y) ^= t; (x) ^= (t <<  4); \
} while (0)

/* des round macro */
#define des_round(k,x,y) do { \
    t = (k) ^ (x); \
    (y) ^= sb[7][(t) & 0x3F] ^ sb[5][((t)>>8) & 0x3F] ^ sb[3][((t)>>16) & 0x3F] ^ sb[1][((t)>>24) & 0x3F]; \
    t = (k) ^ (((x)<<28) | ((x)>>4)); \
    (y) ^= sb[6][(t) & 0x3F] ^ sb[4][((t)>>8) & 0x3F] ^ sb[2][((t)>>16) & 0x3F] ^ sb[0][((t)>>24) & 0x3F]; \
} while (0)

static void set_keys(uint32_t SK[32], const uint8_t key[8])
{
    int32_t i;
    uint32_t x, y, t;

    get_uint32_be(x, key, 0);
    get_uint32_be(y, key, 4);

    /* permuted choice 1 */
    t =  ((y >>  4) ^ x) & 0x0F0F0F0F;  x ^= t; y ^= (t <<  4);
    t =  ((y      ) ^ x) & 0x10101010;  x ^= t; y ^= (t      );

    x =   (lhs[ (x      ) & 0xF] << 3) | (lhs[ (x >>  8) & 0xF ] << 2)
        | (lhs[ (x >> 16) & 0xF] << 1) | (lhs[ (x >> 24) & 0xF ]     )
        | (lhs[ (x >>  5) & 0xF] << 7) | (lhs[ (x >> 13) & 0xF ] << 6)
        | (lhs[ (x >> 21) & 0xF] << 5) | (lhs[ (x >> 29) & 0xF ] << 4);

    y =   (rhs[ (y >>  1) & 0xF] << 3) | (rhs[ (y >>  9) & 0xF ] << 2)
        | (rhs[ (y >> 17) & 0xF] << 1) | (rhs[ (y >> 25) & 0xF ]     )
        | (rhs[ (y >>  4) & 0xF] << 7) | (rhs[ (y >> 12) & 0xF ] << 6)
        | (rhs[ (y >> 20) & 0xF] << 5) | (rhs[ (y >> 28) & 0xF ] << 4);

    x &= 0x0FFFFFFF;
    y &= 0x0FFFFFFF;

    /* calculate subkeys */
    for (i = 0; i < 16; i++)
    {
        if (i < 2 || i == 8 || i == 15)
        {
            x = ((x << 1) | (x >> 27)) & 0x0FFFFFFF;
            y = ((y << 1) | (y >> 27)) & 0x0FFFFFFF;
        }
        else
        {
            x = ((x << 2) | (x >> 26)) & 0x0FFFFFFF;
            y = ((y << 2) | (y >> 26)) & 0x0FFFFFFF;
        }

        *SK++ =   ((x <<  4) & 0x24000000) | ((x << 28) & 0x10000000)
                | ((x << 14) & 0x08000000) | ((x << 18) & 0x02080000)
                | ((x <<  6) & 0x01000000) | ((x <<  9) & 0x00200000)
                | ((x >>  1) & 0x00100000) | ((x << 10) & 0x00040000)
                | ((x <<  2) & 0x00020000) | ((x >> 10) & 0x00010000)
                | ((y >> 13) & 0x00002000) | ((y >>  4) & 0x00001000)
                | ((y <<  6) & 0x00000800) | ((y >>  1) & 0x00000400)
                | ((y >> 14) & 0x00000200) | ((y      ) & 0x00000100)
                | ((y >>  5) & 0x00000020) | ((y >> 10) & 0x00000010)
                | ((y >>  3) & 0x00000008) | ((y >> 18) & 0x00000004)
                | ((y >> 26) & 0x00000002) | ((y >> 24) & 0x00000001);

        *SK++ =   ((x << 15) & 0x20000000) | ((x << 17) & 0x10000000)
                | ((x << 10) & 0x08000000) | ((x << 22) & 0x04000000)
                | ((x >>  2) & 0x02000000) | ((x <<  1) & 0x01000000)
                | ((x << 16) & 0x00200000) | ((x << 11) & 0x00100000)
                | ((x <<  3) & 0x00080000) | ((x >>  6) & 0x00040000)
                | ((x << 15) & 0x00020000) | ((x >>  4) & 0x00010000)
                | ((y >>  2) & 0x00002000) | ((y <<  8) & 0x00001000)
                | ((y >> 14) & 0x00000808) | ((y >>  9) & 0x00000400)
                | ((y      ) & 0x00000200) | ((y <<  7) & 0x00000100)
                | ((y >>  7) & 0x00000020) | ((y >>  3) & 0x00000011)
                | ((y <<  2) & 0x00000004) | ((y >> 21) & 0x00000002);
    }
}

/**
 *  \brief set the key for des encryption and decryption.
 *  \param[in] key: 8 byte key
 *  \return 1 success
 */
int des_set_key(const uint8_t key[8])
{
    set_keys(des_sk, key);
    return 1;
}

/**
 *  \brief des ecb encryption and decryption.
 *  \param[in] input: 8 byte input
 *  \param[out] output: 8 byte output
 *  \param[in] mode: DES_ENCRYPT or DES_DECRYPT
 *  \return 1 success
 */
int des_crypt_ecb(const uint8_t input[8], uint8_t output[8], uint8_t mode)
{
    int32_t i;
    uint32_t x, y, t, k;

    get_uint32_be(x, input, 0);
    get_uint32_be(y, input, 4);

    des_ip(x, y);

    for (i = 0; i < 8; i++)
    {
        k = des_sk[mode==DES_ENCRYPT?i:7-i];
        des_round(k, y, x);
        des_round(k, x, y);
    }

    des_fp(y, x);

    put_uint32_be(y, output, 0);
    put_uint32_be(x, output, 4);

    return 1;
}

/**
 *  \brief des cbc encryption and decryption.
 *  \param[in] input: input
 *  \param[out] output: output
 *  \param[in] length: the length of input and output needs to be a multiple of 8
 *  \param[in] mode: DES_ENCRYPT or DES_DECRYPT
 *  \return 1 success
 */
int des_crypt_cbc(const uint8_t *input, uint8_t *output, uint32_t length, uint8_t mode)
{
    int32_t i;
    uint8_t temp[8];
    uint8_t iv[8] = {0};

    if (length % 8) return 0;

    if (mode == DES_ENCRYPT)
    {
        while (length > 0)
        {
            for (i = 0; i < 8; i++) output[i] = (uint8_t)(input[i] ^ iv[i]);
            des_crypt_ecb(output, output, mode);
            memcpy(iv, output, 8);
            input  += 8;
            output += 8;
            length -= 8;
        }
    }
    else
    {
        while (length > 0)
        {
            memcpy(temp, input, 8);
            des_crypt_ecb(input, output, mode);
            for (i = 0; i < 8; i++) output[i] = (uint8_t)(output[i] ^ iv[i]);
            memcpy(iv, temp, 8);
            input  += 8;
            output += 8;
            length -= 8;
        }
    }

    return 1;
}

/**
 *  \brief set the double key for des3 encryption and decryption.
 *  \param[in] key: 16 byte key
 *  \return 1 success
 */
int des3_set_key2(const uint8_t key[16])
{
    int32_t i;
    set_keys(des3_esk, key);
    set_keys(des3_dsk + 32, key + 8);
    for (i = 0; i < 32; i += 2)
    {
        des3_dsk[i     ] = des3_esk[30 - i];
        des3_dsk[i +  1] = des3_esk[31 - i];

        des3_esk[i + 32] = des3_dsk[62 - i];
        des3_esk[i + 33] = des3_dsk[63 - i];

        des3_esk[i + 64] = des3_esk[i    ];
        des3_esk[i + 65] = des3_esk[i + 1];

        des3_dsk[i + 64] = des3_dsk[i    ];
        des3_dsk[i + 65] = des3_dsk[i + 1];
    }
    return 1;
}

/**
 *  \brief set the triple key for des3 encryption and decryption.
 *  \param[in] key: 24 byte key
 *  \return 1 success
 */
int des3_set_key3(const uint8_t key[24])
{
    int32_t i;
    set_keys(des3_esk, key);
    set_keys(des3_dsk + 32, key +  8);
    set_keys(des3_esk + 64, key + 16);
    for (i = 0; i < 32; i += 2)
    {
        des3_dsk[i     ] = des3_esk[94 - i];
        des3_dsk[i +  1] = des3_esk[95 - i];

        des3_esk[i + 32] = des3_dsk[62 - i];
        des3_esk[i + 33] = des3_dsk[63 - i];

        des3_dsk[i + 64] = des3_esk[30 - i];
        des3_dsk[i + 65] = des3_esk[31 - i];
    }
    return 1;
}

/**
 *  \brief des3 ecb encryption and decryption.
 *  \param[in] input: 8 byte input
 *  \param[out] output: 8 byte output
 *  \param[in] mode: DES_ENCRYPT or DES_DECRYPT
 *  \return 1 success
 */
int des3_crypt_ecb(const uint8_t input[8], uint8_t output[8], uint8_t mode)
{
    int32_t i, r = 3;
    uint32_t x, y, t, *sk;

    get_uint32_be(x, input, 0);
    get_uint32_be(y, input, 4);

    des_ip(x, y);

    sk = ((mode==DES_ENCRYPT)?des3_esk:des3_dsk);

    for (i = 0; i < 8; i++)
    {
        des_round(*sk++, y, x);
        des_round(*sk++, x, y);
    }
    for (i = 0; i < 8; i++)
    {
        des_round(*sk++, x, y);
        des_round(*sk++, y, x);
    }
    for (i = 0; i < 8; i++)
    {
        des_round(*sk++, y, x);
        des_round(*sk++, x, y);
    }

    des_fp(y, x);

    put_uint32_be(y, output, 0);
    put_uint32_be(x, output, 4);

    return 1;
}

/**
 *  \brief des3 cbc encryption and decryption.
 *  \param[in] input: input
 *  \param[out] output: output
 *  \param[in] length: the length of input and output needs to be a multiple of 8
 *  \param[in] mode: DES_ENCRYPT or DES_DECRYPT
 *  \return 1 success
 */
int des3_crypt_cbc(const uint8_t *input, uint8_t *output, uint32_t length, uint8_t mode)
{
    int32_t i;
    uint8_t temp[8];
    uint8_t iv[8] = {0};

    if (length % 8) return 0;

    if (mode == DES_ENCRYPT)
    {
        while (length > 0)
        {
            for (i = 0; i < 8; i++) output[i] = (uint8_t)(input[i] ^ iv[i]);
            des3_crypt_ecb(output, output, mode);
            memcpy(iv, output, 8);
            input  += 8;
            output += 8;
            length -= 8;
        }
    }
    else
    {
        while (length > 0)
        {
            memcpy(temp, input, 8);
            des3_crypt_ecb(input, output, mode);
            for (i = 0; i < 8; i++) output[i] = (uint8_t)(output[i] ^ iv[i]);
            memcpy(iv, temp, 8);
            input  += 8;
            output += 8;
            length -= 8;
        }
    }

    return 1;
}

/* TODO: mbedtls
    AES 
    SHA1 
    MD5 
    HMAC 
    blowfish 
    RSA 
*/