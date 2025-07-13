/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  map_cfg.h
 *         \unit  map
 *        \brief  This is a general-purpose C language map module, key type configuration. 
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __map_cfg_H
#define __map_cfg_H

#include <stdarg.h>

/* string type define */
typedef char* string;

#define MAP_KEY_TYPE_POINTER                0
#define MAP_KEY_TYPE_ENTITY                 1

/* macro type */
#define MK_TYPE(type)                       MAP_KEY_TYPE__##type
#define MK_TRANS(type)                      map_key_trans__##type

/* key type */
#define MAP_KEY_TYPE__int                   MAP_KEY_TYPE_ENTITY
#define MAP_KEY_TYPE__char                  MAP_KEY_TYPE_ENTITY
#define MAP_KEY_TYPE__string                MAP_KEY_TYPE_POINTER
#define MAP_KEY_TYPE__float                 MAP_KEY_TYPE_ENTITY
#define MAP_KEY_TYPE__double                MAP_KEY_TYPE_ENTITY

/* key transform function declare */
void* map_key_trans__int(void* map, va_list args);
void* map_key_trans__char(void* map, va_list args);
void* map_key_trans__string(void* map, va_list args);
void* map_key_trans__float(void* map, va_list args);
void* map_key_trans__double(void* map, va_list args);

/*********************************************************************************************************
 * add other key type transform function declare here
 ********************************************************************************************************/

#endif 
