/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  map_cfg.c
 *         \unit  map
 *        \brief  This is a general-purpose C language map module,key type configuration.
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "map_cfg.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "map.h"

extern void* map_trans_key(void* map, void *address, int size);

/** 
 *  \brief key transform function of type int
 *  \param[in] map: map handler
 *  \param[in] key: int key
 *  \return address of key
 */
void* map_key_trans__int(void* map, va_list args)
{
    int key;
    key = va_arg(args, int);
    return map_trans_key(map, &key, sizeof(int));
}

/** 
 *  \brief key transform function of type char
 *  \param[in] map: map handler
 *  \param[in] key: char key
 *  \return address of key
 */
void* map_key_trans__char(void* map, va_list args)
{
    char key;
    key = va_arg(args, int);
    return map_trans_key(map, &key, sizeof(char));
}

/** 
 *  \brief key transform function of type char* string
 *  \param[in] map: map handler
 *  \param[in] key: string key
 *  \return address of key
 */
void* map_key_trans__string(void* map, va_list args)
{
    char* key;
    key = va_arg(args, char*);
    return map_trans_key(map, key, strlen(key) + 1);
}

/** 
 *  \brief key transform function of type float
 *  \param[in] map: map handler
 *  \param[in] key: string key
 *  \return address of key
 */
void* map_key_trans__float(void* map, va_list args)
{
    float key;
    key = va_arg(args, double);
    return map_trans_key(map, &key, sizeof(float));
}

/** 
 *  \brief key transform function of type double
 *  \param[in] map: map handler
 *  \param[in] key: string key
 *  \return address of key
 */
void* map_key_trans__double(void* map, va_list args)
{
    double key;
    key = va_arg(args, double);
    return map_trans_key(map, &key, sizeof(double));
}

/*********************************************************************************************************
 * add other key type transform function here
 ********************************************************************************************************/

