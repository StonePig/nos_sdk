/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  cPatten.c
 *         \unit  cPatten
 *        \brief  This is a C language artistic character patterns
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "cPatten.h"
#include <stdio.h>
#include <string.h>

typedef struct 
{
    char *line[5];
} artCharType_simple;

static artCharType_simple artCharType_simpleTable[] = {
    /* A */
    {
        "  AAA  ",
        " A   A ",
        " AAAAA ",
        " A   A ",
        " A   A ",
    },
    /* B */
    {
        " BBB   ",
        " B   B ", 
        " BBB   ",
        " B   B ", 
        " BBB   ",
    },
    /* C */
    {
        "  CCC  ",
        " C   C ",
        " C     ",
        " C   C ",
        "  CCC  ",
    },
    /* D */
    {
        " DDD   ",
        " D   D ", 
        " D   D ", 
        " D   D ", 
        " DDD   ",
    },
    /* E */
    {
        " EEEE ", 
        " E    ", 
        " EEE  ", 
        " E    ", 
        " EEEE ", 
    },
    /* F */
    {
        " FFFFF ", 
        " F     ", 
        " FFF   ", 
        " F     ", 
        " F     ", 
    },
    /* G */
    {
        "  GGG  ",
        " G     ",
        " G  GG ",
        " G   G ",
        "  GGG  ", 
    },
    /* H */
    {
        " H   H ", 
        " H   H ", 
        " HHHHH ", 
        " H   H ", 
        " H   H ", 
    },
    /* I */
    {
        " III ", 
        "  I  ", 
        "  I  ", 
        "  I  ", 
        " III ", 
    },
    /* J */
    {
        "    JJJ ", 
        "      J ", 
        "      J ", 
        " J    J ", 
        "  JJJJ  ", 
    },
    /* K */
    {
        " K   K ", 
        " K  K  ", 
        " KK    ", 
        " K  K  ", 
        " K   K ",
    },
    /* L */
    {
        " L     ", 
        " L     ", 
        " L     ", 
        " L     ", 
        " LLLLL ", 
    },
    /* M */
    {
        " M     M ", 
        " MM   MM ", 
        " M M M M ", 
        " M  M  M ", 
        " M     M ", 
    },
    /* N */
    {
        " N     N ", 
        " NN    N ", 
        " N N   N ", 
        " N  N  N ", 
        " N   N N ", 
    },
    /* O */
    {
        "  OOO  ", 
        " O   O ", 
        " O   O ", 
        " O   O ", 
        "  OOO  ",  
    },
    /* P */
    {
        " PPP   ",
        " P   P ", 
        " PPP   ",
        " P     ", 
        " P     ",  
    },
    /* Q */
    {
        "  QQQ   ",
        " Q   Q  ",
        " Q   Q  ",
        " Q  QQ  ",
        "  QQQ Q ",
    },
    /* R */
    {
        " RRR   ",
        " R   R ",
        " RRR   ",
        " R  R  ",
        " R   R ",
    },
    /* S */
    {
        "  SSS  ",
        " S     ",
        "  SS   ",
        "     S ",
        "  SSS  ",
    },
    /* T */
    {
        " TTTTT ",  
        "   T   ",  
        "   T   ",  
        "   T   ",  
        "   T   ",
    },
    /* U */
    {
        " U   U ", 
        " U   U ", 
        " U   U ", 
        " U   U ", 
        "  UUU  ",
    },
    /* V */
    {
        " V     V ", 
        " V     V ", 
        "  V   V  ", 
        "   V V   ", 
        "    V    ",
    },
    /* W */
    {
        " W       W ", 
        " W       W ", 
        " W   W   W ", 
        " W  W W  W ", 
        "  W   W W  ",
    },
    /* X */
    {
        " X   X ", 
        "  X X  ", 
        "   X   ", 
        "  X X  ", 
        " X   X ",
    },
    /* Y */
    {
        " Y   Y ", 
        "  Y Y  ", 
        "   Y   ", 
        "   Y   ", 
        "   Y   ", 
    },
    /* Z */
    {
        " ZZZZZ ", 
        "    Z  ", 
        "   Z   ", 
        "  Z    ", 
        " ZZZZZ ", 
    }
}; 

static void cPatten_putChar(char c, char mask)
{
    if (mask && c != ' ')
    {
        putchar(mask);
    }
    else  
    {
        putchar(c);
    }
}

static void cPatten_putString(char *s, char mask)
{
    while (*s)
    {
        cPatten_putChar(*s, mask);
        s++;
    }
}

static char cMask = 0;

int cPatten_setMask(char c)
{
    if ((c >= ' ' && c <= '~') || c == 0)
    {
        cMask = c;
    }
    else return 0;

    return 1;
}

void cPatten_showChar(char c)
{
    int i;
    int tableIndex = -1;

    if ('A' <= c && c <= 'Z') tableIndex = c - 'A';
    if ('a' <= c && c <= 'z') tableIndex = c - 'a';

    if (tableIndex < 0) return;

    for (i = 0; i < 5; i++)
    {
        cPatten_putString(artCharType_simpleTable[tableIndex].line[i], cMask);
        printf("\r\n");
    }
}

void cPatten_showString(char *s)
{
    int len, i, j;
    int tableIndex;
    char c;

    if (!s) return;

    len = strlen(s);

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < len; j++)
        {
            c = s[j];

            tableIndex = -1;

            if ('A' <= c && c <= 'Z') tableIndex = c - 'A';
            if ('a' <= c && c <= 'z') tableIndex = c - 'a';

            if (tableIndex < 0) continue;

            cPatten_putString(artCharType_simpleTable[tableIndex].line[i], cMask);
        }
        printf("\r\n");
    }
}