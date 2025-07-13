/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  tool.h
 *         \unit  tool
 *        \brief  This is a C language common tool functions and macro definitions
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __tool_H
#define __tool_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdarg.h>
#include <stdio.h>

/* Version infomation */

#define TOOL_V_MAJOR                        1
#define TOOL_V_MINOR                        0
#define TOOL_V_PATCH                        0

/**
 *  \brief Show data by bit
 *  \param[in] data To show the base address of data.
 *  \param[in] width Number of bytes to show.
 *  \return none
 */
void showBits(void *data, unsigned int width);

/**
 *  \brief Show data by hex
 *  \param[in] data To show the base address of data.
 *  \param[in] len Number of bytes to show.
 *  \return none
 */
void showHex(void *data, int len);

/**
 *  \brief Converts an array of characters into a hexadecimal string representation.
 *
 *  \param[in] inArray The input array of characters.
 *  \param[in] maxInSize The maximum size of the input array.
 *  \param[out] outHexString The output hexadecimal string.
 *  \param[in] maxOutSize The maximum size of the output hexadecimal string.
 *  \return The length of the output hexadecimal string if successful, -1 if the output buffer is too small.
 */
int ToStringHex(char inArray[], unsigned int maxInSize, char outHexString[], unsigned int maxOutSize);

/**
* \brief Converts a null-terminated hexadecimal string into an array of characters.
*
* This function takes a null-terminated hexadecimal string as input and converts
* each hexadecimal digit into its corresponding ASCII character. The converted
* characters are stored in the outArray[] parameter. The function returns the
* length of the converted array.
*
* \param[in] inHexString The null-terminated hexadecimal string to be converted.
* \param[in] maxInSize The maximum size of the input character array.
* \param[out] outArray The array to store the converted characters.
* \param[in] maxOutSize The maximum size of the output character array.
*
* \return The length of the converted array if successful, or -1 if an error occurs.
*/
int GetStringHex(char inHexString[], unsigned int maxInSize, char outArray[], unsigned int maxOutSize);

#define setBit(data, i)             ((data)|=(1<<(i)))
#define clrBit(data, i)             ((data)&=(~(1<<(i))))
#define flpBit(data, i)             ((data)^=(1<<(i)))
#define getBit(data, i)             ((data)>>(i)&1)
#define chkBit(data, i)             ((data)&(1<<(i)))
#define MAX(x, y)                   ((x)>(y)?(x):(y))
#define MIN(x, y)                   ((x)<(y)?(x):(y))
#define ABS(x)                      (((x)>0)?(x):(0-(x)))
#define CEIL(num, deno)             (((num)+(deno)-1)/(deno))
#define FLOOR(num, deno)            ((num)/(deno))
#define STR(x)                      #x
#define CONCAT(a, b)                a##b
#define structOffset(s, m)          ((size_t)(&((s*)0)->m))
#define ASSERT(condition)           do{if(!(condition)){fprintf(stderr,"Assertion fail: %s, file %s, line %d\r\n",#condition,__FILE__,__LINE__);exit(0);}}while(0)
#define isNegative(n)               ((n)<0)
#define isEvenNum(n)                ((n)%2==0)
#define isOddNum(n)                 ((n)%2!=0)
#define isSameSign(n1, n2)          ((n1)*(n2)>0)
#define isLeapYear(y)               ((y)%4==0&&(y)%100!=0||(y)%400==0)
#define CLAMP(value, min, max)      (((value)<(min))?(min):(((value)>(max))?(max):(value)))
#define ArrayLength(a)              (sizeof(a)/sizeof((a)[0]))
#define SQUARE(x)                   ((x)*(x))
#define ALLOCATE(type, count)       (type*)malloc((count)*sizeof(type))
#define printChar(c)                printf(#c ": %c\r\n", c)
#define printInt(i)                 printf(#i ": %d\r\n", i)
#define printFloat(f)               printf(#f ": %f\r\n", f)
#define printString(s)              printf(#s ": %s\r\n", s)
#define printPoint(p)               printf(#p ": %p\r\n", p)
#define htol(x)                     ((uint32)(((x)&0xff000000)>>24)|((x)&0x00ff0000)<<8|((x)&0x0000ff00)<<24|((x)&0x000000ff)<<16)

#ifdef __cplusplus
}
#endif

#endif