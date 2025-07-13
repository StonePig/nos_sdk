/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  vector.h
 *         \unit  vector
 *        \brief  This is a C language vector
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __vector_H
#define __vector_H

#include <stdlib.h>

/* version infomation */

#define VECTOR_V_MAJOR                      1
#define VECTOR_V_MINOR                      0
#define VECTOR_V_PATCH                      0

/* vector type definition, hiding structural members, not for external use */

typedef struct VECTOR *vector_t;

/**
 *  \brief create a vector.
 *  \param[in] dsize: size of vector data
 *  \param[in] size: capacity of vector
 *  \return vector handler or NULL fail
 */
vector_t vector_create(int dsize, int size);

/**
 *  \brief delete a vector.
 *  \param[in] vector: vector handler
 *  \return none
 */
void vector_delete(vector_t vector);

/**
 *  \brief get data address of vector.
 *  \param[in] vector: vector handler
 *  \param[in] index: index
 *  \return address of vector data or NULL fail
 */
void* vector_data(vector_t vector, int index);

/**
 *  \brief get the size of vector.
 *  \param[in] vector: vector handler
 *  \return the size of vector
 */
int vector_size(vector_t vector);

/**
 *  \brief resize vector.
 *  \param[in] vector: vector handler
 *  \param[in] size: new size
 *  \return 1 success or 0 fail
 */
int vector_resize(vector_t vector, int size);

/**
 *  \brief get the capacity of vector.
 *  \param[in] vector: vector handler
 *  \return the capacity of vector
 */
int vector_capacity(vector_t vector);

/**
 *  \brief insert data to vector.
 *  \param[in] vector: vector handler
 *  \param[in] index: index
 *  \param[in] data: array of data, if data is NULL, it is to insert a new space without assigning a value
 *  \param[in] num: number of data
 *  \return 1 success or 0 fail
 */
int vector_insert(vector_t vector, int index, void* data, int num);

/**
 *  \brief erase data from vector.
 *  \param[in] vector: vector handler
 *  \param[in] index: index
 *  \param[in] num: number of erase, max count
 *  \return the number of actual erasures
 */
int vector_erase(vector_t vector, int index, int num);

/**
 *  \brief A simple method for `vector_create`.
 *  \param[in] type: data type
 *  \return vector handler or NULL fail
 */
#define vector(type, size)                  vector_create(sizeof(type), size)

/**
 *  \brief A simple method for `vector_delete`.
 *  \param[in] vector: vector handler
 *  \return none
 */
#define _vector(vector)                     do{vector_delete(vector);(vector)=NULL;}while(0)

/**
 *  \brief push data into the vector from the front.
 *  \param[in] vector: address of vector
 *  \param[in] data: the address of data
 *  \return address of vector data or NULL fail
 */
#define vector_push_front(vector, data)     vector_insert((vector), 0, (data), 1)

/**
 *  \brief push data into the vector from the back.
 *  \param[in] vector: address of vector
 *  \param[in] data: the address of data
 *  \return address of vector data or NULL fail
 */
#define vector_push_back(vector, data)      vector_insert((vector), vector_size(vector), (data), 1)

/**
 *  \brief pop data from the vector from the front.
 *  \param[in] vector: address of vector
 *  \param[out] data: the address of data
 *  \return 1 success or 0 fail
 */
#define vector_pop_front(vector)            vector_erase((vector), 0, 1)

/**
 *  \brief pop data from the vector from the back.
 *  \param[in] vector: address of vector
 *  \param[out] data: the address of data
 *  \return 1 success or 0 fail
 */
#define vector_pop_back(vector)             vector_erase((vector), vector_size(vector)-1, 1)

/**
 *  \brief clear vector.
 *  \param[in] vector: address of vector
 *  \return vector size
 */
#define vector_clear(vector)                vector_resize((vector), 0)

/**
 *  \brief Random access method for vector data.
 *  \param[in] vector: vector handler
 *  \param[in] type: data type
 *  \param[in] i: index starting from vector header
 *  \return Reference to vector data
 */
#define vector_at(vector, type, i)          (*(type *)vector_data((vector), (i)))

/**
 *  \brief Using vector as array.
 *  \param[in] vector: vector handler
 *  \param[in] type: data type
 *  \return The array address where the vector actually stores data
 */
#define v2a(vector, type)                   ((type *)vector_data((vector), 0))

#endif
