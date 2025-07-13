/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  tree.h
 *         \unit  tree
 *        \brief  This is a C language tree, general data structure
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __tree_H
#define __tree_H

#include <stdlib.h>
#include <stdarg.h>

/* version infomation */

#define TREE_V_MAJOR                        1
#define TREE_V_MINOR                        0
#define TREE_V_PATCH                        0

/* tree type definition, hiding structural members, not for external use */

typedef struct TREE* tree_t;

/**
 *  \brief create a null tree.
 *  \return tree handler or NULL fail
 */
tree_t tree_create(void);

/**
 *  \brief delete a tree.
 *  \param[in] tree: tree handler
 *  \param[in] func: additional execution function
 *  \return none
 */
void tree_delete(tree_t tree, void (*func)(tree_t tree));

/**
 *  \brief insert an empty child space at the specified index position.
 *  \param[in] tree: tree handler
 *  \param[in] index: index
 *  \return 1 success or 0 fail
 */
int tree_insert(tree_t tree, int index);

/**
 *  \brief Erase the empty child space of the specified index.
 *  \param[in] tree: tree handler
 *  \param[in] index: index
 *  \return 1 success or 0 fail
 */
int tree_erase(tree_t tree, int index);

/**
 *  \brief attach an independent tree to a specified index of another tree.
 *  \param[in] tree: tree handler
 *  \param[in] index: index
 *  \param[in] attach: independent tree
 *  \return 1 success or 0 fail
 */
int tree_attach(tree_t tree, int index, tree_t attach);

/**
 *  \brief detach the tree with the specified index.
 *  \param[in] tree: tree handler
 *  \param[in] index: index
 *  \return the detach tree or NULL fail
 */
tree_t tree_detach(tree_t tree, int index);

/**
 *  \brief get the parent tree.
 *  \param[in] tree: tree handler
 *  \return parent tree or NULL fail
 */
tree_t tree_parent(tree_t tree);

/**
 *  \brief get the child tree.
 *  \param[in] tree: tree handler
 *  \param[in] index: index
 *  \return child tree or NULL fail
 */
tree_t tree_child(tree_t tree, int index);

/**
 *  \brief get the child size(count of children).
 *  \param[in] tree: tree handler
 *  \return child size
 */
int tree_csize(tree_t tree);

/**
 *  \brief get the data of the tree.
 *  \param[in] tree: tree handler
 *  \return data address or NULL fail
 */
const void* tree_data(tree_t tree);

/**
 *  \brief get the data size.
 *  \param[in] tree: tree handler
 *  \return data size
 */
int tree_dsize(tree_t tree);

/**
 *  \brief get the attribute of the tree.
 *  \param[in] tree: tree handler
 *  \return attribute address or NULL fail
 */
const void* tree_attribute(tree_t tree);

/**
 *  \brief get the attribute size.
 *  \param[in] tree: tree handler
 *  \return attribute size
 */
int tree_asize(tree_t tree);

/**
 *  \brief get the size of tree(count of all node).
 *  \param[in] tree: tree handler
 *  \return tree size
 */
int tree_size(tree_t tree);

/**
 *  \brief get the depth of tree(include itself).
 *  \param[in] tree: tree handler
 *  \return tree depth
 */
int tree_depth(tree_t tree);

/**
 *  \brief successive indexes get a child tree.
 *  \param[in] tree: tree handler
 *  \param[in] index: index
 *  \param[in] ...: other indexs, continuous index stops until a negative number
 *  \return child tree or NULL fail
 */
tree_t tree_to_valist(tree_t tree, int index, ...);

/**
 *  \brief set data to tree.
 *  \param[in] tree: tree handler
 *  \param[in] data: address of data, will overwrite the original data
 *  \param[in] size: size of data, the original data will be deleted if it's 0
 *  \return 1 success or 0 fail
 */
int tree_set_data(tree_t tree, void* data, int size);

/**
 *  \brief get the data of tree.
 *  \param[in] tree: tree handler
 *  \param[in] data: address of data
 *  \param[in] size: size of data, need to fit the tree data
 *  \return 1 success or 0 fail
 */
int tree_get_data(tree_t tree, void* data, int size);

/**
 *  \brief set attribute to tree.
 *  \param[in] tree: tree handler
 *  \param[in] attribute: address of attribute, will overwrite the original attribute
 *  \param[in] size: size of attribute, the original attribute will be deleted if it's 0
 *  \return 1 success or 0 fail
 */
int tree_set_attribute(tree_t tree, void* attribute, int size);

/**
 *  \brief get the attribute of tree.
 *  \param[in] tree: tree handler
 *  \param[in] attribute: address of attribute
 *  \param[in] size: size of attribute, need to fit the tree attribute
 *  \return 1 success or 0 fail
 */
int tree_get_attribute(tree_t tree, void* attribute, int size);

/**
 *  \brief tree expansion print.
 *  \param[in] tree: tree handler
 *  \param[in] depth: depth to print, 0 is print all
 *  \param[in] print: print function
 *  \return none
 */
void tree_print(tree_t tree, int depth, void (*print)(tree_t tree));

/**
 *  \brief successive indexes get a child tree, a simple method for `tree_to_valist`.
 *  \param[in] tree: tree handler
 *  \param[in] i: index
 *  \param[in] ...: other indexs, continuous index stops until a negative number
 *  \return child tree or NULL fail
 */
#define tree_to(tree, i, ...)                   (tree_to_valist(tree,(i),##__VA_ARGS__,-1))

#endif 
