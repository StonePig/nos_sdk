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
#include "search.h"

int search_linear(void *array, int left, int right, void *target, SearchOPS *ops)
{
    int i;

    /* Check the validity of input parameters */
    if (!array) return -1;
    if (left > right || right < 0) return -1; // If the left and right boundaries are invalid, return -1
    if (!target) return -1;
    if (!ops) return -1;

    /* Traverse the specified range of an array */
    for (i = left; i <= right; i++)
    {
        /* Compare the current element with the target element using the provided comparison function */
        if (ops->cmp(ops->addr(array, i), target) == 0)
        {
            return i;
        }
    }

    return -1;
}

static int search_binary_s(void *array, int left, int right, int r, void *target, SearchOPS *ops)
{
    int c = 0;
    int mid = left + (right - left) / 2;

    /* There is an error on the left and right boundaries, and the search has failed */
    if (left > right) return -1;

    /* Obtain comparison results */
    c = ops->cmp(ops->addr(array, mid), target) * r;

    /* According to the comparison results, search in segments */
    if (c == 0)
    {
        return mid;
    } 
    else if (c > 0)
    {
        /* Search for the left interval */
        return search_binary_s(array, left, mid - 1, r, target, ops);
    }
    else 
    {
        /* Search for the right interval */
        return search_binary_s(array, mid + 1, right, r, target, ops);
    }
}

int search_binary(void *array, int left, int right, void *target, SearchOPS *ops)
{
    int r = 0;

    /* Check the validity of input parameters */
    if (!array) return -1;
    if (left > right || right < 0) return -1;
    if (!target) return -1;
    if (!ops) return -1;

    /* Get the current sorting rule */
    r = -ops->cmp(ops->addr(array, left), ops->addr(array, right));

    /* Return binary search results */
    return search_binary_s(array, left, right, r, target, ops);
}


