/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  pid.h
 *         \unit  pid
 *        \brief  This is a general-purpose C language pid module
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __pid_H
#define __pid_H

#define PID_V_MAJOR 1
#define PID_V_MINOR 0
#define PID_V_PATCH 0

typedef long pid_data_t;

/* PID controller structure */
typedef struct
{
    /**< Set point or target value */
    pid_data_t point;

    /**< Current value of process variable */
    pid_data_t process;

    /**< Proportional coefficient */
    pid_data_t kp;

    /**< Integration coefficient */
    pid_data_t ki;

    /**< Differential coefficient */
    pid_data_t kd;

    /**< Integral item */
    pid_data_t integral;

    pid_data_t max_integral;

    /**< Previous error */
    pid_data_t error;

    /**< Controller output */
    pid_data_t output;

    pid_data_t max_output;

    pid_data_t min_output;
} pid_info_t;

/**
 * \brief Initializes the members of a PID controller structure.
 *
 * This function initializes the members of a PIDC structure with default values.
 *
 * \param[in] pid A pointer to the PIDC structure to be initialized.
 */
void pid_init(pid_info_t *pid);

/**
 * \brief Computes the output of a PID controller.
 *
 * This function takes a PIDC structure pointer as input and computes the output
 * of a PID controller based on the current error, previous error, integral term,
 * and derivative term.
 *
 * \param[in] pid A pointer to the PIDC structure containing the PID controller parameters and variables.
 *
 * \return The output of the PID controller.
 */
pid_data_t pid_compute(pid_info_t *pid);

#endif
