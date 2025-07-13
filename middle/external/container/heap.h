/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  heap.h
 *         \unit  heap
 *        \brief  This is a general C language heap container module
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __heap_H
#define __heap_H

#include <stdlib.h>

/* version infomation */

#define HEAP_V_MAJOR                        1
#define HEAP_V_MINOR                        0
#define HEAP_V_PATCH                        0

/* heap type definition, hiding structural members, not for external use */

typedef struct HEAP *heap_t;

/* big root: parent > child return 1, small root: parent < child return 1. 
 * Follow the rules to return 1, otherwise return 0.
 */
typedef int (*heap_root_t)(void *parent, void *child);

/**
 *  \brief create heap
 *  \param[in] dsize: data size of heap item
 *  \param[in] capacity: capacity of heap base
 *  \param[in] root: root type of heap, big or small
 *  \return handler of new heap
 */
heap_t heap_create(int dsize, int capacity, heap_root_t root);

/**
 *  \brief delete heap
 *  \param[in] heap: heap handle
 *  \return none
 */
void heap_delete(heap_t heap);

/**
 *  \brief push data into heap
 *  \param[in] heap: heap handle
 *  \param[in] data: address of data
 *  \return 1 success or 0 fail
 */
int heap_push(heap_t heap, void *data);

/**
 *  \brief pop data from heap
 *  \param[in] heap: heap handle
 *  \param[in] data: address of data
 *  \return 1 success or 0 fail
 */
int heap_pop(heap_t heap, void *data);

/**
 *  \brief modify data of heap
 *  \param[in] heap: heap handle
 *  \param[in] index: index of heap, start from 0
 *  \param[in] data: address of data
 *  \return 1 success or 0 fail
 */
int heap_modify(heap_t heap, int index, void *data);

/**
 *  \brief get data of heap
 *  \param[in] heap: heap handle
 *  \param[in] data: address of data
 *  \return 1 success or 0 fail
 */
int heap_top(heap_t heap, void *data);

/**
 *  \brief get the size of heap
 *  \param[in] heap: heap handle
 *  \return size of heap
 */
int heap_size(heap_t heap);

/**
 *  \brief get the index of the parent node
 *  \param[in] i: current node index
 *  \return parent index
 */
#define HEAP_PARENT(i)                      (((i)-1)>>1)

/**
 *  \brief get the index of the left child node
 *  \param[in] i: current node index
 *  \return left child index
 */
#define HEAP_LEFT(i)                        (((i)<<1)+1)

/**
 *  \brief get the index of the right child node
 *  \param[in] i: current node index
 *  \return right child index
 */
#define HEAP_RIGHT(i)                       (((i)<<1)+2)

#endif
