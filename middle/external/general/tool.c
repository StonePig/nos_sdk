/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  tool.c
 *         \unit  tool
 *        \brief  This is a C language common tool functions and macro definitions
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "tool.h"

// #define DEBUG
#ifdef DEBUG
#define LOG(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define LOG(format, ...)
#endif // DEBUG

void showBits(void *data, unsigned int width)
{
    int i;
    unsigned char *base = data;
    while (width-- > 0) 
    {
        for (i = 0; i < 8; i++)
        {
            printf("%d", getBit(*base, (7-i))?1:0);
        }
        printf("\r\n");
        base++;
    }
}

void showHex(void *data, int len)
{
    int i;
    unsigned char *base = data;
    for (i = 0; i < len; i++)
    {
        printf("%02X ", base[i]);
    }
    printf("\r\n");
}

int ToStringHex(char inArray[], unsigned int maxInSize, char outHexString[], unsigned int maxOutSize)
{
    int len = 0;
    unsigned int i = 0, j = 0;
    unsigned char c;
    
    // Check if the output buffer is large enough to hold the hexadecimal string
    if (maxOutSize < maxInSize * 3)
    {
        return -1; // Return -1 if the output buffer is too small
    }
    
    // Convert each character into its hexadecimal representation
    for (i = 0; i < maxInSize; i++)
    {
        // Convert the upper 4 bits of the character into hexadecimal
        c = (inArray[i] >> 4) & 0x0F;
        outHexString[j++] = (c < 10) ? ('0' + c) : ('A' + c - 10);
        
        // Convert the lower 4 bits of the character into hexadecimal
        c = inArray[i] & 0x0F;
        outHexString[j++] = (c < 10) ? ('0' + c) : ('A' + c - 10);
        
        // Add a space between each pair of hexadecimal characters
        outHexString[j++] = ' ';
        
        // Increment the length counter
        len++;
    }
    
    // Null-terminate the output string
    outHexString[j - 1] = 0;
    
    return len; // Return the length of the output hexadecimal string
}

int GetStringHex(char inHexString[], unsigned int maxInSize, char outArray[], unsigned int maxOutSize)
{
    int len = 0; // Counter for the length of the output array
    unsigned int i = 0; // Counter for the input string index
    unsigned int width = 0; // Counter for the width of the converted characters
    char c = 0; // Temporary storage for the converted character

    // Skip any useless characters at the beginning of the input string
    while (i < maxInSize - 1 && inHexString[i] != 0 && inHexString[i] <= ' ')
        i++;

    // Convert each hexadecimal digit in the input string to its corresponding ASCII character
    while (i < maxInSize && inHexString[i] != 0)
    {
        if ('0' <= inHexString[i] && inHexString[i] <= '9')
        {
            c = (c << 4) | (inHexString[i] - '0');
            width++;
        }
        else if ('a' <= inHexString[i] && inHexString[i] <= 'f')
        {
            c = (c << 4) | (inHexString[i] - 'a' + 10);
            width++;
        }
        else if ('A' <= inHexString[i] && inHexString[i] <= 'F')
        {
            c = (c << 4) | (inHexString[i] - 'A' + 10);
            width++;
        }
        else if (' ' == inHexString[i])
        {
            outArray[len++] = c;
            if (len >= maxOutSize)
                break;
            width = 0;
            c = 0;
            // Skip any useless characters between the hexadecimal digits
            while (i < maxInSize - 1 && inHexString[i] != 0 && inHexString[i] <= ' ')
                i++;
            continue;
        }
        else
        {
            return -1; // Return -1 if an invalid hexadecimal digit is encountered
        }

        i++;
    }

    // Append the last converted character if not followed by any useless characters
    if (width > 0)
        outArray[len++] = c;

    return len; // Return the length of the converted array
}
