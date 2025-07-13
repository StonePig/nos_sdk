/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  dList.h
 *         \unit  dList
 *        \brief  This is a C language doubly linked list
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __dList_H
#define __dList_H

#include <stdlib.h>

/* Version infomation */

#define DLIST_V_MAJOR                       1
#define DLIST_V_MINOR                       0
#define DLIST_V_PATCH                       1

/* Type of dList */
typedef struct dList
{
    /**< Doubly linked list, pointing to the next and prev node in the linked list */
    struct dList *next;
    struct dList *prev;

    /**< The base address for storing data, allocated in the API */
    void *data;
    
    /**< The size of node data */
    int size;
} dList;


/**
 * \brief Creates a new doubly linked list.
 *
 * \return A pointer to the newly created dList structure, or NULL if memory allocation fails.
 */
dList *dList_create(void);

/**
 * \brief Deletes a doubly linked list and frees the associated memory.
 *
 * \param[in] list The dList structure to delete.
 */
void dList_delete(dList *list);

/**
 * \brief Attaches a sublist to a specified position in a doubly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the original list.
 * \param[in] index The index at which to attach the sublist (0-based index, negative-Reverse starting from `-1`).
 * \param[in] attach The sublist to attach.
 * \return A pointer to the attach head of the modified list, or NULL if invalid parameters are provided.
 */
dList *dList_attach(dList **listRef, int index, dList *attach);

/**
 * \brief Detaches a sublist from a specified position in a doubly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the list.
 * \param[in] begin The index at begin position which to detach the sublist (0-based index, negative-Reverse starting from `-1`).
 * \param[in] end The index at end position which to detach the sublist (0-based index, negative-Reverse starting from `-1`).
 * \param[out] outPrev A pointer to store the pointer to the previous node of the detached sublist.
 * \return A pointer to the head of the detached sublist, or NULL if invalid parameters are provided.
 */
dList *dList_detach(dList **listRef, int begin, int end, dList **outPrev);

/**
 * \brief Inserts a node with the specified data at a specified position in a doubly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the list.
 * \param[in] index The index at which to insert the node (0-based index, negative-Reverse starting from `-1`).
 * \param[in] data A pointer to the data to be inserted.
 * \param[in] size The size of the data to be inserted.
 * \return A pointer to the newly inserted node, or NULL if memory allocation fails or invalid parameters are provided.
 */
dList *dList_insert(dList **listRef, int index, void *data, int size);

/**
 * \brief Erases a node at the specified position in a doubly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the list.
 * \param[in] index The index of the node to erase (0-based index, negative-Reverse starting from `-1`).
 * \param[out] outPrev A pointer to store the pointer to the previous node of the erased node.
 * \return 1 if the node was successfully erased, 0 if invalid parameters are provided or the index is out of bounds.
 */
int dList_erase(dList **listRef, int index, dList **outPrev);

/**
 * \brief Push a data from the front to the doubly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the list.
 * \param[in] data A pointer to the data to be inserted.
 * \param[in] size The size of the data to be inserted.
 * \return 1 success or 0 fail.
 */
int dList_pushFront(dList **listRef, void *data, int size);

/**
 * \brief Push a data from the back to the doubly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the list.
 * \param[in] data A pointer to the data to be inserted.
 * \param[in] size The size of the data to be inserted.
 * \return 1 success or 0 fail.
 */
int dList_pushBack(dList **listRef, void *data, int size);

/**
 * \brief Pop a data from the front in the doubly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the list.
 * \return 1 success or 0 fail.
 */
int dList_popFront(dList **listRef);

/**
 * \brief Pop a data from the back in the doubly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the list.
 * \return 1 success or 0 fail.
 */
int dList_popBack(dList **listRef);

/**
 * \brief Appends a sublist to the end of a doubly linked list.
 *
 * \param[in] list The head of the doubly linked list.
 * \param[in,out] append A pointer to the pointer to the head of the sublist to append.
 * \return 1 success or 0 fail.
 */
int dList_append(dList *list, dList **append);

/**
 * \brief Creates a copy of a sublist within a doubly linked list.
 *
 * \param[in] list The head of the doubly linked list.
 * \param[in] begin The index at begin position which to copy the sublist (0-based index, negative-Reverse starting from `-1`).
 * \param[in] end The index at end position which to copy the sublist (0-based index, negative-Reverse starting from `-1`).
 * \return A pointer to the head of the copied sublist, or NULL if invalid parameters are provided or the copying process fails.
 */
dList *dList_copy(dList *list, int begin, int end);

/**
 * \brief Reverses a sublist within a doubly linked list.
 *
 * \param[in] list The head of the doubly linked list.
 * \param[in] begin The index at begin position which to reverse the sublist (0-based index, negative-Reverse starting from `-1`).
 * \param[in] end The index at end position which to reverse the sublist (0-based index, negative-Reverse starting from `-1`).
 * \return 1 if the sublist was successfully reversed, 0 if invalid parameters are provided or the reversal process fails.
 */
int dList_reverse(dList *list, int begin, int end);

/**
 * \brief Calculates the size (number of nodes) of a doubly linked list.
 *
 * \param[in] list The head of the doubly linked list.
 * \return The size (number of nodes) of the list.
 */
int dList_size(dList *list);

/**
 * \brief Moves to a specific position in a doubly linked list.
 *
 * \param[in] list The head of the doubly linked list.
 * \param[in] index The index of the desired position (0-based index, negative-Reverse starting from `-1`).
 * \return A pointer to the node at the specified position, or NULL if invalid parameters are provided or the index is out of bounds.
 */
dList *dList_to(dList *list, int index);

/**
 * \brief Sets the data and size of a node in a doubly linked list.
 *
 * \param[in] list The node in the doubly linked list.
 * \param[in] data A pointer to the data to be set.
 * \param[in] size The size of the data to be set.
 * \return 1 if the data was successfully set, 0 if invalid parameters are provided.
 */
int dList_set(dList *list, void* data, int size);

/**
 * \brief Retrieves the data from a node in a doubly linked list.
 *
 * \param[in] list The node in the doubly linked list.
 * \param[out] data A pointer to the memory location where the data will be retrieved.
 * \param[in] size The size of the data to be retrieved.
 * \return 1 if the data was successfully retrieved, 0 if invalid parameters are provided or the retrieval process fails.
 */
int dList_get(dList *list, void* data, int size);

/**
 * \brief Macro for forward iterating over each node in a doubly linked list.
 *
 * \param[in] list The head of the doubly linked list.
 * \param[in] node The iterator variable to represent each node in the list.
 */
#define dList_forEach(list, node)           dList_forEachForward(list, node)
#define dList_forEachForward(list, node) \
    for (dList* (node) = (list); (node); (node) = (((node)->next==(list))?NULL:(node)->next))

/**
 * \brief Macro for reverse iterating over each node in a doubly linked list.
 *
 * \param[in] list The head of the doubly linked list.
 * \param[in] node The iterator variable to represent each node in the list.
 */
#define dList_forEachReverse(list, node) \
    for (dList* (node) = (list)->prev; (node); (node) = (((node)==(list))?NULL:(node)->prev))

/**
 * \brief Macro for referencing the data of a node in a doubly linked list.
 *
 * \param[in] node The node in the doubly linked list.
 * \param[in] type The type of the data stored in the node.
 */
#define dList_ref(node, type)               (*(type *)((node)->data))

/* Standard front and back indexes */
#define dList_front                         (0)
#define dList_back                          (-1)

#endif
