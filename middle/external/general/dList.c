/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  dList.c
 *         \unit  dList
 *        \brief  This is a C language doubly linked list
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "dList.h"
#include <string.h>

dList *dList_create(void)
{
    dList *list;

    /* Allocate memory for the dList structure */
    list = (dList *)malloc(sizeof(dList));
    if (!list) return NULL;

    /* Initialize structural parameters */
    list->next = list;
    list->prev = list;
    list->data = NULL;
    list->size = 0;

    return list;
}

void dList_delete(dList *list)
{
    dList *temp = NULL;

    if (!list) return;

    temp = list->prev;
    temp->next = NULL;

    while (list)
    {
        /* Save the next pointer before freeing the current node */
        temp = list->next;

        /* Free the data associated with the current node */
        if (list->data) free(list->data);

        /* Free the memory allocated for the current node */
        free(list);

        /* Move to the next node in the list */
        list = temp;
    }
}

dList *dList_attach(dList **listRef, int index, dList *attach)
{
    dList *list, *temp;

    /* Input value validity check */
    if (!listRef) return NULL;
    if (!(list = *listRef)) return NULL;
    if (!attach) return NULL;

    if (list->next == list)
    {
        switch (index)
        {
        case 0:
            *listRef = attach;
        case 1:
        case -1:
            break;
        default:
            return NULL;
        }
    }
    else  
    {
        if (index > 0)
        {
            /* Iterative movement to obtain the prev node to be inserted at the location */
            while ((--index) && (list->next != *listRef)) 
            {
                list = list->next;
            }

            /* Return NULL if the index is out of bounds */
            if (index > 0) return NULL;
        }
        else if (index < 0) 
        {
            /* Iterative movement to obtain the prev node to be inserted at the location */
            while ((++index) && (list->prev != *listRef)) 
            {
                list = list->prev;
            }
            list = list->prev;

            /* Return NULL if the index is out of bounds */
            if (index < 0) return NULL;
        }
        else  
        {
            list = list->prev;
            *listRef = attach;
        }
    }

    temp = attach->prev;

    temp->next = list->next;
    list->next->prev = temp;

    list->next = attach;
    attach->prev = list;

    return attach;
}

static int dList_locate2(dList *list, int begin, int end, dList **beginNodeRef, dList **endNodeRef)
{
    dList *beginNode = NULL, *endNode = NULL;

    if (begin >= 0)
    {
        /* Move to the position in the original list where the sublist should be attached */
        beginNode = dList_to(list, begin);
        if (!beginNode) return 0;

        /* Forward positioning of end node */
        if (end > 0)
        {
            /* Ensure the correctness of the start and end positions */
            if (end < begin) return 0;

            /* Starting from the begin node, iteratively move to the specified position node */
            endNode = beginNode;
            while ((begin < end) && (endNode->next != list)) 
            {
                endNode = endNode->next;
                begin++;
            }

            /* Return NULL if the index is out of bounds */
            if (begin != end) return 0;
        }
        else  
        {
            /* Iterative movement to obtain the prev node to be inserted at the location */
            endNode = list->prev;
            while ((++end) && (endNode != beginNode))
            {
                endNode = endNode->prev;
            }

            /* Return NULL if the index is out of bounds */
            if (end < 0) return 0;
        }
    }
    else  
    {
        /* Starting from the beginning in reverse, it is not allowed to position the end from the forward direction */
        if (end > 0) return 0;

        /* Ensure the correctness of the start and end positions */
        if (end < begin) return 0;

        /* Move to the position in the original list where the sublist should be attached */
        endNode = dList_to(list, end);
        if (!endNode) return 0;

        /* Starting from the end node, iteratively move to the specified position node */
        beginNode = endNode;
        begin -= (end + 1);
        while ((++begin) && (beginNode != list))
        {
            beginNode = beginNode->prev;
        }

        /* Return NULL if the index is out of bounds */
        if (begin < 0) return 0;
    }

    *beginNodeRef = beginNode;
    *endNodeRef = endNode;

    return 1;
}

dList *dList_detach(dList **listRef, int begin, int end, dList **outPrev)
{
    dList *list, *detach = NULL, *prev = NULL, *beginNode = NULL, *endNode = NULL;

    /* Input value validity check */
    if (!listRef) return 0;
    if (!(list = *listRef)) return 0;

    if (!dList_locate2(list, begin, end, &beginNode, &endNode)) return NULL;

    /* Update the head of the list to point to the node following the detached sublist */
    if (beginNode == *listRef) *listRef = endNode->next;

    prev = beginNode->prev;

    /* Adjusting the orientation of two circular doubly linked lists */
    prev->next = endNode->next;
    endNode->next->prev = prev;

    beginNode->prev = endNode;
    endNode->next = beginNode;

    detach = beginNode;

    /* Store the pointer to the previous node of the detached sublist */
    if (outPrev) *outPrev = prev;

    return detach;
}

