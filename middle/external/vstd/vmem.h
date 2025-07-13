/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  vmem.h
 *         \unit  vmem
 *        \brief  Simple implementation of memory pool
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __vmem_H
#define __vmem_H

#include "vstdint.h"
#include "vstdlib.h"

/**< Memory block size is 32 bytes */
#define VMEM_BLOCK_SIZE                 (32)
/**< Size of memory pool */
#define VMEM_POOL_SIZE                  (40 * 1024) /* 40K */
/**< Size of memory management table */
#define VMEM_MTABLE_SIZE                (VMEM_POOL_SIZE / VMEM_BLOCK_SIZE)

/**
 *  \brief check dynamic memory usage
 *  \return usage rate (0 - 100) as (0% - 100%)
 */
uint8_t vmem_used(void);

#endif
