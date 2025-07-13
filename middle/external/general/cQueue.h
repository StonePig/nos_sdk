/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  cQueue.h
 *         \unit  cQueue
 *        \brief  This is a C language universal queue controller
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __cQueue_H
#define __cQueue_H

#include <stdlib.h>

/* Version infomation */

#define CQUEUE_V_MAJOR                      1
#define CQUEUE_V_MINOR                      0
#define CQUEUE_V_PATCH                      0

typedef struct 
{
    /**< Index of queue head */
    unsigned int head;

    /**< Index of queue tail */
    unsigned int tail;

    /**< Size of queue */
    unsigned int size;

    /**< Capacity of queue */
    unsigned int cap;
} cQueue;

/*
 * The control method of cQueue controller provides an interface for macro definition methods
 * In general, these functions are not directly used, but rather macro definitions are used
 * Using macro definition methods will be more convenient, direct, and simple 
 */
void cQueue_initCap(cQueue *q, int cap);
int cQueue_moveHead(cQueue *q);
int cQueue_moveTail(cQueue *q);
int cQueue_index(cQueue *q, unsigned int index);

/**
 *  \brief Initialize queue object structure.
 *  \param[in] qObject: queue object
 *  \return none
 */
#define cQueue_init(qObject)        (cQueue_initCap(&((qObject).queue), sizeof((qObject).p_data)/sizeof((qObject).p_data[0])))

/**
 *  \brief check if empty.
 *  \param[in] qObject: queue object
 *  \return 1 empty or 0 not empty
 */
#define cQueue_empty(qObject)       (((qObject).queue.size == 0) ? 1 : 0)

/**
 *  \brief check if full.
 *  \param[in] qObject: queue object
 *  \return 1 full or 0 not full
 */
#define cQueue_full(qObject)        (((qObject).queue.size == (qObject).queue.cap) ? 1 : 0)

/**
 *  \brief push data into the queue.
 *  \param[in] qObject: queue object
 *  \param[in] d: pushed data
 *  \return 1 success or 0 fail
 */
#define cQueue_push(qObject, d)     (((qObject).queue.size == (qObject).queue.cap) ? 0 : ((qObject).p_data[cQueue_moveTail(&((qObject).queue))] = (d), 1))

/**
 *  \brief pop data from the queue.
 *  \param[in] qObject: queue object
 *  \param[in] d: pushed data
 *  \return 1 success or 0 fail
 */
#define cQueue_pop(qObject, d)      (((qObject).queue.size == 0) ? 0 : (((d) = ((qObject).p_data[cQueue_moveHead(&((qObject).queue))])), 1))

/**
 *  \brief Random access method for queue data.
 *  \param[in] qObject: queue object
 *  \param[in] i: index starting from queue header
 *  \return Reference to queue data
 */
#define cQueue_at(qObject, i)       ((qObject).data[cQueue_index(&((qObject).queue), (i))])

#if 0 /* cQueue demo */
void test_int(void)
{
    typedef struct {                                        /* 1. Define a new structure */
        cQueue queue;                                       /* 2. The structure contains cQueue structure member and named `queue` */
        int data[10];                                       /* 3. Define a buffer array of any data type, and named data */
    } intQueueType;                                         /* 4. New structure name */
    intQueueType intQueue;                                  /* 5. Define a queue structure variable */

    cQueue_init(intQueue);                                  /* 6. Initialize the variable */
                                                            /* 7. Next, you can manipulate the queue through this structural variable */
    for (int i = 0; i < intQueue.queue.cap; i++)            /* 8. Queue capacity */
    {
        cQueue_push(intQueue, i);                           /* 9. Push data */
    }

    while (intQueue.queue.size > 0)                         /* 10. Queue size */
    {
        int data;
        cQueue_pop(intQueue, data);                         /* 11. Pop data */
        printf("cQueue_pop %d\r\n", data);
    }
}
#endif 

#endif