dList *dList_insert(dList **listRef, int index, void *data, int size)
{
    dList *node;

    /* Input value validity check */
    if (!listRef) return NULL;

    /* Create a new node */
    node = dList_create();
    if (!node) return NULL;

    /* Set the data for the new node */
    if (!dList_set(node, data, size)) goto FAIL;

    /* Insert the new node at the specified position */
    if (!*listRef)
    {
        if (index == 0 || index == -1) *listRef = node;
        else goto FAIL;
    }
    else  
    {
        /* Jump to the failure label if attaching the new node fails */
        if (!dList_attach(listRef, index, node)) goto FAIL;
    }

    return node;

FAIL:
    /* Delete the new node in case of failure */
    dList_delete(node);
    return NULL;
}

int dList_erase(dList **listRef, int index, dList **outPrev)
{
    dList *node;

    /* Detach the node at the specified position */
    node = dList_detach(listRef, index, index, outPrev);
    if (!node) return 0;

    /* Delete the detached node */
    dList_delete(node);

    return 1;
}

int dList_pushFront(dList **listRef, void *data, int size)
{
    return dList_insert(listRef, 0, data, size) ? 1 : 0;
}

int dList_pushBack(dList **listRef, void *data, int size)
{
    return dList_insert(listRef, -1, data, size) ? 1 : 0;
}

int dList_popFront(dList **listRef)
{
    return dList_erase(listRef, 0, NULL);
}

int dList_popBack(dList **listRef)
{
    return dList_erase(listRef, -1, NULL);
}

int dList_append(dList *list, dList **append)
{
    dList *prev;

    /* Input value validity check */
    if (!list) return 0;
    if (!append || !*append) return 0;

    /* Move to the last node of the list */
    prev = list->prev;

    /* Connect the last node of the list to the head of the sublist */
    (*append)->prev->next = list;
    list->prev = (*append)->prev;

    prev->next = (*append);
    (*append)->prev = prev;

    /* Update the sublist pointer to NULL */
    *append = NULL;

    return 1;
}

dList *dList_copy(dList *list, int begin, int end)
{
    dList *copy = NULL, *node = NULL, *beginNode = NULL, *endNode = NULL;

    if (!dList_locate2(list, begin, end, &beginNode, &endNode)) return NULL;

    if (!dList_pushBack(&copy, beginNode->data, beginNode->size)) goto FAIL;

    for (node = beginNode->next; node != endNode->next; node = node->next)
    {
        /* Jump to the failure label if inserting a node fails */
        if (!dList_pushBack(&copy, node->data, node->size)) goto FAIL;
    }

    return copy;

FAIL:
    /* Delete the copied sublist in case of failure */
    dList_delete(copy);
    return NULL;
}

int dList_reverse(dList *list, int begin, int end)
{
    dList *beginNode = NULL, *endNode = NULL;
    void *data;
    int s;

    /* Input value validity check */
    if (!list) return 0;

    if (begin == end) return 0;

    if (!dList_locate2(list, begin, end, &beginNode, &endNode)) return 0;

    /* Traverse the specified interval in the list for reversing */
    for ( ; (beginNode->next != endNode) && (beginNode->next != endNode->prev); beginNode = beginNode->next, endNode = endNode->prev)
    {
        /* Swap the data and size between the current node and the corresponding node at the end position */
        data = beginNode->data;
        beginNode->data = endNode->data;
        endNode->data = data;

        s = beginNode->size;
        beginNode->size = endNode->size;
        endNode->size = s;
    }

    return 1;
}

int dList_size(dList *list)
{
    dList* node;
    int size = 0;

    /* Input value validity check */
    if (!list) return 0;

    for (node = list; node; node = ((node->next == list) ? NULL : node->next))
    {
        /* Increment the size counter */
        size++;
    }

    return size;
}

dList *dList_to(dList *list, int index)
{
    dList *node = list;

    /* Input value validity check */
    if (!list) return NULL;

    if (index > 0)
    {
        /* Iterative movement to obtain the prev node to be inserted at the location */
        while ((index--) && (node->next != list)) 
        {
            node = node->next;
        }

        /* Return NULL if the index is out of bounds */
        if (index >= 0) return NULL;
    }
    else if (index < 0) 
    {
        /* Iterative movement to obtain the prev node to be inserted at the location */
        node = node->prev;
        while ((++index) && (node != list))
        {
            node = node->prev;
        }

        /* Return NULL if the index is out of bounds */
        if (index < 0) return NULL;
    }

    return node;
}

int dList_set(dList *list, void* data, int size)
{
    void* d = NULL;

    /* Input value validity check */
    if (!list) return 0;
    if (size < 0) return 0;

    /* If the incoming data size is 0, set the air sensitive data directly */
    if (size == 0)
    {
        if (list->data) free(list->data);
        list->data = NULL;
        list->size = 0;
        return 1;
    }

    /* If the data size is inconsistent, update the data storage space */
    if (size != list->size)
    {
        d = realloc(list->data, size);
        if (!d) return 0;
        list->data = d;
    }

    /* Data assignment */
    if (data) memcpy(list->data, data, size);

    /* Update data size */
    list->size = size;

    return 1;
}

int dList_get(dList *list, void* data, int size)
{
    /* Input value validity check */
    if (!list) return 0;
    if (!data) return 0;
    if (size < list->size) return 0;

    /* Data assignment */
    memcpy(data, list->data, list->size);

    return 1;
}
