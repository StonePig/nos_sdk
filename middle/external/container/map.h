/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  map.h
 *         \unit  map
 *        \brief  This is a general-purpose C language map module, with common data structure
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __map_H
#define __map_H

#include "map_cfg.h"
#include <stdlib.h>

/* version infomation */

#define MAP_V_MAJOR                         1
#define MAP_V_MINOR                         0
#define MAP_V_PATCH                         0

/* map type definition, hiding structural members, not for external use */

typedef struct MAP *map_t;

/**
 *  \brief create map
 *  \param[in] vsize: size of map data
 *  \return map handler or NULL: fail
 */
map_t map_create(int vsize, int ksize, void *trans);

/**
 *  \brief delete map
 *  \param[in] map: map handler
 *  \return none
 */
void map_delete(map_t map);

/**
 *  \brief insert data to map.
 *  \param[in] map: map handler
 *  \param[in] value value, address of value
 *  \param[in] ...: mpair(key, value), key and address of value
 *  \return address of map data or NULL fail
 */
void* map_insert(map_t map, ...);

/**
 *  \brief erase data from map.
 *  \param[in] map: map handler
 *  \param[in] ...: key
 *  \return 1 success or 0 fail
 */
int map_erase(map_t map, ...);

/**
 *  \brief find index from map
 *  \param[in] map: map handler
 *  \param[in] ...: key
 *  \return 1 success or 0 fail
 */
int map_find(map_t map, ...);

/**
 *  \brief get the address of item data from map
 *  \param[in] map: map handler
 *  \param[in] ...: key
 *  \return address of map data or map_error(): fail
 */
void* map_data(map_t map, ...);

/**
 *  \brief clear all nodes under the map
 *  \param[in] map: map handler
 *  \return none
 */
void map_clear(map_t map);

/**
 *  \brief get the size of map
 *  \param[in] map: map handler
 *  \return size of map
 */
int map_size(map_t map);

/**
 *  \brief get the size of map key
 *  \param[in] map: map handler
 *  \return size of map key
 */
int map_ksize(map_t map);

/**
 *  \brief get the size of map value
 *  \param[in] map: map handler
 *  \return size of map value
 */
int map_vsize(map_t map);

/**
 *  \brief error return value
 *  \param[in] map: map handler
 *  \return error pointer
 */
void* map_error(map_t map);

/**
 *  \brief iterate init at tail
 *  \param[in] map: map handler
 *  \param[in] orgin: MAP_HEAD or MAP_TAIL
 *  \return none
 */
void map_it_init(map_t map, int orgin);

/**
 *  \brief iterate get
 *  \param[in] map: map handler
 *  \param[out] kaddress: address of key
 *  \param[out] ksize: size of key
 *  \return address of map iterator data
 */
void* map_it_get(map_t map, void **kaddress, int *ksize);

/**
 *  \brief A simple method for `map_create`
 *  \param[in] ktype: key type, default support: char, int, string, float, double
 *  \param[in] vtype: value type
 *  \return map handler or NULL: fail
 */
#define map(ktype, vtype)                   map_create(sizeof(vtype), (MK_TYPE(ktype)==MAP_KEY_TYPE_POINTER)?0:sizeof(ktype), MK_TRANS(ktype))

/**
 *  \brief A simple method for `map_delete`.
 *  \param[in] map: map handler
 *  \return none
 */
#define _map(map)                           do{map_delete(map);(map)=0;}while(0)

/**
 *  \brief Random access method for map data.
 *  \param[in] map: map handler
 *  \param[in] type: value type
 *  \param[in] key: key
 *  \return Reference to map data
 */
#define map_at(map, type, key)              (*(type *)map_data((map), (key)))

/**
 *  \brief Key value pairs.
 *  \param[in] key: key
 *  \param[in] value: address of value
 *  \return pair
 */
#define mpair(key, value)                   (value),(key)

#define MAP_HEAD                            0
#define MAP_TAIL                            1

#endif 
