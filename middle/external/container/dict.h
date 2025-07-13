/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  dict.h
 *         \unit  dict
 *        \brief  This is a general-purpose C language dict module, with common data structure, realized by hash table.
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __dict_H
#define __dict_H

#include <stdlib.h>

/* Version infomation */
#define DICT_V_MAJOR                        1
#define DICT_V_MINOR                        0
#define DICT_V_REVISE                       0

/** Using dict error area
 * when there is an error in the read or write operation, 
 * the operation space will point to the error space to ensure that the program runs normally. */
#define DICT_USE_ERROR

/* dict type definition, hiding structural members, not for external use */

typedef struct DICT *dict_t;

/**
 *  \brief create dict
 *  \param[in] vsize: size of dict data
 *  \return dict handler or NULL: fail
 */
dict_t dict_create(unsigned int dsize);

/**
 *  \brief delete dict
 *  \param[in] dict: dict handler
 *  \return none
 */
void dict_delete(dict_t dict);

/**
 *  \brief insert data to dict.
 *  \param[in] dict: dict handler
 *  \param[in] key: address of key
 *  \param[in] value: address of value
 *  \return address of dict data or NULL fail
 */
void* dict_insert(dict_t dict, void *key, void *value);

/**
 *  \brief erase data from dict.
 *  \param[in] dict: dict handler
 *  \param[in] key: address of key
 *  \return 1 success or 0 fail
 */
int dict_erase(dict_t dict, void *key);

/**
 *  \brief clear all nodes under the dict
 *  \param[in] dict: dict handler
 *  \return none
 */
void dict_clear(dict_t dict);

/**
 *  \brief get the size of dict
 *  \param[in] dict: dict handler
 *  \return size of dict
 */
int dict_size(dict_t dict);

/**
 *  \brief get the size of value
 *  \param[in] dict: dict handler
 *  \return size of value
 */
int dict_vsize(dict_t dict);

/**
 *  \brief find key from dict
 *  \param[in] dict: dict handler
 *  \param[in] key: address of key
 *  \return 1 success or 0 fail
 */
int dict_find(dict_t dict, void *key);

/**
 *  \brief get the address of item data from dict
 *  \param[in] dict: dict handler
 *  \param[in] key: address of key
 *  \return address of dict data or dict_error(): fail
 */
void* dict_value(dict_t dict, void *key);

/**
 *  \brief error return value
 *  \param[in] dict: dict handler
 *  \return error pointer
 */
#ifdef DICT_USE_ERROR
void* dict_error(dict_t dict);
#else 
#define dict_error(dict) NULL
#endif 

/**
 *  \brief Set key length
 *  \param[in] dict: dict handler
 *  \param[in] ksize: 0 - indefinite length, need to specify the `klength` function to get the length of the key, 
                      other - definite length, `klength` will have no effect.
 *  \param[in] klength: function to get indefinite key length
 *  \return none
 */
int dict_set_klength(dict_t dict, unsigned int ksize, int (*klength)(void *key));

/**
 *  \brief iterate init
 *  \param[in] dict: dict handler
 *  \return none
 */
void dict_it_init(dict_t dict);

/**
 *  \brief iterate get
 *  \param[in] dict: dict handler
 *  \param[out] key: out key
 *  \return address of dict iterator data
 */
void* dict_it_get(dict_t dict, char **key);

/**
 *  \brief simplified creation method
 *  \param[in] type: the type of value can be any entity type, for example, `int`, `char`, ...
 *  \return dict handler or NULL: fail
 */
#define dict(type)                          dict_create(sizeof(type))

/**
 *  \brief simplified delete method
 *  \param[in] dict: dict handler
 *  \return none
 */
#define _dict(dict)                         do{dict_delete(dict);(dict)=NULL;}while(0)

/**
 *  \brief access method
 *  \param[in] dict: dict handler
 *  \param[in] type: the type of value, it needs to be the same as when it was created.
 *  \param[in] key: address of key
 *  \return none
 */
#define dict_at(dict, type, key)            (*(type *)dict_value((dict), (key)))

/** 
 *  \brief convert literals to addresses
 *  \param[in] type: type, such as int/char/float etc.
 *  \param[in] value: literal value
 *  \return address of literal
 */
#ifndef literal
#define literal(type, value)                ((type[1]){value})
#endif 

#endif 
