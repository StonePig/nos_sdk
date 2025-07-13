/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  queue.c
 *         \unit  queue
 *        \brief  This is a C language queue
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "queue.h"
#include <string.h>

typedef struct QUEUE
{
    void* base;                         /**< base address of data */
    int cst;                            /**< base const */
    int dsize;                          /**< size of queue data */
    int capacity;                       /**< capacity of queue */
    int size;                           /**< size of queue */
    int head;                           /**< index of queue head */
    int tail;                           /**< index of queue tail */
} QUEUE;

/* Address of queue array */
#define at(i)                           (((unsigned char *)(queue->base))+(i)*(queue->dsize)) 

queue_t queue_create(int dsize, int capacity, void *base)
{
    queue_t queue;

    /* Input value validity check */
    if (dsize <= 0) return NULL;
    if (capacity <= 0) return NULL;

    /* Allocate memory for the QUEUE structure */
    queue = (queue_t)malloc(sizeof(QUEUE));
    if (!queue) return NULL;

    /* Initialize structural parameters */
    queue->base = base;
    queue->cst = 1;
    queue->capacity = capacity;
    queue->dsize = dsize;
    queue->tail = 0;
    queue->head = 0;
    queue->size = 0;

    /* Dynamically allocate an array without passing it in */
    if (!queue->base) 
    {
        queue->base = malloc(dsize * capacity); 
        queue->cst = 0;
    }

    /* Check if the array space is valid */
    if (!queue->base) 
    {
        queue_delete(queue); 
        return NULL;
    }

    return queue;
}

void queue_delete(queue_t queue)
{
    /* Input value validity check */
    if (!queue) return;

    /* If it is not a constant array but a dynamic array, release the allocated space */
    if (!queue->cst && queue->base) free(queue->base);

    /* Free queue structure */
    free(queue);
}

int queue_push(queue_t queue, void* data)
{
    /* Input value validity check */
    if (!queue) return 0;

    /* Check if the queue is full */
    if (queue_full(queue)) return 0;

    /* Assigning data to the queue */
    if (data) memcpy(at(queue->tail), data, queue->dsize);

    /* Update queue status */
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->size++;

    return 1;
}

int queue_pop(queue_t queue, void* data)
{
    /* Input value validity check */
    if (!queue) return 0;

    /* Check if the queue is full */
    if (queue_empty(queue)) return 0;

    /* Assigning data from the queue */
    if (data) memcpy(data, at(queue->head), queue->dsize);

    /* Update queue status */
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size--;

    return 1;
}

void queue_clear(queue_t queue)
{
    /* Input value validity check */
    if (!queue) return;

    /* Reset queue status */
    queue->tail = 0;
    queue->head = 0;
    queue->size = 0;
}

int queue_index(queue_t queue, int index)
{
    /* Input value validity check */
    if (!queue) return -1;
    if (index < 0 || index >= queue->size) return -1;

    /* Starting from the head, calculate the data index */
    return (queue->head + index) % (queue->capacity);
}

void* queue_data(queue_t queue, int index)
{
    /* Input value validity check */
    if (!queue) return NULL;

    /* Get indexe for accessing data */
    index = queue_index(queue, index);
    if (index < 0) return NULL;

    /* Return array address based on index */
    return (void*)at(index);
}

int queue_size(queue_t queue)
{
    /* Input value validity check */
    if (!queue) return 0;

    /* Return queue size */
    return queue->size;
}

int queue_capacity(queue_t queue)
{
    /* Input value validity check */
    if (!queue) return 0;

    /* Return queue capacity */
    return queue->capacity;
}

int queue_dsize(queue_t queue)
{
    /* Input value validity check */
    if (!queue) return 0;

    /* Return queue data size */
    return queue->dsize;
}

int queue_empty(queue_t queue)
{
    /* Input value validity check */
    if (!queue) return 1;

    /* Determine if size is 0 */
    return (queue->size == 0) ? 1 : 0;
}

int queue_full(queue_t queue)
{
    /* Input value validity check */
    if (!queue) return 0;

    /* Determine if size is capacity */
    return (queue->size == queue->capacity) ? 1 : 0;
}
