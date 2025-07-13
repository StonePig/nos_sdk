/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  kern.h
 *         \unit  kern
 *        \brief  This is a simple timing task scheduling kernel module for C language
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "kern.h"
#include <stdlib.h>

/* task control block */ 
typedef struct 
{
    task_t task;                                /* task id, unique, natural number */
    unsigned short period;                      /* task run cycle */  
    unsigned short ctime;                       /* task countdown */
    task_handler_t handler;                     /* task callback function */
} TCB, *tcb_t;

/* task list */
typedef struct TASK_LIST 
{
    struct TASK_LIST *next;                     /* next task */
    tcb_t tcb;                                  /* task control block */
} TASK_LIST, *task_list_t;

/* kern manager */
typedef struct 
{
    task_list_t base;                           /* next task */
    kern_tick_t tick_func;                      /* task control block */
    unsigned int tick;
    unsigned short time_slice;
    task_t running;
} KERN_MANAGE;

static KERN_MANAGE manager;

int kern_init(kern_tick_t tick_func, unsigned short time_slice)
{
    if (!tick_func) return KE_TASK_INVALID;
    manager.tick_func = tick_func;
    manager.time_slice = time_slice;
    manager.base = NULL;
    manager.tick = 0;
    manager.running = 0;
    return KE_OK;
}

task_t task_create(unsigned short period, task_handler_t handler)
{
    task_t task = 1;
    task_list_t node = NULL, prev = NULL;

    if (!handler) return 0;

    /* alloc task id */
    node = manager.base;
    while (node)
    {
        if (task < node->tcb->task) break;
        else task = node->tcb->task + 1;

        prev = node;
        node = node->next;
    }

    /* allocate task control block space */
    node = (task_list_t)malloc(sizeof(TASK_LIST));
    if (!node) return 0;
    node->tcb = (tcb_t)malloc(sizeof(TCB));
    if (!node->tcb) { free(node); return 0; }
    node->next = NULL;
    
    if (!prev) manager.base = node;
    else  
    {
        node->next = prev->next;
        prev->next = node;
    }
    
    node->tcb->task = task;
    node->tcb->period = period;
    node->tcb->ctime = 0;
    node->tcb->handler = handler;

    return task;
}

int task_delete(task_t task)
{
    task_list_t node = NULL, prev = NULL;

    if (task == 0) return KE_TASK_ID_ERR;

    node = manager.base;
    while (node)
    {
        if (node->tcb->task >= task) break;
        prev = node;
        node = node->next;
    }
    if (!node || task != node->tcb->task) return KE_TASK_ID_ERR;

    if (!prev) manager.base = node->next;
    else prev->next = node->next;
    
    free(node->tcb);
    free(node);
    
    return KE_OK;
}

task_t task_running(void)
{
    return manager.running;
}

void kern_schedule(void)
{
    task_list_t l;
    unsigned int t = manager.tick_func();

    while (1)
    {
        l = manager.base;
        t = manager.tick_func();
        t = t < manager.tick ? ((int)manager.tick + t) : (t - manager.tick);

        if (t)
        {
            while (l)
            {
                l->tcb->ctime += t;
                if (l->tcb->ctime > l->tcb->period)
                {
                    if (l->tcb->handler)
                    {
                        manager.running = l->tcb->task;
                        l->tcb->handler();
                        manager.running = 0;
                    }
                    l->tcb->ctime = 0;
                }

                l = l->next;
            }

            manager.tick = manager.tick_func();
        }
    }
}
