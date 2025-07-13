/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  search.h
 *         \unit  search
 *        \brief  This is a C language version of commonly used search algorithms
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __search_H
#define __search_H

#define SEARCH_V_MAJOR                      1
#define SEARCH_V_MINOR                      0
#define SEARCH_V_PATCH                      0

typedef struct 
{
    /**
     *  \brief get the address of the space where the element is located
     *  \param[in] array: data handle
     *  \param[in] index: item data index
     *  \return address of item data
     */
    void* (*addr)(void *array, int index);

    /** 
     *  \brief function to compare element and target
     *  \param[in] element: address of array element
     *  \param[in] target: address of target data
     *  \return positive: element > target
     *          negative: element < target
     *          0:        element = target
     */
    int (*cmp)(void *element, void *target);
} SearchOPS;

/**
* \brief Performs linear search on an array.
*
* This function performs a linear search on the elements of an array within a specified
* range, using a user-defined comparison function provided through the SearchOPS structure.
*
* \param[in] array A pointer to the array to be searched.
* \param[in] left The left index of the search range.
* \param[in] right The right index of the search range.
* \param[in] target A pointer to the target element to be searched for.
* \param[in] ops A pointer to the SearchOPS structure containing the comparison function and address function.
*
* \return The index of the found element if successful, or -1 if the element is not found or an error occurs.
*/
int search_linear(void *array, int left, int right, void *target, SearchOPS *ops);

/**
* \brief Performs binary search on a sorted array.
*
* This function performs a binary search on the elements of a sorted array within a specified
* range, using a user-defined comparison function provided through the SearchOPS structure.
*
* \param[in] array A pointer to the sorted array to be searched.
* \param[in] left The left index of the search range.
* \param[in] right The right index of the search range.
* \param[in] target A pointer to the target element to be searched for.
* \param[in] ops A pointer to the SearchOPS structure containing the comparison function and address function.
*
* \return The index of the found element if successful, or -1 if the element is not found or an error occurs.
*/
int search_binary(void *array, int left, int right, void *target, SearchOPS *ops);

#endif
