/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  list.c
 *         \unit  list
 *        \brief  This is a C language singly linked list with built-in iterators, simple, reliable, fast, small space
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "list.h"
#include <string.h>

/* type of list node */
typedef struct _NODE_
{
    struct _NODE_ *next;            /**< next node */
} NODE;
#define data(node) ((node)+1)       /**< data of node */

/* type of list */
typedef struct LIST
{
    NODE* base;                     /**< address of base node */
    NODE* iterator;                 /**< iterator of list */
    int size;                       /**< size of list */
    int dsize;                      /**< data size */
    int index;                      /**< index of iterator */
} LIST;

list_t list_create(int dsize)
{
    list_t list;

    /* Input value validity check */
    if (dsize <= 0) return NULL;

    /* Allocate memory for the LIST structure */
    list = (list_t)malloc(sizeof(LIST));
    if (!list) return NULL;

    /* Initialize structural parameters */
    list->base = NULL;
    list->iterator = NULL;
    list->dsize = dsize;
    list->size = 0;
    list->index = 0;

    return list;
}

void list_delete(list_t list)
{
    NODE *node, *next;

    /* Input value validity check */
    if (!list) return;

    /* Iteratively free each node */
    node = list->base;
    while (node)
    {
        next = node->next;
        free(node);
        node = next;
    }

    /* Free list structure */
    free(list);
}

/**
 *  \brief iterator iterates to the specified node.
 *  \param[in] list: list handler
 *  \param[in] index: index
 *  \return address of node
 */
static NODE* list_node(list_t list, int index)
{
    if (!list) return NULL;
    if (index < 0 || index >= list->size) return NULL;
    if (index < list->index || !list->iterator || index == 0)
    {
        list->index = 0;
        list->iterator = list->base;
    }
    while (list->iterator && list->index < index)
    {
        list->iterator = list->iterator->next;
        list->index++;
    }
    return list->iterator;
}

void* list_insert(list_t list, int index, void* data)
{
    NODE *node, *prev = NULL;

    /* Input value validity check */
    if (!list) return NULL;
    if (index < 0 || index > list->size) return NULL;

    /* Allocate memory for the NODE structure */
    node = (NODE*)malloc(sizeof(NODE) + list->dsize);
    if (!node) return NULL;

    /* Assigning data to the list */
    if (data) memcpy(data(node), data, list->dsize);

    /* Adjusting the linked list structure */
    if (index)  
    {
        prev = list_node(list, index - 1);
        node->next = prev->next;
        prev->next = node;
    }
    else  
    {
        node->next = list->base;
        list->base = node;
        list->iterator = list->base, list->index = 0;
    }

    /* Update list status */
    list->size++;

    return data(node);
}

int list_erase(list_t list, int index, int num)
{
    NODE *node, *prev = NULL;
    int count = 0;

    /* Input value validity check */
    if (!list) return 0;
    if (index < 0 || index >= list->size) return 0;
    if (num <= 0) return 0;

    /* Correct the number of erases to be made */
    if (num > list->size - index) num = list->size - index;


    /* Not starting from the list header to erase */
    if (index)
    {
        prev = list_node(list, index - 1);
        for (count = 0; count < num; count++)
        {
            if (!prev) break;
            node = prev->next;
            prev->next = node->next;
            free(node);
        }
    }
    /* Starting from the list header to erase */
    else  
    {
        prev = list->base;
        for (count = 0; count < num; count++)
        {
            if (!prev) break;
            node = prev->next;
            free(prev);
            prev = node;
        }
        list->base = prev;
        list->iterator = list->base, list->index = 0;
    }

    /* Update list status */
    list->size -= count;

    return count;
}

void* list_data(list_t list, int index)
{
    NODE *node;

    /* Input value validity check */
    if (!list) return NULL;
    if (index < 0 || index >= list->size) return NULL;

    /* Get node for accessing data */
    node = list_node(list, index);
    if (!node) return NULL;

    /* Return address of node data */
    return data(node);
}

int list_size(list_t list)
{
    /* Input value validity check */
    if (!list) return 0;

    /* Return list size */
    return list->size;
}


int list_dsize(list_t list)
{
    /* Input value validity check */
    if (!list) return 0;

    /* Return list data size */
    return list->dsize;
}
