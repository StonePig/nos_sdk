/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  sort.h
 *         \unit  sort
 *        \brief  This is a general-purpose C language sort module
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __sort_H
#define __sort_H

#define SORT_V_MAJOR                        1
#define SORT_V_MINOR                        0
#define SORT_V_PATCH                        0
          
/* Sorting algorithm operation function set structure definition */
typedef struct 
{
    /** 
     *  \brief ordering rules, e.g front < back ascending and front > back descending
     *  \param[in] front: address of front data
     *  \param[in] back: address of back data
     *  \return positive: following sorting rules
     *          negative: violating sorting rules
     *          0: does't affect sorting rules
     */
    int (*order)(void *front, void *back);

    /**
     *  \brief get the address of the space where the element is located
     *  \param[in] array: data handle
     *  \param[in] index: item data index
     *  \return address of item data
     */
    void* (*addr)(void *array, int index);

    /**
     *  \brief Swap data between two spaces
     *  \param[in] array: data handle
     *  \param[in] index0: item data index0
     *  \param[in] index1: item data index1
     *  \return none
     */
    void (*swap)(void *array, int index0, int index1);
    
} SOPS;


/* Sorting algorithm declare
 * 
 * All method interface types are consistent, and the external use methods are also consistent.
 *
 * Choose a specific sorting algorithm based on specific usage scenarios.
 * 
 * Choose `SOPS` based on different data structures and sorting rules.
 *
 *  \param[in] array: data handle
 *  \param[in] begin: begin index
 *  \param[in] end: end index
 *  \param[in] ops: operation function set
 *  \return 1: success or 0: fail
 */

int sort_bubble(void *array, int begin, int end, SOPS* ops);
int sort_select(void *array, int begin, int end, SOPS* ops);
int sort_insert(void *array, int begin, int end, SOPS* ops);
int sort_shell(void *array, int begin, int end, SOPS* ops);
int sort_quick(void *array, int begin, int end, SOPS* ops);
int sort_heap(void *array, int begin, int end, SOPS* ops);

/* Basic `SOPS` declare
 * 
 * Contains a set of ascending and descending order operation functions for basic data types. 
 *
 * These `SOPS` can be used directly for array type data sets.
 * 
 * For other types of data sets (such as list, queue, ...), you can reuse some functions of `SOPS` and 
 * add a small number of change functions to form a new `SOPS`.
 */

extern SOPS sops_char_ascend;
extern SOPS sops_char_descend;
extern SOPS sops_uchar_ascend;
extern SOPS sops_uchar_descend;
extern SOPS sops_short_ascend;
extern SOPS sops_short_descend;
extern SOPS sops_ushort_ascend;
extern SOPS sops_ushort_descend;
extern SOPS sops_int_ascend;
extern SOPS sops_int_descend;
extern SOPS sops_uint_ascend;
extern SOPS sops_uint_descend;
extern SOPS sops_float_ascend;
extern SOPS sops_float_descend;
extern SOPS sops_double_ascend;
extern SOPS sops_double_descend;

#endif 
