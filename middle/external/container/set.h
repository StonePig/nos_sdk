/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  set.h
 *         \unit  set
 *        \brief  This is a general-purpose C language set module, with common data structure
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __set_H
#define __set_H

#include <stdlib.h>

/* version infomation */

#define SET_V_MAJOR                         1
#define SET_V_MINOR                         0
#define SET_V_PATCH                         0

/* set type definition, hiding structural members, not for external use */

typedef struct SET *set_t;

/**
 *  \brief create set
 *  \param[in] dsize: size of set data
 *  \return set handler or NULL: fail
 */
set_t set_create(int dsize);

/**
 *  \brief delete set
 *  \param[in] set: set handler
 *  \return none
 */
void set_delete(set_t set);

/**
 *  \brief insert data to set.
 *  \param[in] set: set handler
 *  \param[in] index: index
 *  \param[in] data: address of data
 *  \return address of set data or NULL fail
 */
void* set_insert(set_t set, int index, void* data);

/**
 *  \brief erase data from set.
 *  \param[in] set: set handler
 *  \param[in] index: index
 *  \return 1 success or 0 fail
 */
int set_erase(set_t set, int index);

/**
 *  \brief clear all nodes under the set
 *  \param[in] set: set handler
 *  \return none
 */
void set_clear(set_t set);

/**
 *  \brief get the size of set
 *  \param[in] set: set handler
 *  \return size of set
 */
int set_size(set_t set);

/**
 *  \brief get the data size of set.
 *  \param[in] set: set handler
 *  \return data size of set
 */
int set_dsize(set_t set);

/**
 *  \brief find index from set
 *  \param[in] set: set handler
 *  \param[in] index: index
 *  \return 1 success or 0 fail
 */
int set_find(set_t set, int index);

/**
 *  \brief get the address of item data from set
 *  \param[in] set: set handler
 *  \param[in] index: index
 *  \return address of set data or set_error(): fail
 */
void* set_data(set_t set, int index);

/**
 *  \brief error return value
 *  \param[in] set: set handler
 *  \return error pointer
 */
void* set_error(set_t set);

/**
 *  \brief iterate init at tail
 *  \param[in] set: set handler
 *  \param[in] orgin: SET_HEAD or SET_TAIL
 *  \return none
 */
void set_it_init(set_t set, int orgin);

/**
 *  \brief iterate get
 *  \param[in] set: set handler
 *  \param[out] out_index: out index
 *  \return address of set iterator data
 */
void* set_it_get(set_t set, int *out_index);

/**
 *  \brief A simple method for `set_create`
 *  \param[in] type: data type
 *  \return set handler or NULL: fail
 */
#define set(type)                           set_create(sizeof(type))

/**
 *  \brief A simple method for `set_delete`.
 *  \param[in] set: set handler
 *  \return none
 */
#define _set(set)                           do{set_delete(set);(set)=NULL;}while(0)

/**
 *  \brief Random access method for set data.
 *  \param[in] set: set handler
 *  \param[in] type: data type
 *  \param[in] i: index starting from set header
 *  \return Reference to set data
 */
#define set_at(set, type, i)                (*(type *)set_data((set), (i)))

#define SET_HEAD                            0
#define SET_TAIL                            1

#endif 
