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
#ifndef __kern_H
#define __kern_H

/* Version infomation */
#define KERN_V_MAJOR                1
#define KERN_V_MINOR                0
#define KERN_V_REVISE               0

/* task type */
typedef unsigned int task_t;

/* task callback function */
typedef void (*task_handler_t)(void);

/* the kernel gets the time callback function */
typedef unsigned int (*kern_tick_t)(void);

/* kern error return value */
#define KE_OK               0                   /* no error */
#define KE_ALLOC_FAIL       1                   /* memory space allocation failed */
#define KE_TASK_LIST_NULL   2                   /* task list is empty */
#define KE_NO_THIS_TASK     3                   /* no such task */
#define KE_TASK_NUM_OVER    4                   /* number of tasks exceeded */
#define KE_TASK_ID_ERR      5                   /* wrong task id */
#define KE_TASK_REPEAT      6                   /* repetitive tasks */
#define KE_TASK_INVALID     7                   /* invalid parameter */

/**
 *  \brief initialize the kernel.
 *  \param[in] tick_func: function to get the tick
 *  \param[in] time_slice: length of tick
 *  \return KE_OK success or other fail
 */
int kern_init(kern_tick_t tick_func, unsigned short time_slice);

/**
 *  \brief create task.
 *  \param[in] period: task scheduling period, the unit is the time slice period passed in by the `kern_init`
 *  \param[in] handler: task handler
 *  \return task id or 0 fail
 */
task_t task_create(unsigned short period, task_handler_t handler);

/**
 *  \brief delete task.
 *  \param[in] task: task handler
 *  \return KE_OK success or other fail
 */
int task_delete(task_t task);

/**
 *  \brief get running task.
 *  \return task id or 0 no task is running
 */
task_t task_running(void);

/**
 *  \brief task scheduling function.
 *  \return none
 */
void kern_schedule(void);

#endif 
