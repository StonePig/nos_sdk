/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  vstdlib.h
 *         \unit  vstdlib
 *        \brief  Similar to the C standard library stdlib
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __vstdlib_H
#define __vstdlib_H

#include "vstdint.h"

extern int v_atoi(const char *str);
extern long int v_atol(const char *str);
extern long long int v_atoll(const char *str);
extern double v_atof(const char *str);
extern float v_strtof(const char *str, char **endptr);
extern double v_strtod(const char *str, char **endptr);
extern long double v_strtold(const char *str, char **endptr);
extern float v_strtof16(const char *str, char **endptr);
extern float v_strtof32(const char *str, char **endptr);
extern double v_strtof64(const char *str, char **endptr);
extern long double v_strtof128(const char *str, char **endptr);
extern float v_strtof32x(const char *str, char **endptr);
extern double v_strtof64x(const char *str, char **endptr);
extern long double v_strtof128x(const char *str, char **endptr);
extern long int v_strtol(const char *str, char **endptr, int base);
extern unsigned long int v_strtoul(const char *str, char **endptr, int base);
extern long long int v_strtoq(const char *str, char **endptr, int base);
extern unsigned long long int v_strtouq(const char *str, char **endptr, int base);
extern long long int v_strtoll(const char *str, char **endptr, int base);
extern unsigned long long int v_strtoull(const char *str, char **endptr, int base);
extern void v_srand(int seed);
extern int v_rand(void);
extern void v_free(void *ptr);
extern void *v_malloc(size_t size);
extern void *v_calloc(size_t size);
extern void *v_realloc(void *ptr, size_t size);

#endif 
