/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  vstdlib.c
 *         \unit  vstdlib
 *        \brief  Similar to the C standard library stdlib
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "vstdlib.h"
#include "vctype.h"
#include "vstddef.h"

int v_atoi(const char *str)
{
    int result = v_atoll(str);
    return result;
}

long int v_atol(const char *str)
{
    long int result = v_atoll(str);
    return result;
}

long long int v_atoll(const char *str)
{
    long long int result = 0;
    int sign = 1;
    int i = 0;
    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i] == '+')
    {
        i++;
    }
    while (str[i] != '\0' && v_isdigit(str[i]))
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return sign * result;
}

double v_atof(const char *str)
{
    return v_strtod(str, NULL);
}

float v_strtof(const char *str, char **endptr)
{
    float result = v_strtold(str, endptr);
    return result;
}

double v_strtod(const char *str, char **endptr)
{
    double result = v_strtold(str, endptr);
    return result;
}

long double v_strtold(const char *str, char **endptr)
{
    long double result = 0.0L;
    int sign = 1;
    int fraction = 0;
    long double div = 1.0L;
    int i = 0;
    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i] == '+')
    {
        i++;
    }
    while (str[i] != '\0' && (v_isdigit(str[i]) || str[i] == '.'))
    {
        if (str[i] == '.')
        {
            fraction = 1;
        }
        else if (fraction == 0)
        {
            result = result * 10.0L + (long double) (str[i] - '0');
        }
        else
        {
            div *= 10.0L;
            result = result + (long double) (str[i] - '0') / div;
        }
        i++;
    }
    if (endptr != NULL)
    {
        *endptr = (char *) &str[i];
    }
    return sign * result;
}

static int hex_to_int(char c)
{
    if (v_isdigit(c))
    {
        return c - '0';
    }
    else if (v_islower(c))
    {
        return c - 'a' + 10;
    }
    else if (v_isupper(c))
    {
        return c - 'A' + 10;
    }
    else
    {
        return 0;
    }
}

float v_strtof16(const char *str, char **endptr)
{
    float result = 0.0f;
    int i = 0;
    if (str[i] == '-')
    {
        i++;
    }
    else if (str[i] == '+')
    {
        i++;
    }
    if (str[i] == '0' && (str[i+1] == 'x' || str[i+1] == 'X'))
    {
        i += 2;
    }
    while (v_isxdigit(str[i]))
    {
        result = result * 16.0f + (float) hex_to_int(str[i]);
        i++;
    }
    if (endptr != NULL)
    {
        *endptr = (char *) &str[i];
    }
    return result;
}

float v_strtof32(const char *str, char **endptr)
{
    float result = v_strtof(str, endptr);
    return result;
}

double v_strtof64(const char *str, char **endptr)
{
    double result = v_strtod(str, endptr);
    return result;
}

long double v_strtof128(const char *str, char **endptr)
{
    long double result = v_strtold(str, endptr);
    return result;
}

float v_strtof32x(const char *str, char **endptr)
{
    float result = v_strtof128x(str, endptr);
    return result;
}

double v_strtof64x(const char *str, char **endptr)
{
    double result = v_strtof128x(str, endptr);
    return result;
}

long double v_strtof128x(const char *str, char **endptr)
{
    long double result = 0.0L;
    int i = 0;
    if (str[i] == '-')
    {
        i++;
    }
    else if (str[i] == '+')
    {
        i++;
    }
    if (str[i] == '0' && (str[i+1] == 'x' || str[i+1] == 'X'))
    {
        i += 2;
    }
    while (v_isxdigit(str[i]))
    {
        result = result * 16.0L + (long double) hex_to_int(str[i]);
        i++;
    }
    if (endptr != NULL)
    {
        *endptr = (char *) &str[i];
    }
    return result;
}

long int v_strtol(const char *str, char **endptr, int base)
{
    long int result = v_strtoq(str, endptr, base);
    return result;
}

unsigned long int v_strtoul(const char *str, char **endptr, int base)
{
    unsigned long int result = v_strtouq(str, endptr, base);
    return result;
}

long long int v_strtoq(const char *str, char **endptr, int base)
{
    long long int result = 0;
    int sign = 1;
    int i = 0;
    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i] == '+')
    {
        i++;
    }
    if (base == 0)
    {
        if (str[i] == '0' && (str[i + 1] == 'x' || str[i + 1] == 'X'))
        {
            base = 16;
            i += 2;
        }
        else if (str[i] == '0')
        {
            base = 8;
            i++;
        }
        else
        {
            base = 10;
        }
    }
    while (str[i] != '\0')
    {
        int digit = 0;
        if (str[i] >= '0' && str[i] <= '9')
        {
            digit = str[i] - '0';
        }
        else if (str[i] >= 'a' && str[i] <= 'z')
        {
            digit = str[i] - 'a' + 10;
        }
        else if (str[i] >= 'A' && str[i] <= 'Z')
        {
            digit = str[i] - 'A' + 10;
        }
        else
        {
            break;
        }
        if (digit >= base)
        {
            break;
        }
        result = result * base + digit;
        i++;
    }
    if (endptr != NULL)
    {
        *endptr = (char *) &str[i];
    }
    return sign * result;
}

unsigned long long int v_strtouq(const char *str, char **endptr, int base)
{
    unsigned long long int result = 0;
    int i = 0;
    if (base == 0)
    {
        if (str[i] == '0' && (str[i + 1] == 'x' || str[i + 1] == 'X'))
        {
            base = 16;
            i += 2;
        }
        else if (str[i] == '0')
        {
            base = 8;
            i++;
        }
        else
        {
            base = 10;
        }
    }
    while (str[i] != '\0')
    {
        int digit = 0;
        if (str[i] >= '0' && str[i] <= '9')
        {
            digit = str[i] - '0';
        }
        else if (str[i] >= 'a' && str[i] <= 'z')
        {
            digit = str[i] - 'a' + 10;
        }
        else if (str[i] >= 'A' && str[i] <= 'Z')
        {
            digit = str[i] - 'A' + 10;
        }
        else
        {
            break;
        }
        if (digit >= base)
        {
            break;
        }
        result = result * base + digit;
        i++;
    }
    if (endptr != NULL)
    {
        *endptr = (char *) &str[i];
    }
    return result;
}

long long int v_strtoll(const char *str, char **endptr, int base)
{
    return v_strtoq(str, endptr, base);
}

unsigned long long int v_strtoull(const char *str, char **endptr, int base)
{
    return v_strtouq(str, endptr, base);
}

#define _mul 1103515245
#define _ins 1

static int _seed_value = 0;

void v_srand(int seed)
{
    _seed_value = seed;
}

int v_rand(void)
{
    _seed_value = (_mul * _seed_value) + _ins;
    return _seed_value;
}