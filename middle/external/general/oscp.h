/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  oscp.h
 *         \unit  oscp
 *        \brief  This is a simple analog oscilloscope module for C language
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __oscp_H
#define __oscp_H

/* Version infomation */
#define OSCP_V_MAJOR                1
#define OSCP_V_MINOR                0
#define OSCP_V_REVISE               0

/* Display resolution, which is the maximum horizontal value that a numerical value can display */
#define RESOLUTION                  100

/* Time scale, how long it takes to display a data point */
#define O_SCALE_5MS                 1
#define O_SCALE_10MS                2
#define O_SCALE_20MS                3
#define O_SCALE_50MS                4
#define O_SCALE_100MS               5
#define O_SCALE_200MS               6
#define O_SCALE_500MS               7
#define O_SCALE_1S                  8
#define O_SCALE_2S                  9

/**
 *  \brief the displayed processing task function needs to be called every 5ms..
 *  \return none
 */
void oscp_handle(void);

/**
 *  \brief set the address of the value to be monitored.
 *  \param[in] *m: the address of the value to be monitored
 *  \return 1 success or 0 fail
 */
int oscp_set_monitor(int *m);

/**
 *  \brief set the time scale.
 *  \param[in] *s: time scale, O_SCALE_XXX
 *  \return 1 success or 0 fail
 */
int oscp_set_scale(int s);

#endif 
