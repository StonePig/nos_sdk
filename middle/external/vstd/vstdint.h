/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  vsdint.h
 *         \unit  vsdint
 *        \brief  Similar to the C standard library sdint
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __vsdint_H
#define __vsdint_H

#include "vstddef.h"

typedef signed char         int8_t;
typedef unsigned char       uint8_t;
typedef short               int16_t;
typedef unsigned short      uint16_t;
typedef int                 int32_t;
typedef unsigned            uint32_t;
typedef long long           int64_t;
typedef unsigned long long  uint64_t;

typedef signed char         int_least8_t;
typedef unsigned char       uint_least8_t;
typedef short               int_least16_t;
typedef unsigned short      uint_least16_t;
typedef int                 int_least32_t;
typedef unsigned            uint_least32_t;
typedef long long           int_least64_t;
typedef unsigned long long  uint_least64_t;

typedef signed char         int_fast8_t;
typedef unsigned char       uint_fast8_t;
typedef short               int_fast16_t;
typedef unsigned short      uint_fast16_t;
typedef int                 int_fast32_t;
typedef unsigned  int       uint_fast32_t;
typedef long long           int_fast64_t;
typedef unsigned long long  uint_fast64_t;

#define INT8_MIN            (-128)
#define INT16_MIN           (-32768)
#define INT32_MIN           (-2147483647 - 1)
#define INT64_MIN           (-9223372036854775807LL - 1)

#define INT8_MAX            127
#define INT16_MAX           32767
#define INT32_MAX           2147483647
#define INT64_MAX           9223372036854775807LL

#define UINT8_MAX           0xffU                   /* 255U */
#define UINT16_MAX          0xffffU                 /* 65535U */
#define UINT32_MAX          0xffffffffUL            /* 4294967295U */
#define UINT64_MAX          0xffffffffffffffffULL   /* 18446744073709551615ULL */

#define INT_LEAST8_MIN      INT8_MIN
#define INT_LEAST16_MIN     INT16_MIN
#define INT_LEAST32_MIN     INT32_MIN
#define INT_LEAST64_MIN     INT64_MIN

#define INT_LEAST8_MAX      INT8_MAX
#define INT_LEAST16_MAX     INT16_MAX
#define INT_LEAST32_MAX     INT32_MAX
#define INT_LEAST64_MAX     INT64_MAX

#define UINT_LEAST8_MAX     UINT8_MAX
#define UINT_LEAST16_MAX    UINT16_MAX
#define UINT_LEAST32_MAX    UINT32_MAX
#define UINT_LEAST64_MAX    UINT64_MAX

#define INT_FAST8_MIN       INT8_MIN
#define INT_FAST16_MIN      INT16_MIN
#define INT_FAST32_MIN      INT32_MIN
#define INT_FAST64_MIN      INT64_MIN

#define INT_FAST8_MAX       INT8_MAX
#define INT_FAST16_MAX      INT16_MAX
#define INT_FAST32_MAX      INT32_MAX
#define INT_FAST64_MAX      INT64_MAX

#define UINT_FAST8_MAX      UINT8_MAX
#define UINT_FAST16_MAX     UINT16_MAX
#define UINT_FAST32_MAX     UINT32_MAX
#define UINT_FAST64_MAX     UINT64_MAX

#ifdef _WIN64
# define INTPTR_MIN         INT64_MIN
# define INTPTR_MAX         INT64_MAX
# define UINTPTR_MAX        UINT64_MAX
#else
# define INTPTR_MIN         INT32_MIN
# define INTPTR_MAX         INT32_MAX
# define UINTPTR_MAX        UINT32_MAX
#endif

#define INTMAX_MIN          INT64_MIN
#define INTMAX_MAX          INT64_MAX
#define UINTMAX_MAX         UINT64_MAX

#define PTRDIFF_MIN         INTPTR_MIN
#define PTRDIFF_MAX         INTPTR_MAX

#define SIG_ATOMIC_MIN      INTPTR_MIN
#define SIG_ATOMIC_MAX      INTPTR_MAX

#define SIZE_MAX            UINTPTR_MAX

#define WCHAR_MIN           0
#define WCHAR_MAX           0xffff /* UINT16_MAX */

#define WINT_MIN            0
#define WINT_MAX            0xffff /* UINT16_MAX */

#define INT8_C(val)         val
#define UINT8_C(val)        val
#define INT16_C(val)        val
#define UINT16_C(val)       val

#define INT32_C(val)        val
#define UINT32_C(val)       val##U
#define INT64_C(val)        val##LL
#define UINT64_C(val)       val##ULL

#define INTMAX_C(val)       INT64_C(val)
#define UINTMAX_C(val)      UINT64_C(val)

#endif
