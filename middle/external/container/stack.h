/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  stack.h
 *         \unit  stack
 *        \brief  This is a C language stack
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __stack_H
#define __stack_H

#include <stdlib.h>

/* version infomation */

#define STACK_V_MAJOR                       1
#define STACK_V_MINOR                       0
#define STACK_V_PATCH                       0

/* stack type definition, hiding structural members, not for external use */

typedef struct STACK *stack_t;

/**
 *  \brief create a stack.
 *  \param[in] dsize: size of stack data
 *  \param[in] capacity: capacity of stack
 *  \param[in] base: allocated array or pass in `NULL` to dynamically allocate space
 *  \return stack handler or NULL fail
 */
stack_t stack_create(int dsize, int capacity, void *base);

/**
 *  \brief delete a stack.
 *  \param[in] stack: stack handler
 *  \return none
 */
void stack_delete(stack_t stack);

/**
 *  \brief push data into the stack.
 *  \param[in] stack: address of stack
 *  \param[in] data: the address of data
 *  \return 1 success or 0 fail
 */
int stack_push(stack_t stack, void* data);

/**
 *  \brief pop data from the stack.
 *  \param[in] stack: address of stack
 *  \param[out] data: the address of data
 *  \return 1 success or 0 fail
 */
int stack_pop(stack_t stack, void* data);

/**
 *  \brief clear stack.
 *  \param[in] stack: address of stack
 *  \return none
 */
void stack_clear(stack_t stack);

/**
 *  \brief get stack index.
 *  \param[in] stack: address of stack
 *  \param[in] index: index of stack
 *  \return index of stack buffer or negative fail
 */
int stack_index(stack_t stack, int index);

/**
 *  \brief get data address of stack.
 *  \param[in] stack: stack handler
 *  \param[in] index: index
 *  \return address of stack data or NULL fail
 */
void* stack_data(stack_t stack, int index);

/**
 *  \brief get size of stack.
 *  \param[in] stack: stack handler
 *  \return size of stack
 */
int stack_size(stack_t stack);

/**
 *  \brief get capacity of stack.
 *  \param[in] stack: stack handler
 *  \return capacity of stack
 */
int stack_capacity(stack_t stack);

/**
 *  \brief get data size of stack.
 *  \param[in] stack: stack handler
 *  \return data size of stack
 */
int stack_dsize(stack_t stack);

/**
 *  \brief check if empty.
 *  \param[in] stack: stack handler
 *  \return 1 empty or 0 not empty
 */
int stack_empty(stack_t stack);

/**
 *  \brief check if full.
 *  \param[in] stack: stack handler
 *  \return 1 full or 0 not full
 */
int stack_full(stack_t stack);

/**
 *  \brief A simple method for `stack_create`.
 *  \param[in] type: data type
 *  \param[in] capacity: capacity of stack
 *  \return stack handler or NULL fail
 */
#define stack(type, capacity)               stack_create(sizeof(type), (capacity), NULL)

/**
 *  \brief A simple method for `stack_delete`.
 *  \param[in] stack: stack handler
 *  \return none
 */
#define _stack(stack)                       do{stack_delete(stack);(stack)=NULL;}while(0)

/**
 *  \brief Random access method for stack data.
 *  \param[in] stack: stack handler
 *  \param[in] type: data type
 *  \param[in] i: index starting from stack header
 *  \return Reference to stack data
 */
#define stack_at(stack, type, i)            (*(type *)stack_data((stack), (i)))

#endif
