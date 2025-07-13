/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  sList.h
 *         \unit  sList
 *        \brief  This is a C language singly linked list
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __sList_H
#define __sList_H

#include <stdlib.h>

/* Version infomation */

#define SLIST_V_MAJOR                       1
#define SLIST_V_MINOR                       0
#define SLIST_V_PATCH                       1

/* Type of sList */
typedef struct sList
{
    /**< Singly linked list, pointing to the next node in the linked list */
    struct sList *next;

    /**< The base address for storing data, allocated in the API */
    void *data;
    
    /**< The size of node data */
    int size;
} sList;


/**
 * \brief Creates a new singly linked list.
 *
 * \return A pointer to the newly created sList structure, or NULL if memory allocation fails.
 */
sList *sList_create(void);

/**
 * \brief Deletes a singly linked list and frees the associated memory.
 *
 * \param[in] list The sList structure to delete.
 */
void sList_delete(sList *list);

/**
 * \brief Attaches a sublist to a specified position in a singly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the original list.
 * \param[in] index The index at which to attach the sublist (0-based index, negative-end index).
 * \param[in] attach The sublist to attach.
 * \return A pointer to the attach head of the modified list, or NULL if invalid parameters are provided.
 */
sList *sList_attach(sList **listRef, int index, sList *attach);

/**
 * \brief Detaches a sublist from a specified position in a singly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the list.
 * \param[in] index The index at which to detach the sublist (0-based index, negative-end index).
 * \param[in] count The number of nodes to detach from the specified position (-1 to detach all nodes until the end).
 * \param[out] outPrev A pointer to store the pointer to the previous node of the detached sublist.
 * \return A pointer to the head of the detached sublist, or NULL if invalid parameters are provided.
 */
sList *sList_detach(sList **listRef, int index, int count, sList **outPrev);

/**
 * \brief Inserts a node with the specified data at a specified position in a singly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the list.
 * \param[in] index The index at which to insert the node (0-based index, negative-end index).
 * \param[in] data A pointer to the data to be inserted.
 * \param[in] size The size of the data to be inserted.
 * \return A pointer to the newly inserted node, or NULL if memory allocation fails or invalid parameters are provided.
 */
sList *sList_insert(sList **listRef, int index, void *data, int size);

/**
 * \brief Erases a node at the specified position in a singly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the list.
 * \param[in] index The index of the node to erase (0-based index, negative-end index).
 * \param[out] outPrev A pointer to store the pointer to the previous node of the erased node.
 * \return 1 if the node was successfully erased, 0 if invalid parameters are provided or the index is out of bounds.
 */
int sList_erase(sList **listRef, int index, sList **outPrev);

/**
 * \brief Push a data from the front to the singly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the list.
 * \param[in] data A pointer to the data to be inserted.
 * \param[in] size The size of the data to be inserted.
 * \return 1 success or 0 fail.
 */
int sList_pushFront(sList **listRef, void *data, int size);

/**
 * \brief Push a data from the back to the singly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the list.
 * \param[in] data A pointer to the data to be inserted.
 * \param[in] size The size of the data to be inserted.
 * \return 1 success or 0 fail.
 */
int sList_pushBack(sList **listRef, void *data, int size);

/**
 * \brief Pop a data from the front in the singly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the list.
 * \return 1 success or 0 fail.
 */
int sList_popFront(sList **listRef);

/**
 * \brief Pop a data from the back in the singly linked list.
 *
 * \param[in,out] listRef A pointer to the pointer to the head of the list.
 * \return 1 success or 0 fail.
 */
int sList_popBack(sList **listRef);

/**
 * \brief Appends a sublist to the end of a singly linked list.
 *
 * \param[in] list The head of the singly linked list.
 * \param[in,out] append A pointer to the pointer to the head of the sublist to append.
 * \return 1 success or 0 fail.
 */
int sList_append(sList *list, sList **append);

/**
 * \brief Creates a copy of a sublist within a singly linked list.
 *
 * \param[in] list The head of the singly linked list.
 * \param[in] begin The starting index of the sublist (0-based index, negative-end index).
 * \param[in] end The ending index of the sublist (0-based index, negative-end index).
 * \return A pointer to the head of the copied sublist, or NULL if invalid parameters are provided or the copying process fails.
 */
sList *sList_copy(sList *list, int begin, int end);

/**
 * \brief Reverses a sublist within a singly linked list.
 *
 * \param[in] list The head of the singly linked list.
 * \param[in] begin The starting index of the sublist (0-based index, negative-end index).
 * \param[in] end The ending index of the sublist (0-based index, negative-end index).
 * \return 1 if the sublist was successfully reversed, 0 if invalid parameters are provided or the reversal process fails.
 */
int sList_reverse(sList *list, int begin, int end);

/**
 * \brief Calculates the size (number of nodes) of a singly linked list.
 *
 * \param[in] list The head of the singly linked list.
 * \return The size (number of nodes) of the list.
 */
int sList_size(sList *list);

/**
 * \brief Moves to a specific position in a singly linked list.
 *
 * \param[in] list The head of the singly linked list.
 * \param[in] index The index of the desired position (0-based index, negative-end index).
 * \return A pointer to the node at the specified position, or NULL if invalid parameters are provided or the index is out of bounds.
 */
sList *sList_to(sList *list, int index);

/**
 * \brief Sets the data and size of a node in a singly linked list.
 *
 * \param[in] list The node in the singly linked list.
 * \param[in] data A pointer to the data to be set.
 * \param[in] size The size of the data to be set.
 * \return 1 if the data was successfully set, 0 if invalid parameters are provided.
 */
int sList_set(sList *list, void* data, int size);

/**
 * \brief Retrieves the data from a node in a singly linked list.
 *
 * \param[in] list The node in the singly linked list.
 * \param[out] data A pointer to the memory location where the data will be retrieved.
 * \param[in] size The size of the data to be retrieved.
 * \return 1 if the data was successfully retrieved, 0 if invalid parameters are provided or the retrieval process fails.
 */
int sList_get(sList *list, void* data, int size);

/**
 * \brief Macro for iterating over each node in a singly linked list.
 *
 * \param[in] list The head of the singly linked list.
 * \param[in] node The iterator variable to represent each node in the list.
 */
#define sList_forEach(list, node)           for (sList* (node) = (list); (node); (node) = (node)->next)

/**
 * \brief Macro for referencing the data of a node in a singly linked list.
 *
 * \param[in] node The node in the singly linked list.
 * \param[in] type The type of the data stored in the node.
 */
#define sList_ref(node, type)               (*(type *)((node)->data))

/* Standard front and back indexes */
#define sList_front                         (0)
#define sList_back                          (-1)

#endif
