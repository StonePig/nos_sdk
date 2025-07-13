/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  list.h
 *         \unit  list
 *        \brief  This is a C language singly linked list with built-in iterators, simple, reliable, fast, small space
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __list_H
#define __list_H

#include <stdlib.h>

/* version infomation */

#define LIST_V_MAJOR                        1
#define LIST_V_MINOR                        0
#define LIST_V_PATCH                        0

/* list type definition, hiding structural members, not for external use */

typedef struct LIST *list_t;

/**
 *  \brief create a list.
 *  \param[in] dsize: size of list data
 *  \return list handler or NULL fail
 */
list_t list_create(int dsize);

/**
 *  \brief delete a list.
 *  \param[in] list: list handler
 *  \return none
 */
void list_delete(list_t list);

/**
 *  \brief insert data to list.
 *  \param[in] list: list handler
 *  \param[in] index: index
 *  \param[in] data: address of data
 *  \return address of list data or NULL fail
 */
void* list_insert(list_t list, int index, void* data);

/**
 *  \brief erase data from list.
 *  \param[in] list: list handler
 *  \param[in] index: index
 *  \param[in] num: number of erase
 *  \return the number of actual erasures
 */
int list_erase(list_t list, int index, int num);

/**
 *  \brief get data address of list.
 *  \param[in] list: list handler
 *  \param[in] index: index
 *  \return address of list data or NULL fail
 */
void* list_data(list_t list, int index);

/**
 *  \brief get the size of list.
 *  \param[in] list: list handler
 *  \return size of list
 */
int list_size(list_t list);

/**
 *  \brief get the data size of list.
 *  \param[in] list: list handler
 *  \return data size of list
 */
int list_dsize(list_t list);

/**
 *  \brief A simple method for `list_create`.
 *  \param[in] type: data type
 *  \return list handler or NULL fail
 */
#define list(type)                          list_create(sizeof(type))

/**
 *  \brief A simple method for `list_delete`.
 *  \param[in] list: list handler
 *  \return none
 */
#define _list(list)                         do{list_delete(list);(list)=NULL;}while(0)

/**
 *  \brief push data into the list from the front.
 *  \param[in] list: address of list
 *  \param[in] data: the address of data
 *  \return address of list data or NULL fail
 */
#define list_push_front(list, data)         list_insert((list), 0, (data))

/**
 *  \brief push data into the list from the back.
 *  \param[in] list: address of list
 *  \param[in] data: the address of data
 *  \return address of list data or NULL fail
 */
#define list_push_back(list, data)          list_insert((list), list_size(list), (data))

/**
 *  \brief pop data from the list from the front.
 *  \param[in] list: address of list
 *  \param[out] data: the address of data
 *  \return 1 success or 0 fail
 */
#define list_pop_front(list)                list_erase((list), 0, 1)

/**
 *  \brief pop data from the list from the back.
 *  \param[in] list: address of list
 *  \param[out] data: the address of data
 *  \return 1 success or 0 fail
 */
#define list_pop_back(list)                 list_erase((list), list_size(list), 1)

/**
 *  \brief clear list.
 *  \param[in] list: address of list
 *  \return list size
 */
#define list_clear(list)                    list_erase((list), 0, list_size(list))

/**
 *  \brief Random access method for list data.
 *  \param[in] list: list handler
 *  \param[in] type: data type
 *  \param[in] i: index starting from list header
 *  \return Reference to list data
 */
#define list_at(list, type, i)              (*(type *)list_data((list), (i)))

#endif
