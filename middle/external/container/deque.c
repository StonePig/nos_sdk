/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  deque.c
 *         \unit  deque
 *        \brief  This is a C language deque
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "deque.h"
#include <string.h>

/* type of deque */
typedef struct DEQUE
{
    void* base;                         /**< base address of data */
    int cst;                            /**< base const */
    int dsize;                          /**< size of deque data */
    int capacity;                       /**< capacity of deque */
    int size;                           /**< size of deque */
    int head;                           /**< index of deque head */
    int tail;                           /**< index of deque tail */
} DEQUE;

/* Address of deque array */
#define at(i)                           (((unsigned char *)(deque->base))+(i)*(deque->dsize))

deque_t deque_create(int dsize, int capacity, void *base)
{
    deque_t deque;

    /* Input value validity check */
    if (dsize <= 0) return NULL;
    if (capacity <= 0) return NULL;

    /* Allocate memory for the DEQUE structure */
    deque = (deque_t)malloc(sizeof(DEQUE));
    if (!deque) return NULL;

    /* Initialize structural parameters */
    deque->base = base;
    deque->cst = 1;
    deque->capacity = capacity;
    deque->dsize = dsize;
    deque->tail = 0;
    deque->head = 0;
    deque->size = 0;

    /* Dynamically allocate an array without passing it in */
    if (!deque->base) 
    {
        deque->base = malloc(dsize * capacity); 
        deque->cst = 0;
    }

    /* Check if the array space is valid */
    if (!deque->base) 
    {
        deque_delete(deque); 
        return NULL;
    }
    
    return deque;
}

void deque_delete(deque_t deque)
{
    /* Input value validity check */
    if (!deque) return;

    /* If it is not a constant array but a dynamic array, release the allocated space */
    if (!deque->cst && deque->base) free(deque->base);

    /* Free deque structure */
    free(deque);
}

int deque_push_front(deque_t deque, void* data)
{
    /* Input value validity check */
    if (!deque) return 0;

    /* Check if the deque is full */
    if (deque_full(deque)) return 0;

    /* Update deque status */
    deque->head = (deque->head + deque->capacity - 1) % deque->capacity;
    deque->size++;

    /* Assigning data to the deque */
    if (data) memcpy(at(deque->head), data, deque->dsize);
    
    return 1;
}

int deque_push_back(deque_t deque, void* data)
{
    /* Input value validity check */
    if (!deque) return 0;

    /* Check if the deque is full */
    if (deque_full(deque)) return 0;

    /* Assigning data to the deque */
    if (data) memcpy(at(deque->tail), data, deque->dsize);

    /* Update deque status */
    deque->tail = (deque->tail + 1) % deque->capacity;
    deque->size++;

    return 1;
}

int deque_pop_front(deque_t deque, void* data)
{
    /* Input value validity check */
    if (!deque) return 0;

    /* Check if the deque is full */
    if (deque_empty(deque)) return 0;

    /* Assigning data from the deque */
    if (data) memcpy(data, at(deque->head), deque->dsize);

    /* Update deque status */
    deque->head = (deque->head + 1) % deque->capacity;
    deque->size--;

    return 1;
}

int deque_pop_back(deque_t deque, void* data)
{
    /* Input value validity check */
    if (!deque) return 0;

    /* Check if the deque is full */
    if (deque_empty(deque)) return 0;

    /* Update deque status */
    deque->tail = (deque->tail + deque->capacity - 1) % deque->capacity;
    deque->size--;

    /* Assigning data from the deque */
    if (data) memcpy(data, at(deque->tail), deque->dsize);
    
    return 1;
}

void deque_clear(deque_t deque)
{
    /* Input value validity check */
    if (!deque) return;

    /* Reset deque status */
    deque->tail = 0;
    deque->head = 0;
    deque->size = 0;
}

int deque_index(deque_t deque, int index)
{
    /* Input value validity check */
    if (!deque) return -1;
    if (index < 0 || index >= deque->size) return -1;

    /* Starting from the head, calculate the data index */
    return (deque->head + index) % (deque->capacity);
}

void* deque_data(deque_t deque, int index)
{
    /* Input value validity check */
    if (!deque) return NULL;

    /* Get indexe for accessing data */
    index = deque_index(deque, index);
    if (index < 0) return NULL;

    /* Return array address based on index */
    return (void*)at(index);
}

int deque_size(deque_t deque)
{
    /* Input value validity check */
    if (!deque) return 0;

    /* Return deque size */
    return deque->size;
}

int deque_capacity(deque_t deque)
{
    /* Input value validity check */
    if (!deque) return 0;

    /* Return deque capacity */
    return deque->capacity;
}

int deque_dsize(deque_t deque)
{
    /* Input value validity check */
    if (!deque) return 0;

    /* Return deque data size */
    return deque->dsize;
}

int deque_empty(deque_t deque)
{
    /* Input value validity check */
    if (!deque) return 1;

    /* Determine if size is 0 */
    return (deque->size == 0) ? 1 : 0;
}

int deque_full(deque_t deque)
{
    /* Input value validity check */
    if (!deque) return 0;

    /* Determine if size is capacity */
    return (deque->size == deque->capacity) ? 1 : 0;
}
