/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  calculate.h
 *         \unit  calculate
 *        \brief  This is a simple math expression calculation module for C language
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __calculate_H
#define __calculate_H

/* Version infomation */
#define CALCULATE_V_MAJOR           1
#define CALCULATE_V_MINOR           0
#define CALCULATE_V_REVISE          0

/* Configuration information */
#define CALCULATE_EXFUNC_MAX        32                      /**< The maximum extern function count supported */

/**
 *  \brief enter a calculation expression to calculate the result, support 'in_function_table[]' table operation function
 *  \param[in] *expression: expression
 *  \return calculation result or NAN fail
 */
double calculate(const char *expression);

/**
 *  \brief Export external functions
 *  \param[in] *name: function name
 *  \param[in] func: function handle
 *  \param[in] argc: count of arguments
 *  \return 1 success or 0 fail
 */
int calculate_export(const char *name, double (*func)(), int argc);

#endif
