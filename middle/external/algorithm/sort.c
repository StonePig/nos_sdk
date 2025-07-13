/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  sort.c
 *         \unit  sort
 *        \brief  This is a general-purpose C language sort module
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "sort.h"
#include <stdlib.h>
#include <string.h>

/* simplify the operation function for obtaining `ops` */
#define ptr(i)          ((ops)->addr(array, (i))) 
#define order(f, b)     ((ops)->order(ptr(f), ptr(b))) 
#define swap(i, j)      ((ops)->swap(array, (i), (j))) 

/** 
 *  \brief Bubble sort algorithm
 *  \param[in] array: Data handle
 *  \param[in] begin: Begin index
 *  \param[in] end: End index
 *  \param[in] ops: Function operation set
 *  \return 1: success or 0: fail
 */
int sort_bubble(void *array, int begin, int end, SOPS* ops)
{
    int i = 0, j = 0;
    int tail = end + 1;

    /* Check the validity of input parameters. */
    if (!array) return 0;
    if (begin >= end || begin < 0) return 0;
    if (!ops || !ops->order || !ops->addr || !ops->swap) return 0;

    /* Traverse data */
    for (i = begin; i < tail; i++)
    {
        for (j = i; j < tail; j++)
        {
            /* Violating the sorting rules, adjust the position to meet the rules */
            if (order(i, j) < 0) swap(i, j);
        }
    }

    return 1;
}

/** 
 *  \brief Selection sort algorithm
 *  \param[in] array: Data handle
 *  \param[in] begin: Begin index
 *  \param[in] end: End index
 *  \param[in] ops: Function operation set
 *  \return 1: success or 0: fail
 */
int sort_select(void *array, int begin, int end, SOPS* ops)
{
    int i = 0, j = 0;
    int tail = end + 1;
    int temp = 0;

    /* Check the validity of input parameters. */
    if (!array) return 0;
    if (begin >= end || begin < 0) return 0;
    if (!ops || !ops->order || !ops->addr || !ops->swap) return 0;

    /* Traverse data */
    for (i = begin; i < tail; i++)
    {
        temp = i;
        for (j = i + 1; j < tail; j++)
        {
            if (order(temp, j) < 0) temp = j;        
        }
        if (temp != i) swap(temp, i);
    }

    return 1;
}

/** 
 *  \brief Insert sort algorithm
 *  \param[in] array: Data handle
 *  \param[in] begin: Begin index
 *  \param[in] end: End index
 *  \param[in] ops: Function operation set
 *  \return 1: success or 0: fail
 */
int sort_insert(void *array, int begin, int end, SOPS* ops)
{
    int i = 0, j = 0;
    int tail = end + 1;

    /* Check the validity of input parameters. */
    if (!array) return 0;
    if (begin >= end || begin < 0) return 0;
    if (!ops || !ops->order || !ops->addr || !ops->swap) return 0;
    
    /* Traverse data */
    for (i = 1; i < tail; i++)
    {
        if (order(i - 1, i) < 0)
        {
            for (j = i - 1; j >= begin; j--)
            {
                if (order(j, j + 1) < 0) swap(j, j + 1);
                else break;
            }
        }
    }

    return 1;
}

/** 
 *  \brief Shell sort algorithm
 *  \param[in] array: Data handle
 *  \param[in] begin: Begin index
 *  \param[in] end: End index
 *  \param[in] ops: Function operation set
 *  \return 1: success or 0: fail
 */
int sort_shell(void *array, int begin, int end, SOPS* ops)
{
    int i = 0, j = 0, k = 0;
    int tail = end + 1;
    int increasement = tail - begin;

    /* Check the validity of input parameters. */
    if (!array) return 0;
    if (begin >= end || begin < 0) return 0;
    if (!ops || !ops->order || !ops->addr || !ops->swap) return 0;

    /* Traverse data */
    do
    {
        /* Determine the increment of the grouping */
        increasement = increasement / 3 + 1;
        for (i = begin; i < increasement + begin; i++)
        {
            for (j = i + increasement; j < tail; j += increasement)
            {
                if (order(j - increasement, j) < 0)
                {
                    for (k = j - increasement; k >= (int)begin; k -= increasement)
                    {
                        if (order(k, k + increasement) < 0) swap(k + increasement, k);
                        else break;
                    }
                }
            }
        }
    } while (increasement > 1);

    return 1;
}

