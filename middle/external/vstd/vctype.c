/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  vctype.c
 *         \unit  vctype
 *        \brief  Similar to the C standard library ctype
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "vctype.h"

int v_tolower(int c) 
{
    if (c >= 'A' && c <= 'Z')
    {
        return c + ('a' - 'A');
    }
    return c;
}

int v_toupper(int c)
{
    if (c >= 'a' && c <= 'z')
    {
        return c - ('a' - 'A');
    }
    return c;
}

int v_isascii(int c)
{
    return (c >= 0 && c <= 127);
}

int v_toascii(int c)
{
    return (c & 0x7f);
}

int v_isalpha(int c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int v_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

int v_isalnum(int c)
{
    return (v_isalpha(c) || v_isdigit(c));
}

int v_iscntrl(int c)
{
    return ((c >= 0 && c <= 31) || (c == 127));
}

int v_islower(int c)
{
    return (c >= 'a' && c <= 'z');
}

int v_isupper(int c)
{
    return (c >= 'A' && c <= 'Z');
}

int v_isgraph(int c)
{
    return (c >= '!' && c <= '~');
}

int v_isprint(int c)
{
    return (c >= ' ' && c <= '~');
}

int v_ispunct(int c)
{
    return ((c >= '!' && c <= '/') || (c >= ':' && c <= '@') || (c >= '[' && c <= '`') || (c >= '{' && c <= '~'));
}

int v_isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

int v_isxdigit(int c)
{
    return (v_isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}

int v_isctype(int c, int mask)
{
    if (mask & (_V_ISalnum | _V_ISalpha | _V_ISdigit | _V_ISlower | _V_ISprint | _V_ISpunct | _V_ISspace | _V_ISupper))
    {
        if (mask & _V_ISalnum) 
        {
            return v_isalnum(c);
        }
        if (mask & _V_ISalpha)
        {
            return v_isalpha(c);
        }
        if (mask & _V_ISdigit)
        {
            return v_isdigit(c);
        }
        if (mask & _V_ISlower)
        {
            return v_islower(c);
        }
        if (mask & _V_ISprint)
        {
            return v_isprint(c);
        }
        if (mask & _V_ISpunct)
        {
            return v_ispunct(c);
        }
        if (mask & _V_ISspace)
        {
            return v_isspace(c);
        }
        if (mask & _V_ISupper)
        {
            return v_isupper(c);
        }
    }
    return 0;
}