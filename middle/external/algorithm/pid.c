/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  pid.c
 *         \unit  pid
 *        \brief  This is a general-purpose C language pid module
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "pid.h"

void pid_init(pid_info_t *pid)
{
    /* Initialize pid structure members */
    pid->point = 0;
    pid->process = 0;
    pid->kp = 0;
    pid->ki = 0;
    pid->kd = 0;
    pid->integral = 0;
    pid->error = 0;
    pid->output = 0;
}

pid_data_t pid_compute(pid_info_t *pid)
{
    pid_data_t error;      /* Record current error */
    pid_data_t delta;      /* Record delta error */
    pid_data_t derivative; /* Record derivative item */

    /* Calculate current error */
    error = pid->point - pid->process;

    /* Calculate the change in error */
    delta = error - pid->error;

    /* Accumulation of integral terms */
    pid->integral += error * pid->ki;
    if (pid->integral > pid->max_integral)
    {
        pid->integral = pid->max_integral;
    }
    if (pid->integral < 0 - pid->max_integral)
    {
        pid->integral = 0 - pid->max_integral;
    }

    /* Calculate differential terms */
    derivative = delta * pid->kd;

    /* Calculate PID output */
    pid->output = pid->kp * error + pid->integral + derivative;

    if (pid->output > pid->max_output)
    {
        pid->output = pid->max_output;
    }
    else if (pid->output < pid->min_output)
    {
        pid->output = pid->min_output;
    }

    /* Update previous error */
    pid->error = error;

    return pid->output; /* Return controller output */
}
