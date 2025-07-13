/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  stack.c
 *         \unit  stack
 *        \brief  This is a C language stack
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "stack.h"
#include <string.h>

typedef struct STACK
{
    void* base;                         /**< base address of data */
    int cst;                            /**< base const */
    int dsize;                          /**< size of stack data */
    int capacity;                       /**< capacity of stack */
    int top;                            /**< index of stack top */
} STACK;

/* Address of stack array */
#define at(i)                           (((unsigned char *)(stack->base))+(i)*(stack->dsize))

stack_t stack_create(int dsize, int capacity, void *base)
{
    stack_t stack;

    /* Input value validity check */
    if (dsize <= 0) return NULL;
    if (capacity <= 0) return NULL;

    /* Allocate memory for the STACK structure */
    stack = (stack_t)malloc(sizeof(STACK));
    if (!stack) return NULL;

    /* Initialize structural parameters */
    stack->base = base;
    stack->cst = 1;
    stack->capacity = capacity;
    stack->dsize = dsize;
    stack->top = 0;

    /* Dynamically allocate an array without passing it in */
    if (!stack->base) 
    {
        stack->base = malloc(dsize * capacity); 
        stack->cst = 0;
    }

    /* Check if the array space is valid */
    if (!stack->base) 
    {
        stack_delete(stack);
        return NULL;
    }
    
    return stack;
}

void stack_delete(stack_t stack)
{
    /* Input value validity check */
    if (!stack) return;

    /* If it is not a constant array but a dynamic array, release the allocated space */
    if (!stack->cst && stack->base) free(stack->base);

    /* Free stack structure */
    free(stack);
}

int stack_push(stack_t stack, void* data)
{
    /* Input value validity check */
    if (!stack) return 0;

    /* Check if the stack is full */
    if (stack_full(stack)) return 0;

    /* Assigning data to the stack */
    if (data) memcpy(at(stack->top), data, stack->dsize);

    /* Update stack status */
    stack->top++;

    return 1;
}

int stack_pop(stack_t stack, void* data)
{
    /* Input value validity check */
    if (!stack) return 0;

    /* Check if the stack is full */
    if (stack_empty(stack)) return 0;

    /* Update stack status */
    stack->top--;

    /* Assigning data from the stack */
    if (data) memcpy(data, at(stack->top), stack->dsize);

    return 1;
}

void stack_clear(stack_t stack)
{
    /* Input value validity check */
    if (!stack) return;

    /* Reset stack status */
    stack->top = 0;
}

int stack_index(stack_t stack, int index)
{
    /* Input value validity check */
    if (!stack) return -1;
    if (index < 0 || index >= stack->top) return -1;

    /* Starting from the bottom, calculate the data index */
    return index;
}

void* stack_data(stack_t stack, int index)
{
    /* Input value validity check */
    if (!stack) return NULL;
    if (index < 0 || index >= stack->top) return NULL;

    /* Return array address based on index */
    return (void*)at(index);
}

int stack_size(stack_t stack)
{
    /* Input value validity check */
    if (!stack) return 0;

    /* Return stack size */
    return stack->top;
}

int stack_capacity(stack_t stack)
{
    /* Input value validity check */
    if (!stack) return 0;

    /* Return stack capacity */
    return stack->capacity;
}

int stack_dsize(stack_t stack)
{
    /* Input value validity check */
    if (!stack) return 0;

    /* Return stack data size */
    return stack->dsize;
}

int stack_empty(stack_t stack)
{
    /* Input value validity check */
    if (!stack) return 1;

    /* Determine if size is 0 */
    return (stack->top == 0) ? 1 : 0;
}

int stack_full(stack_t stack)
{
    /* Input value validity check */
    if (!stack) return 0;

    /* Determine if size is capacity */
    return (stack->top == stack->capacity) ? 1 : 0;
}