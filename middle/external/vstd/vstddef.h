/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  vstddef.h
 *         \unit  vstddef
 *        \brief  Similar to the C standard library stddef
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __vstddef_H
#define __vstddef_H

/* define size_t */
#ifndef size_t
typedef unsigned long size_t;
#endif 

/* define ptrdiff_t */
#ifndef ptrdiff_t
typedef long ptrdiff_t;
#endif 

/* define wchar_t */
#ifndef wchar_t
typedef int wchar_t;
#endif 

/* define NULL */
#ifndef NULL
#define NULL ((void*)0)
#endif

/* define offsetof */
#ifndef offsetof
#ifdef _WIN64
#define offsetof(s,m) (size_t)( (ptrdiff_t)&(((s *)0)->m) )
#else
#define offsetof(s,m) (size_t)&(((s *)0)->m)
#endif
#endif 

#endif 
