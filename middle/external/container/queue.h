/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  queue.h
 *         \unit  queue
 *        \brief  This is a C language queue
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __queue_H
#define __queue_H

#include <stdlib.h>

/* version infomation */

#define QUEUE_V_MAJOR                       1
#define QUEUE_V_MINOR                       0
#define QUEUE_V_PATCH                       0

/* queue type definition, hiding structural members, not for external use */

typedef struct QUEUE *queue_t;

/**
 *  \brief create a queue.
 *  \param[in] dsize: size of queue data
 *  \param[in] capacity: capacity of queue
 *  \param[in] base: allocated array or pass in `NULL` to dynamically allocate space
 *  \return queue handler or NULL fail
 */
queue_t queue_create(int dsize, int capacity, void *base);

/**
 *  \brief delete a queue.
 *  \param[in] queue: queue handler
 *  \return none
 */
void queue_delete(queue_t queue);

/**
 *  \brief push data into the queue.
 *  \param[in] queue: address of queue
 *  \param[in] data: the address of data
 *  \return 1 success or 0 fail
 */
int queue_push(queue_t queue, void* data);

/**
 *  \brief pop data from the queue.
 *  \param[in] queue: address of queue
 *  \param[out] data: the address of data
 *  \return 1 success or 0 fail
 */
int queue_pop(queue_t queue, void* data);

/**
 *  \brief clear queue.
 *  \param[in] queue: address of queue
 *  \return none
 */
void queue_clear(queue_t queue);

/**
 *  \brief get queue index.
 *  \param[in] queue: address of queue
 *  \param[in] index: index of queue
 *  \return index of queue buffer or negative fail
 */
int queue_index(queue_t queue, int index);

/**
 *  \brief get data address of queue.
 *  \param[in] queue: queue handler
 *  \param[in] index: index
 *  \return address of queue data or NULL fail
 */
void* queue_data(queue_t queue, int index);

/**
 *  \brief get size of queue.
 *  \param[in] queue: queue handler
 *  \return size of queue
 */
int queue_size(queue_t queue);

/**
 *  \brief get capacity of queue.
 *  \param[in] queue: queue handler
 *  \return capacity of queue
 */
int queue_capacity(queue_t queue);

/**
 *  \brief get data size of queue.
 *  \param[in] queue: queue handler
 *  \return data size of queue
 */
int queue_dsize(queue_t queue);

/**
 *  \brief check if empty.
 *  \param[in] queue: queue handler
 *  \return 1 empty or 0 not empty
 */
int queue_empty(queue_t queue);

/**
 *  \brief check if full.
 *  \param[in] queue: queue handler
 *  \return 1 full or 0 not full
 */
int queue_full(queue_t queue);

/**
 *  \brief A simple method for `queue_create`.
 *  \param[in] type: data type
 *  \param[in] capacity: capacity of queue
 *  \return queue handler or NULL fail
 */
#define queue(type, capacity)               queue_create(sizeof(type), (capacity), NULL)

/**
 *  \brief A simple method for `queue_delete`.
 *  \param[in] queue: queue handler
 *  \return none
 */
#define _queue(queue)                       do{queue_delete(queue);(queue)=NULL;}while(0)

/**
 *  \brief Random access method for queue data.
 *  \param[in] queue: queue handler
 *  \param[in] type: data type
 *  \param[in] i: index starting from queue header
 *  \return Reference to queue data
 */
#define queue_at(queue, type, i)            (*(type *)queue_data((queue), (i)))

#endif
