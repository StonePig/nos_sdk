/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  vector.c
 *         \unit  vector
 *        \brief  This is a C language vector
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "vector.h"
#include <string.h>

/* type of vector */
typedef struct VECTOR
{
    void* base;                     /**< base address for storing data */
    int dsize;                      /**< size of item */
    int size;                       /**< size of vector */
    int capacity;                   /**< capacity of vector */
} VECTOR;

/* get the smallest 'mul' multiple larger than 'x' */
#define up_multiple(x, mul)         ((x)+((mul)-((x)-1)%(mul))-1)

/* address of void array */
#define at(i)                       (((unsigned char *)(vector->base))+(i)*(vector->dsize)) 

/** 
 *  \brief calculate capacity according to gradient
 *  \param[in] size: size
 *  \return capacity
 */
static int gradient_capacity(int size)
{
    int capacity = 1;
    if (size <= 1) return 1;
    while (capacity < size) capacity <<= 1;
    capacity >>= 1;
    if (capacity < 4) capacity = capacity << 1;
    else if (capacity < 16) capacity = up_multiple(size, capacity >> 1);
    else if (capacity < 256) capacity = up_multiple(size, capacity >> 2);
    else capacity = up_multiple(size, 64);
    return capacity;
}

vector_t vector_create(int dsize, int size)
{
    vector_t vector;
    int capacity;

    /* Input value validity check */
    if (dsize <= 0 || size < 0) return NULL;

    /* Calculate the capacity required for the vector to meet size requirements */
    capacity = gradient_capacity(size);

    /* Allocate memory for the LIST structure */
    vector = (vector_t)malloc(sizeof(VECTOR));
    if (!vector) return NULL;

    /* Allocate memory for the array */
    vector->base = malloc(dsize * capacity);
    if (!vector->base)
    {
        free(vector);
        return NULL;
    }

    /* Initialize structural parameters */
    vector->dsize = dsize;
    vector->size = size;
    vector->capacity = capacity;

    return vector;
}

void vector_delete(vector_t vector)
{
    /* Input value validity check */
    if (!vector) return;

    /* Free array */
    free(vector->base);

    /* Free list structure */
    free(vector);
}

void* vector_data(vector_t vector, int index)
{
    /* Input value validity check */
    if (!vector) return NULL;
    if (index < 0 || index >= vector->size) return NULL;

    /* Return address of index data */
    return (void *)at(index);
}

int vector_size(vector_t vector)
{
    /* Input value validity check */
    if (!vector) return 0;

    /* Return list size */
    return vector->size;
}

int vector_capacity(vector_t vector)
{
    /* Input value validity check */
    if (!vector) return 0;

    /* Return list capacity */
    return vector->capacity;
}

int vector_resize(vector_t vector, int size)
{
    void* base = NULL;
    int capacity;

    /* Input value validity check */
    if (!vector) return 0;
    if (size < 0) return 0;

    /* Calculate the capacity required for the vector to meet size requirements */
    capacity = gradient_capacity(size);

    /* If the capacity changes, reallocate the capacity */
    if (capacity != vector->capacity)
    {
        base = realloc(vector->base, capacity * vector->dsize);
        if (!base) return 0;
        vector->base = base;
        vector->capacity = capacity;
    }

    /* Update list status */
    vector->size = size;

    return 1;
}

int vector_insert(vector_t vector, int index, void* data, int num)
{
    int i = 0;
    int size;

    /* Input value validity check */
    if (!vector) return 0;
    if (index < 0 || index > vector->size) return 0;
    if (num <= 0) return 0;

    /* Record the original size */
    size = vector->size;

    /* Reassign space and expand capacity */
    if (!vector_resize(vector, vector->size + num)) return 0;

    /* Move the data at the insertion position back */
    if (index < size) memmove(at(index + num), at(index), vector->dsize * (size - index));
    
    /* Assigning data to the list */
    if (data) memcpy(at(index + i), data, vector->dsize * num);

    return 1;
}

int vector_erase(vector_t vector, int index, int num)
{
    unsigned char *op_ptr = NULL;

    /* Input value validity check */
    if (!vector) return 0;
    if (vector->size == 0) return 0;
    if (index < 0 || index >= vector->size) return 0;
    if (num <= 0) return 0;

    /* Correct the number of erases to be made */
    if (num > vector->size - index) num = vector->size - index;

    /* Move the data at the insertion position front */
    memmove(at(index), at(index + num), vector->dsize * (vector->size - (index + num)));
    
    /* Reassign space and reduce capacity */
    vector_resize(vector, vector->size - num);

    return 1;
}
