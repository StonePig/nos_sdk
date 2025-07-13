/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  vmem.c
 *         \unit  vmem
 *        \brief  Simple implementation of memory pool
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "vmem.h"
#include "vstring.h"

/**< Memory pool */
static uint8_t mem_pool_base[VMEM_POOL_SIZE] = {0};
/**< Memory management table */
static uint16_t mem_map_base[VMEM_MTABLE_SIZE] = {0};
/**< Memory table size */
static const uint32_t mem_table_size = VMEM_MTABLE_SIZE;
/**< Memory chunking size */
static const uint32_t mem_block_size = VMEM_BLOCK_SIZE;
/**< Memory pool size */
static const uint32_t mem_pool_size = VMEM_POOL_SIZE;

uint8_t vmem_used(void)
{
    uint32_t used = 0;
    uint32_t i;
    for (i = 0; i < mem_table_size; i++)
    {
        if(mem_map_base[i])used++;
    }
    return (used*100)/(mem_table_size);
}

/**
 *  \brief memory allocation function for internal calls
 *  \param[in] size: memory size to allocate (in bytes)
 *  \return memory offset address or 0xFFFFFFFF failed
 */
static uint32_t memory_alloc(uint32_t size)
{
    signed long offset=0;
    uint32_t nmemb; /* Number of memory blocks required */
    uint32_t cmemb = 0; /* Number of contiguous empty memory blocks */
    uint32_t i;

    /* No allocation required */
    if (size == 0) return 0xFFFFFFFF;

    /* Obtain the number of contiguous memory blocks that need to be allocated */
    nmemb = size / mem_block_size;

    /* Round up to obtain enough memory block space */
    if (size % mem_block_size) nmemb++;

    /* Search the entire memory control area */
    for (offset = mem_table_size - 1; offset >= 0; offset--)
    {
        /* Continuously increasing the number of empty memory blocks */
        if (!mem_map_base[offset]) cmemb++;
        /* Zeroing Continuous Memory Blocks */
        else cmemb = 0;

        /* Found consecutive nmemb empty memory blocks */
        if (cmemb == nmemb)
        {
            /* Mark memory block is not empty */
            for (i = 0; i < nmemb; i++)
            {
                mem_map_base[offset + i] = nmemb;
            }

            /* Return offset address */
            return (offset * mem_block_size);
        }
    }

    return 0xFFFFFFFF;
}

/**
 *  \brief memory free function for internal calls
 *  \param[in] offset: memory address offset
 *  \return 0 success or others fail
 */ 
static uint8_t memory_free(uint32_t offset)
{
    int index, nmemb, i;

    /* Offset within memory pool */
    if (offset < mem_pool_size)
    {
        index = offset / mem_block_size; /* Offset memory block number */
        nmemb = mem_map_base[index]; /* Number of memory blocks */

        /* Memory Block Zeroing */
        for (i = 0; i < nmemb; i++)
        {
            mem_map_base[index + i] = 0;
        }

        return 0;
    }
    /* Offset not within memory pool */
    else return 2;
}

void v_free(void *ptr)
{
    uint32_t offset;

    if (ptr == NULL) return;

    offset = (uint32_t)ptr - (uint32_t)mem_pool_base;

    memory_free(offset);
}

void *v_malloc(size_t size)
{
    uint32_t offset;

    offset = memory_alloc(size);

    if (offset == 0xFFFFFFFF) return NULL;

    return (void *)((uint32_t)mem_pool_base + offset);
}

void *v_realloc(void *ptr, size_t size)
{
    uint32_t offset;
    offset = memory_alloc(size);

    if (offset == 0xFFFFFFFF) return NULL;

    /* Copy old memory content to new memory */
    v_memcpy((void *)((uint32_t)mem_pool_base + offset), ptr, size);

    /* Release old memory */
    v_free(ptr);

    return (void *)((uint32_t)mem_pool_base + offset);
}

void *v_calloc(size_t size)
{
    void *ptr;

    ptr = v_malloc(size);

    if (ptr) v_memset(ptr, 0, size);

    return ptr;
}
