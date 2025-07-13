/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  deque.h
 *         \unit  deque
 *        \brief  This is a C language deque
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __deque_H
#define __deque_H

#include <stdlib.h>

/* version infomation */

#define DEQUE_V_MAJOR                       1
#define DEQUE_V_MINOR                       0
#define DEQUE_V_PATCH                       0

/* deque type definition, hiding structural members, not for external use */

typedef struct DEQUE *deque_t;

/**
 *  \brief create a deque.
 *  \param[in] dsize: size of deque data
 *  \param[in] capacity: capacity of deque
 *  \param[in] base: allocated array or pass in `NULL` to dynamically allocate space
 *  \return deque handler or NULL fail
 */
deque_t deque_create(int dsize, int capacity, void *base);

/**
 *  \brief delete a deque.
 *  \param[in] deque: deque handler
 *  \return none
 */
void deque_delete(deque_t deque);

/**
 *  \brief push data into the deque from the front.
 *  \param[in] deque: address of deque
 *  \param[in] data: the address of data
 *  \return 1 success or 0 fail
 */
int deque_push_front(deque_t deque, void* data);

/**
 *  \brief push data into the deque from the back.
 *  \param[in] deque: address of deque
 *  \param[in] data: the address of data
 *  \return 1 success or 0 fail
 */
int deque_push_back(deque_t deque, void* data);

/**
 *  \brief pop data from the deque from the front.
 *  \param[in] deque: address of deque
 *  \param[out] data: the address of data
 *  \return 1 success or 0 fail
 */
int deque_pop_front(deque_t deque, void* data);

/**
 *  \brief pop data from the deque from the back.
 *  \param[in] deque: address of deque
 *  \param[out] data: the address of data
 *  \return 1 success or 0 fail
 */
int deque_pop_back(deque_t deque, void* data);

/**
 *  \brief clear deque.
 *  \param[in] *deque: address of deque
 *  \return none
 */
void deque_clear(deque_t deque);

/**
 *  \brief get deque index.
 *  \param[in] *deque: address of deque
 *  \param[in] index: index of deque
 *  \return index of deque buffer
 */
int deque_index(deque_t deque, int index);

/**
 *  \brief get data address of deque.
 *  \param[in] deque: deque handler
 *  \param[in] index: index
 *  \return address of deque data or NULL fail
 */
void* deque_data(deque_t deque, int index);

/**
 *  \brief get size of deque.
 *  \param[in] deque: deque handler
 *  \return size of deque
 */
int deque_size(deque_t deque);

/**
 *  \brief get capacity of deque.
 *  \param[in] deque: deque handler
 *  \return capacity of deque
 */
int deque_capacity(deque_t deque);

/**
 *  \brief get data size of deque.
 *  \param[in] deque: deque handler
 *  \return data size of deque
 */
int deque_dsize(deque_t deque);

/**
 *  \brief check if empty.
 *  \param[in] deque: deque handler
 *  \return 1 empty or 0 not empty
 */
int deque_empty(deque_t deque);

/**
 *  \brief check if full.
 *  \param[in] deque: deque handler
 *  \return 1 full or 0 not full
 */
int deque_full(deque_t deque);

/**
 *  \brief A simple method for `deque_create`.
 *  \param[in] type: data type
 *  \param[in] capacity: capacity of deque
 *  \return deque handler or NULL fail
 */
#define deque(type, capacity)               deque_create(sizeof(type), (capacity), NULL)

/**
 *  \brief A simple method for `deque_delete`.
 *  \param[in] deque: deque handler
 *  \return none
 */
#define _deque(deque)                       do{deque_delete(deque);(deque)=NULL;}while(0)

/**
 *  \brief Random access method for deque data.
 *  \param[in] deque: deque handler
 *  \param[in] type: data type
 *  \param[in] i: index starting from deque header
 *  \return Reference to deque data
 */
#define deque_at(deque, type, i)            (*(type *)deque_data((deque), (i)))

#endif
