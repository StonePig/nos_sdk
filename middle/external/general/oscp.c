/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  oscp.c
 *         \unit  oscp
 *        \brief  This is a simple analog oscilloscope module for C language
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "oscp.h"
#include <stdio.h>

static unsigned int scale = 10;
static int* monitor = NULL;

static void display(int level)
{
    static int prev = -1;
    int i = 0;
    int min, max;

    if (prev == -1) prev = level;

    if (prev < level) 
    {
        min = prev;
        max = level;
    }
    else
    {
        min = level;
        max = prev;
    }

    putchar('#');
    if (prev < 0) putchar('<');
    for (i = 0; i <= RESOLUTION; i++)
    {
        if (prev == i)
        {
            putchar('|');
        }
        else if (i > min && i < max)
        {
            putchar('_');
        }
        else
        {
            putchar(' ');
        }
    }
    if (prev > RESOLUTION) putchar('>');
    printf("#\r\n");

    printf("\r#");
    for (i = 0; i <= RESOLUTION; i++)
    {
        if (i % 10 == 0) putchar('+');
        else putchar('-');
    }
    printf("#\r");

    prev = level;
}

void oscp_handle(void)
{
    static unsigned int count = 0;

    count++;
    if (count >= 2520000) count = 0;

    if (count % scale == 0)
    {
        if (monitor) display(*monitor);
    }
}

int oscp_set_monitor(int *m)
{
    if (!m) return 0;

    monitor = m;

    return 1;
}

int oscp_set_scale(int s)
{
    if (s == O_SCALE_5MS) scale = 1;
    else if (s == O_SCALE_10MS) scale = 2;
    else if (s == O_SCALE_20MS) scale = 4;
    else if (s == O_SCALE_50MS) scale = 10;
    else if (s == O_SCALE_100MS) scale = 20;
    else if (s == O_SCALE_200MS) scale = 40;
    else if (s == O_SCALE_500MS) scale = 100;
    else if (s == O_SCALE_1S) scale = 200;
    else if (s == O_SCALE_2S) scale = 400;
    else return 0;

    return 1;
}
