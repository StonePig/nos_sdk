/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  valloc.h
 *         \unit  valloc
 *        \brief  Test how much space is allocated
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __valloc_H
#define __valloc_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>

/* Version infomation */

#define VALLOC_V_MAJOR                      1
#define VALLOC_V_MINOR                      0
#define VALLOC_V_PATCH                      0

/* These methods are generally not directly called, but use versions that are reinstalled through macro definitions */

void* vm_malloc(size_t size, char *file, int line);
void* vm_calloc(size_t num, size_t size, char *file, int line);
void vm_free(void* block);
void* vm_realloc(void* block, size_t size, char *file, int line);

/* Statistics on memory usage */

/**
 *  \brief List memory blocks that have been allocated but not yet free
 *         This method will provide a detailed list of memory block addresses and sizes, and indicate which file and line to allocate them in
 *  \return none
 */
void v_check_unfree(void);

/**
 *  \brief Count how many blocks of memory are currently allocated
 *  \return count of memory
 */
int v_check_count(void);

/**
 *  \brief Count how much memory is currently allocated and used in total
 *  \return total size of memory
 */
int v_check_used(void);

/* These memory APIs, named the same as those provided by stdlib, 
 * are intended to replace the method of Valloc in stdlib to track memory usage.
 * 
 * When using, simply include the `valloc.h` header file after `stdlib.h`, 
 * and the API of `stdlib.h` will be replaced by the API of `valloc.h`
 * Without the need for large-scale function replacement.
 */

#define malloc(s)                           vm_malloc(s, __FILE__, __LINE__)
#define calloc(n, s)                        vm_calloc(n, s, __FILE__, __LINE__)
#define free(b)                             vm_free(b)
#define realloc(b, s)                       vm_realloc(b, s, __FILE__, __LINE__)

#ifdef __cplusplus
}
#endif

#endif