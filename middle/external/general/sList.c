/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  sList.c
 *         \unit  sList
 *        \brief  This is a C language singly linked list
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "sList.h"
#include <string.h>

sList *sList_create(void)
{
    sList *list;

    /* Allocate memory for the sList structure */
    list = (sList *)malloc(sizeof(sList));
    if (!list) return NULL;

    /* Initialize structural parameters */
    memset(list, 0, sizeof(sList));

    return list;
}

void sList_delete(sList *list)
{
    sList *temp = NULL;

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

sList *sList_attach(sList **listRef, int index, sList *attach)
{
    sList *list, *temp = attach;

    /* Input value validity check */
    if (!listRef) return NULL;
    if (!(list = *listRef)) return NULL;
    if (!attach) return NULL;

    /* Attach the sublist at the beginning of the original list */
    if (index == 0)
    {
        /* Move to the last node of the sublist */
        while (temp->next) temp = temp->next;

        /* Connect the last node of the sublist to the original list */
        temp->next = list;

        /* Update the head of the original list to point to the sublist */
        *listRef = attach;

        return attach;
    }

    /* Move to the position in the original list where the sublist should be attached */
    while (list->next && (index < 0 || --index))
    {
        list = list->next;
    }
    /* Return NULL if the index is out of bounds */
    if (index > 0) return NULL;

    /* Attach the sublist in the middle or at the end of the original list */
    while (temp->next) temp = temp->next;
    /* Connect the last node of the sublist to the node following the attachment point */
    temp->next = list->next;
    /* Connect the attachment point in the original list to the sublist */
    list->next = attach;

    return attach;
}

sList *sList_detach(sList **listRef, int index, int count, sList **outPrev)
{
    sList *list, *detach = NULL, *prev = NULL;

    /* Input value validity check */
    if (!listRef) return 0;
    if (!(list = *listRef)) return 0;

    /* Move to the position in the original list where the sublist should be attached */
    while (list->next && (index < 0 || index--))
    {
        /* Keep track of the previous node */
        prev = list;
        /* Move to the next node in the list */
        list = list->next;
    }
    /* Return NULL if the index is out of bounds */
    if (index > 0) return NULL;

    /* Set the detach pointer to the current node */
    detach = list;

    /*  Move to the next node in the list */
    while (list->next && (count < 0 || --count)) list = list->next;

    /* Connect the previous node to the node following the detached sublist */
    /* Update the head of the list to point to the node following the detached sublist */
    prev ? (prev->next = list->next) : (*listRef = list->next);
    /* Set the next pointer of the last node in the detached sublist to NULL */
    list->next = NULL;

    /* Store the pointer to the previous node of the detached sublist */
    if (outPrev) *outPrev = prev;

    return detach;
}

sList *sList_insert(sList **listRef, int index, void *data, int size)
{
    sList *node;

    /* Input value validity check */
    if (!listRef) return NULL;

    /* Create a new node */
    node = sList_create();
    if (!node) return NULL;

    /* Set the data for the new node */
    if (!sList_set(node, data, size)) goto FAIL;

    /* Insert the new node at the specified position */
    if (!*listRef)
    {
        if (index <= 0) *listRef = node;
        else goto FAIL;
    }
    else  
    {
        /* Jump to the failure label if attaching the new node fails */
        if (!sList_attach(listRef, index, node)) goto FAIL;
    }

    return node;

FAIL:
    /* Delete the new node in case of failure */
    sList_delete(node);
    return NULL;
}

int sList_erase(sList **listRef, int index, sList **outPrev)
{
    sList *node;

    /* Detach the node at the specified position */
    node = sList_detach(listRef, index, 1, outPrev);
    if (!node) return 0;

    /* Delete the detached node */
    sList_delete(node);

    return 1;
}

int sList_pushFront(sList **listRef, void *data, int size)
{
    return sList_insert(listRef, 0, data, size) ? 1 : 0;
}

int sList_pushBack(sList **listRef, void *data, int size)
{
    return sList_insert(listRef, -1, data, size) ? 1 : 0;
}

int sList_popFront(sList **listRef)
{
    return sList_erase(listRef, 0, NULL);
}

int sList_popBack(sList **listRef)
{
    return sList_erase(listRef, -1, NULL);
}

int sList_append(sList *list, sList **append)
{
    /* Input value validity check */
    if (!list) return 0;
    if (!append || !*append) return 0;

    /* Move to the last node of the list */
    list = sList_to(list, -1);

    /* Connect the last node of the list to the head of the sublist */
    list->next = *append;
    /* Update the sublist pointer to NULL */
    *append = NULL;

    return 1;
}

sList *sList_copy(sList *list, int begin, int end)
{
    int size;
    sList *copy = NULL, *node = NULL;

    /* Get the size(count of list node) of list */
    size = sList_size(list);
    if (size == 0) return 0;

    /* Set the default begin and end index to the last node of the list */
    if (begin < 0) begin = size - 1;
    if (end < 0) end = size - 1;

    /* Return NULL if the indices are out of bounds or invalid */
    if (begin >= end || end >= size) return 0;

    /* Traverse the specified interval in the list for copying */
    for (list = sList_to(list, begin), end -= begin; end >= 0; end--, list = list->next)
    {
        /* Jump to the failure label if inserting a node fails */
        node = sList_insert(&node, -1, list->data, list->size);
        if (!node) goto FAIL;

        /* Set the copy pointer to the head of the copied sublist */
        if (!copy) copy = node;
    }

    return copy;

FAIL:
    /* Delete the copied sublist in case of failure */
    sList_delete(copy);
    return NULL;
}

int sList_reverse(sList *list, int begin, int end)
{
    int size;
    sList *node, *temp;
    void *data;
    int s;

    /* Input value validity check */
    if (!list) return 0;
    
    /* Get the size(count of list node) of list */
    size = sList_size(list);
    if (size == 0) return 0;

    /* Set the default begin and end index to the last node of the list */
    if (begin < 0) begin = size - 1;
    if (end < 0) end = size - 1;

    /* Return NULL if the indices are out of bounds or invalid */
    if (begin >= end || end >= size) return 0;

    /* Traverse the specified interval in the list for reversing */
    for (node = sList_to(list, begin), end -= begin; end > 0; end -= 2, node = node->next)
    {
        /* Move to the node at the end position of the sublist */
        temp = sList_to(node, end);

        /* Swap the data and size between the current node and the corresponding node at the end position */
        data = node->data;
        node->data = temp->data;
        temp->data = data;

        s = node->size;
        node->size = temp->size;
        temp->size = s;
    }

    return 1;
}

int sList_size(sList *list)
{
    int size = 0;

    /* Input value validity check */
    if (!list) return 0;

    while (list)
    {
        /* Increment the size counter */
        size++;

        list = list->next;
    }

    return size;
}

sList *sList_to(sList *list, int index)
{
    /* Input value validity check */
    if (!list) return NULL;

    /* Move to the next node in the list */
    while (list->next && (index < 0 || index--))
    {
        list = list->next;
    }
    /* Return NULL if the index is out of bounds */
    if (index > 0) return NULL;

    return list;
}

int sList_set(sList *list, void* data, int size)
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

int sList_get(sList *list, void* data, int size)
{
    /* Input value validity check */
    if (!list) return 0;
    if (!data) return 0;
    if (size < list->size) return 0;

    /* Data assignment */
    memcpy(data, list->data, list->size);

    return 1;
}
