/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  cQueue.c
 *         \unit  cQueue
 *        \brief  This is a C language universal queue controller
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "cQueue.h"

void cQueue_initCap(cQueue *q, int cap)
{
    q->head = 0;
    q->tail = 0;
    q->size = 0;
    q->cap = cap;
}

int cQueue_moveHead(cQueue *q)
{
    int index = q->head;
    q->head = (q->head + 1) % q->cap;
    q->size--;
    return index;
}

int cQueue_moveTail(cQueue *q)
{
    int index = q->tail;
    q->tail = (q->tail + 1) % q->cap;
    q->size++;
    return index;
}

int cQueue_index(cQueue *q, unsigned int index)
{
    /* Starting from the head, calculate the data index */
    return (q->head + index) % (q->cap);
}
