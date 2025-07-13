/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  vlog.c
 *         \unit  vlog
 *        \brief  This is a simple log module for C language
 *       \author  Lamdonn
 *      \version  v1.1.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "vlog.h"
#include <stdio.h>

#define VLOG_CHANNEL_MAX                    (8)

typedef struct 
{
    vlogChnType channel;                    /**< channel mask */
    char console;                           /**< flag whether to output to the console */
    vlogFuncType func;                      /**< flag whether to output to the callback function */
    FILE *file;                             /**< flag whether to output to the offline file, protected ones cannot be modified directly */
} vlogChnParaType;

/* Channel configuration, default only opens channel 0 */
static vlogChnType cmask = VLOG_CHANNEL_0;

/* vlog output buffer */ 
static char vlog_buffer[VLOG_BUFFER_SIZE];

/* vlog channel parameters */
static vlogChnParaType chn[VLOG_CHANNEL_MAX] = {
/*   channel            console     func        file*/
    {VLOG_CHANNEL_0,    1,          0,          0}, 
    {VLOG_CHANNEL_1,    1,          0,          0}, 
    {VLOG_CHANNEL_2,    1,          0,          0}, 
    {VLOG_CHANNEL_3,    1,          0,          0}, 
    {VLOG_CHANNEL_4,    1,          0,          0}, 
    {VLOG_CHANNEL_5,    1,          0,          0}, 
    {VLOG_CHANNEL_6,    1,          0,          0}, 
    {VLOG_CHANNEL_7,    1,          0,          0}, 
};

int vlog(vlogChnType channel, const char *format, ...)
{
    int len = -1;
    vlogChnType c;
    va_list args;

    for (c = 0; c < VLOG_CHANNEL_MAX; c++)
    {
        /* match channel */
        if ((chn[c].channel & cmask) && (chn[c].channel & channel))
        {
            /* The current channel has a concrete output object */
            if (chn[c].console || chn[c].file || chn[c].func)
            {
                /* If the output isn't already formatted, it needs to be. And format it only once. */
                if (len == -1)
                {
                    va_start(args, format);
                    len = vsnprintf(vlog_buffer, sizeof(vlog_buffer), format, args);
                    va_end(args);
                    if (len < 0) break;
                }
                
                /* Output to default console */
                if (chn[c].console) printf(vlog_buffer);

                /* Output to offline file */
                if (chn[c].file) fwrite(vlog_buffer, sizeof(char), len, chn[c].file);

                /* Output to callback function */
                if (chn[c].func) chn[c].func(vlog_buffer, len);
            }
        }
    }

    return len;
}

void vlog_set_filter(vlogChnType mask)
{
    cmask = mask;
}

vlogChnType vlog_get_filter(void)
{
    return cmask;
}

int vlog_set_console(vlogChnType channel, int console)
{
    int count = 0;
    vlogChnType c;

    for (c = 0; c < VLOG_CHANNEL_MAX; c++)
    {
        /* match channel */
        if (chn[c].channel & channel)
        {
            chn[c].console = (console ? 1: 0);

            count++;
        }
    }

    return count;
}

vlogChnType vlog_get_console(void)
{
    vlogChnType mask = 0;
    vlogChnType c;

    for (c = 0; c < VLOG_CHANNEL_MAX; c++)
    {
        /* match channel */
        if (chn[c].console)
        {
            mask |= chn[c].channel;
        }
    }

    return mask;
}

int vlog_set_offline(vlogChnType channel, const char *filename)
{
    int count = 0;
    vlogChnType c;
    FILE *file;

    for (c = 0; c < VLOG_CHANNEL_MAX; c++)
    {
        /* match channel */
        if (chn[c].channel & channel)
        {
            /* If the name passed is empty, then the default is to turn off offline output */
            if (!filename)
            {
                fclose(chn[c].file);
                chn[c].file = 0;
                continue;
            }

            /* If a file has been defined for offline saving, it needs to be closed to open a new file */
            if (chn[c].file)
            {
                fclose(chn[c].file);
            }

            /* Open an offline save file as an append */
            file = fopen(filename, "a+");
            if (!file) continue;

            chn[c].file = file;

            count++;
        }
    }

    return count;
}

vlogChnType vlog_get_offline(void)
{
    vlogChnType mask = 0;
    vlogChnType c;

    for (c = 0; c < VLOG_CHANNEL_MAX; c++)
    {
        /* match channel */
        if (chn[c].file)
        {
            mask |= chn[c].channel;
        }
    }

    return mask;
}

int vlog_set_func(vlogChnType channel, vlogFuncType func)
{
    int count = 0;
    vlogChnType c;

    for (c = 0; c < VLOG_CHANNEL_MAX; c++)
    {
        /* match channel */
        if (chn[c].channel & channel)
        {
            chn[c].func = func;

            count++;
        }
    }

    return count;
}

vlogChnType vlog_get_func(void)
{
    vlogChnType mask = 0;
    vlogChnType c;

    for (c = 0; c < VLOG_CHANNEL_MAX; c++)
    {
        /* match channel */
        if (chn[c].func)
        {
            mask |= chn[c].channel;
        }
    }

    return mask;
}

