/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  heap.c
 *         \unit  heap
 *        \brief  This is a general C language heap container module
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "heap.h"
#include <string.h>

/* type of heap */
struct HEAP 
{
    void* base;                         /**< base address of heap */
    int dsize;                          /**< data size */
    int capacity;                       /**< capacity of base address */
    int size;                           /**< size of heap */
    heap_root_t root;                   /**< root type of heap, big or small */
} HEAP;

/* Address of heap array */
#define at(i)                           (((unsigned char *)(heap->base))+(i)*(heap->dsize))

/* Assign source data to target data */
#define assign(t, s)                    memcpy((t), (s), heap->dsize)

heap_t heap_create(int dsize, int capacity, heap_root_t root) 
{
    heap_t heap;

    /* Input value validity check */
    if (dsize <= 0) return NULL;
    if (capacity <= 0) return NULL;
    if (!root) return NULL;

    /* Allocate memory for the HEAP structure */
    heap = (heap_t)malloc(sizeof(HEAP));
    if (!heap) return NULL;

    /* Dynamically allocate an array */
    heap->base = malloc(dsize * capacity);
    if (!heap->base) 
    {
        free(heap);
        return NULL;
    }

    /* Initialize structural parameters */
    heap->size = 0;
    heap->dsize = dsize;
    heap->capacity = capacity;
    heap->root = root;

    return heap;
}

void heap_delete(heap_t heap) 
{
    /* Input value validity check */
    if (!heap) return;

    /* Release the allocated space */
    if (heap->base) free(heap->base);

    /* Free heap structure */
    free(heap);
}

static void swap(void *data0, void *data1, int size)
{
    char temp;
    int i;
    for (i = 0; i < size; i++)
    {
        temp = ((char *)data0)[i];
        ((char *)data0)[i] = ((char *)data1)[i];
        ((char *)data1)[i] = temp;
    }
}

int heap_push(heap_t heap, void *data) 
{
    int i, parent;

    /* Input value validity check */
    if (!heap) return 0;
    if (heap->size == heap->capacity) return 0;

    /* Get the current size of the heap */
    i = heap->size;

    /* Calculate the index of the parent node */
    parent = HEAP_PARENT(i);

    /* While i is not 0 and the current data is greater than its parent, 
     * assign the value of the parent to the current node and update i to the index of the parent node 
     */
    while (i != 0 && heap->root(data, at(parent)))
    {
        assign(at(i), at(parent));
        i = parent;
        parent = HEAP_PARENT(i);
    }

    /* Assign the data to the correct position */
    assign(at(i), data);

    /* Update the size of the heap */
    heap->size++;

    return 1;
}

int heap_pop(heap_t heap, void *data) 
{
    void *last;
    int i, child;

    /* Input value validity check */
    if (!heap) return 0;
    if (heap->size == 0) return 0;

    /* If data is provided, assign the top element of the heap to data */
    if (data) assign(data, at(0));

    /* If data is provided, assign the top element of the heap to data */
    last = at(heap->size - 1);

    /* Initialize variables for parent and child nodes */
    i = 0, child = 1;

    while (child < heap->size) 
    {
        /* If the right child is smaller, move to the right child */
        if (child < heap->size - 1 && heap->root(at(child + 1), at(child))) child++;

        /* If the parent node follow the rule with both nodes */
        if (heap->root(at(child), last))
        {
            /* Swap the parent and child nodes */
            assign(at(i), at(child)); 

            i = child;

            /* Move to the left child */
            child = HEAP_LEFT(child); 
        }
        else break; 
    }

    /* Assign the last element to the parent node */
    assign(at(i), last);

    /* Decrease the size of the heap */
    heap->size--;

    return 1;
}

int heap_modify(heap_t heap, int index, void *data) 
{
    int parent;

    /* Input value validity check */
    if (!heap) return 0;
    if (!data) return 0;
    if (index < 0 || index >= heap->size) return 0;

    if (heap->root(data, at(index)))
    {
        /* Assign the data to the specified index position */
        assign(at(index), data);

        /* Get the parent node of the specified index position */
        parent = HEAP_PARENT(index);
        while (index > 0 && heap->root(at(index), at(parent)))
        {
            /* Swap the parent and child nodes */
            swap(at(index), at(parent), heap->dsize); 

            index = parent;

            /* Get the parent node of the current index position */
            parent = HEAP_PARENT(index);
        }
    } 
    else if (heap->root(at(index), data))
    {
        int i = index, child = HEAP_LEFT(index); /* nitialize variables for parent and child nodes */

        /* Assign the data to the specified index position */
        assign(at(index), data);
        while (child < heap->size) 
        {
            /* If the right child is smaller, move to the right child */
            if (child < heap->size - 1 && heap->root(at(child + 1), at(child))) child++;

            if (heap->root(at(child), data))
            {
                /* Swap the parent and child nodes */
                assign(at(i), at(child));

                i = child;

                /* Move to the left child */
                child = HEAP_LEFT(child);
            } 
            else break;
        }

        /* Assign the data to the last parent node */
        assign(at(i), data);
    }

    return 1;
}

int heap_top(heap_t heap, void *data) 
{
    /* Input value validity check */
    if (!heap) return 0;
    if (!data) return 0;
    if (heap->size == 0) return 0;

    /* Copy data out */
    assign(data, at(0));

    return 1;
}

int heap_size(heap_t heap) 
{
    /* Input value validity check */
    if (!heap) return 0;

    /* Return heap size */
    return heap->size;
}