/** 
 *  \brief Quick sort algorithm
 *  \param[in] array: Data handle
 *  \param[in] begin: Begin index
 *  \param[in] end: End index
 *  \param[in] ops: Function operation set
 *  \return 1: success or 0: fail
 */
int sort_quick(void *array, int begin, int end, SOPS* ops)
{
    int temp = begin;           /* reference */
    int i = begin;
    int j = end;

    /* Check the validity of input parameters. */
    if (!array) return 0;
    if (begin >= end || begin < 0) return 0;
    if (!ops || !ops->order || !ops->addr || !ops->swap) return 0;

    /* Traverse data */
    while (i != j)
    {
        /* Find numbers that do not conform to the sorting rules from right to left */
        while (order(j, temp) <= 0 && j > i) j--;
        /* Find numbers that do not violate the ordering rules from left to right */
        while (order(i, temp) >= 0 && j > i) i++;

        if (j > i) swap(i, j);
    }
    swap(i, temp);

    /* Recursively sort subsequences */
    sort_quick(array, begin, i, ops);
    sort_quick(array, i + 1, end, ops);

    return 1;
}

/** 
 *  \brief Adjust the binary tree structure in the heap sort algorithm
 *  \param[in] array: Data handle
 *  \param[in] i: Current node index
 *  \param[in] tail: Array len
 *  \param[in] ops: Function operation set
 *  \return 0: ok or other: fail
 */
static int heap_adjust(void *array, int base, int i, int tail, SOPS* ops)
{
    int parent = i;
    int lchild = (i - base) * 2 + 1 + base;
    int rchild = (i - base) * 2 + 2 + base;

    if (lchild < tail && order(lchild, parent) < 0) parent = lchild;

    if (rchild < tail && order(rchild, parent) < 0) parent = rchild;

    /* If the value at i is smaller than the value of its left and right child nodes, swap it with the parent value */
    if (parent != i)
    {
        swap(i, parent);

        /* Recursion */
        heap_adjust(array, base, parent, tail, ops);
    }

    return 1;
}

/** 
 *  \brief Heap sort algorithm
 *  \param[in] array: Data handle
 *  \param[in] begin: Begin index
 *  \param[in] end: End index
 *  \param[in] ops: Function operation set
 *  \return 1: success or 0: fail
 */
int sort_heap(void *array, int begin, int end, SOPS* ops)
{
    int i = 0;
    int length = end - begin + 1;

    /* Check the validity of input parameters. */
    if (!array) return 0;
    if (begin >= end || begin < 0) return 0;
    if (!ops || !ops->order || !ops->addr || !ops->swap) return 0;

    /* Init heap, (length / 2 - 1 + begin) is the sequence number of the last non-leaf node in the binary tree*/
    for (i = length / 2 - 1 + begin; i >= (int)begin; i--)
    {
        heap_adjust(array, begin, i, end + 1, ops);
    }
    /* Swap the top and last elements of the heap */
    for (i = end; i >= (int)begin; i--)
    {
        swap(begin, i);
        heap_adjust(array, begin, begin, i, ops);
    }

    return 1;
}

/* int */
static int int_ascend(void *front, void *back)
{
    if (*(int *)front < *(int *)back) return 1;
    else if (*(int *)front > *(int *)back) return -1;
    else return 0;
}

static int int_descend(void *front, void *back)
{
    return -int_ascend(front, back);
}

static int uint_ascend(void *front, void *back)
{
    if (*(unsigned int *)front < *(unsigned int *)back) return 1;
    else if (*(unsigned int *)front > *(unsigned int *)back) return -1;
    else return 0;
}

static int uint_descend(void *front, void *back)
{
    return -uint_ascend(front, back);
}

static void* int_addr(void *array, int index)
{
    return ((int *)array) + index;
}

static void int_swap(void *array, int index0, int index1)
{
    int temp = ((int *)array)[index0];
    ((int *)array)[index0] = ((int *)array)[index1];
    ((int *)array)[index1] = temp;
}

/* char */
static int char_ascend(void *front, void *back)
{
    if (*(char *)front < *(char *)back) return 1;
    else if (*(char *)front > *(char *)back) return -1;
    else return 0;
}

