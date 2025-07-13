/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  vlog.h
 *         \unit  vlog
 *        \brief  This is a simple log module for C language
 *       \author  Lamdonn
 *      \version  v1.1.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __vlog_H
#define __vlog_H

#include <stdarg.h>

/* Version infomation */

#define VLOG_V_MAJOR                        1
#define VLOG_V_MINOR                        1
#define VLOG_V_PATCH                        0

/* channel type define */

typedef unsigned char                       vlogChnType;

/* callback function define */

typedef void                                (*vlogFuncType)(char *buf, int len);

/* vlog buffer size */

#define VLOG_BUFFER_SIZE                    (1024)

/* vlog channel */

#define VLOG_CHANNEL_0                      ((vlogChnType)(0x01))
#define VLOG_CHANNEL_1                      ((vlogChnType)(0x02))
#define VLOG_CHANNEL_2                      ((vlogChnType)(0x04))
#define VLOG_CHANNEL_3                      ((vlogChnType)(0x08))
#define VLOG_CHANNEL_4                      ((vlogChnType)(0x10))
#define VLOG_CHANNEL_5                      ((vlogChnType)(0x20))
#define VLOG_CHANNEL_6                      ((vlogChnType)(0x40))
#define VLOG_CHANNEL_7                      ((vlogChnType)(0x80))
#define VLOG_CHANNEL_ALL                    ((vlogChnType)(0xFF))

/* Enable and disable a channel */

#define VLOG_ENABALE(c)                     vlog_set_filter(vlog_get_filter() | (c))
#define VLOG_DISABALE(c)                    vlog_set_filter(vlog_get_filter() & ~(c))

/**
 *  \brief output log information
 *  \param[in] channel: VLOG_CHANNEL_XXX, from 0 to 7
 *  \param[in] format: format string
 *  \param[in] ...: format parameters
 *  \return log string length
 */
int vlog(vlogChnType channel, const char *format, ...);

/**
 *  \brief set vlog channel filter configuration
 *  \param[in] mask: VLOG_CHANNEL_XXX, from 0 to 7, multiple channels can be selected through `|`
 *  \return none 
 */
void vlog_set_filter(vlogChnType mask);

/**
 *  \brief get vlog channel filter configuration
 *  \return channel filter configuration mask
 */
vlogChnType vlog_get_filter(void);

/**
 *  \brief turns vlog output to the console on or off
 *  \param[in] channel: offline save file name
 *  \param[in] console: 0 off or others on
 *  \return 1 success or 0 fail 
 */
int vlog_set_console(vlogChnType channel, int console);

/**
 *  \brief gets the channel console open status
 *  \return status mask
 */
vlogChnType vlog_get_console(void);

/**
 *  \brief start vlog offline save, need to be used in pairs with stop vlog offline save 
 *  \param[in] filename: offline save file name, or NULL(0) stop vlog offline save
 *  \return 1 success or 0 fail 
 */
int vlog_set_offline(vlogChnType channel, const char *filename);

/**
 *  \brief gets the channel offline open status
 *  \return status mask
 */
vlogChnType vlog_get_offline(void);

/**
 *  \brief set additional vlog callback function
 *  \return none 
 */
int vlog_set_func(vlogChnType channel, vlogFuncType func);

/**
 *  \brief gets the channel callback function open status
 *  \return status mask
 */
vlogChnType vlog_get_func(void);

#endif 