static int char_descend(void *front, void *back)
{
    return -char_ascend(front, back);
}

static int uchar_ascend(void *front, void *back)
{
    if (*(unsigned char *)front < *(unsigned char *)back) return 1;
    else if (*(unsigned char *)front > *(unsigned char *)back) return -1;
    else return 0;
}

static int uchar_descend(void *front, void *back)
{
    return -uchar_ascend(front, back);
}

static void* char_addr(void *array, int index)
{
    return ((char *)array) + index;
}

static void char_swap(void *array, int index0, int index1)
{
    char temp = ((char *)array)[index0];
    ((char *)array)[index0] = ((char *)array)[index1];
    ((char *)array)[index1] = temp;
}

/* short */
static int short_ascend(void *front, void *back)
{
    if (*(short *)front < *(short *)back) return 1;
    else if (*(short *)front > *(short *)back) return -1;
    else return 0;
}

static int short_descend(void *front, void *back)
{
    return -short_ascend(front, back);
}

static int ushort_ascend(void *front, void *back)
{
    if (*(unsigned short *)front < *(unsigned short *)back) return 1;
    else if (*(unsigned short *)front > *(unsigned short *)back) return -1;
    else return 0;
}

static int ushort_descend(void *front, void *back)
{
    return -ushort_ascend(front, back);
}

static void* short_addr(void *array, int index)
{
    return ((short *)array) + index;
}

static void short_swap(void *array, int index0, int index1)
{
    short temp = ((short *)array)[index0];
    ((short *)array)[index0] = ((short *)array)[index1];
    ((short *)array)[index1] = temp;
}

/* float */
static int float_ascend(void *front, void *back)
{
    if (*(float *)front < *(float *)back) return 1;
    else if (*(float *)front > *(float *)back) return -1;
    else return 0;
}

static int float_descend(void *front, void *back)
{
    return -float_ascend(front, back);
}

static void* float_addr(void *array, int index)
{
    return ((float *)array) + index;
}

static void float_swap(void *array, int index0, int index1)
{
    float temp = ((float *)array)[index0];
    ((float *)array)[index0] = ((float *)array)[index1];
    ((float *)array)[index1] = temp;
}

/* double */
static int double_ascend(void *front, void *back)
{
    if (*(double *)front < *(double *)back) return 1;
    else if (*(double *)front > *(double *)back) return -1;
    else return 0;
}

static int double_descend(void *front, void *back)
{
    return -double_ascend(front, back);
}

static void* double_addr(void *array, int index)
{
    return ((double *)array) + index;
}

static void double_swap(void *array, int index0, int index1)
{
    double temp = ((double *)array)[index0];
    ((double *)array)[index0] = ((double *)array)[index1];
    ((double *)array)[index1] = temp;
}

/* Basic `SOPS` define */

SOPS sops_int_ascend        = {int_ascend,      int_addr,       int_swap    };
SOPS sops_int_descend       = {int_descend,     int_addr,       int_swap    };
SOPS sops_uint_ascend       = {uint_ascend,     int_addr,       int_swap    };
SOPS sops_uint_descend      = {uint_descend,    int_addr,       int_swap    };
SOPS sops_char_ascend       = {char_ascend,     char_addr,      char_swap   };
SOPS sops_char_descend      = {char_descend,    char_addr,      char_swap   };
SOPS sops_uchar_ascend      = {uchar_ascend,    char_addr,      char_swap   };
SOPS sops_uchar_descend     = {uchar_descend,   char_addr,      char_swap   };
SOPS sops_short_ascend      = {short_ascend,    short_addr,     short_swap  };
SOPS sops_short_descend     = {short_descend,   short_addr,     short_swap  };
SOPS sops_ushort_ascend     = {ushort_ascend,   short_addr,     short_swap  };
SOPS sops_ushort_descend    = {ushort_descend,  short_addr,     short_swap  };
SOPS sops_float_ascend      = {float_ascend,    float_addr,     float_swap  };
SOPS sops_float_descend     = {float_descend,   float_addr,     float_swap  };
SOPS sops_double_ascend     = {double_ascend,   double_addr,    double_swap };
SOPS sops_double_descend    = {double_descend,  double_addr,    double_swap };
