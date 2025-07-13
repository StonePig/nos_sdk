/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  set.c
 *         \unit  set
 *        \brief  This is a general-purpose C language set module, with common data structure
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "set.h"
#include <stdio.h>
#include <string.h>

/* set node type define */
typedef struct NODE
{
    struct NODE *parent;                /**< parent node */
    struct NODE *left;                  /**< left child node */
    struct NODE *right;                 /**< right child node */
    int color;                          /**< node color */
    int index;                          /**< node index */
} NODE;
#define data(node) ((node)+1)           /**< data of node */

/* set type define */
typedef struct SET
{
    NODE* root;                         /**< root node */
    NODE* nil;                          /**< nil node */
    NODE* iterator;                     /**< iterator of set */
    int orgin;                          /**< iterator orgin */
    int size;                           /**< set size */
    int dsize;                          /**< data size */
} SET;

/* set node color */
#define BLACK                           (0)
#define RED                             (1)

set_t set_create(int dsize)
{
    set_t set;

    /* Input value validity check */
    if (dsize <= 0) return NULL;

    /* Allocate memory for the SET structure */
    set = (set_t)malloc(sizeof(SET));
    if (!set) return NULL;

    /* Allocate memory for the nil node */
    set->nil = (NODE*)malloc(sizeof(NODE) + dsize);
    if (!set->nil) 
    {
        free(set); 
        return NULL;
    }

    /* Initialize structural parameters */
    set->nil->color = BLACK;
    set->nil->parent = set->nil;
    set->nil->left = set->nil;
    set->nil->right = set->nil;
    set->iterator = set->nil;
    set->root = set->nil;
    set->dsize = dsize;
    set->size = 0;

    return set;
}

static void recursion_delete_node(set_t set, NODE* node)
{
    if (node == set->nil) return;
    recursion_delete_node(set, node->left);
    recursion_delete_node(set, node->right);
    free(node);
}

void set_delete(set_t set)
{
    /* Input value validity check */
    if (!set) return;

    /* Clear set */
    set_clear(set);

    /* Free allocated space */
    free(set->nil);
    free(set);
}

static NODE* set_find_node(set_t set, int index)
{
    NODE* node = set->root;

    /* Loop through the left and right branches until they match the index */
    while (node != set->nil)
    {
        if (index < node->index) node = node->left;
        else if (index > node->index) node = node->right;
        else return node;
    }

    return set->nil;
}

static void left_rotate(set_t set, NODE* x) 
{
    NODE* y = x->right; /* Get the right child of x */

    /* Set x's right child to y's left child */
    x->right = y->left; 
    /* Update the parent of y's left child if it is not nil */
    if (y->left != set->nil) y->left->parent = x;

    /* Set y's parent to x's parent */
    y->parent = x->parent;

    /* If x is the root, update the root to y */
    if (x->parent == set->nil) set->root = y;
    /* If x is the left child of its parent, update the left child of its parent to y */
    else if (x == x->parent->left) x->parent->left = y;
    /* update the right child of its parent to y */
    else x->parent->right = y;

    y->left = x;
    x->parent = y;
}


static void right_rotate(set_t set, NODE* y) 
{
    NODE* x = y->left; /* Get the left child of y */

    /* Set y's left child to x's right child */
    y->left = x->right;
    /* Update the parent of x's right child if it is not nil */
    if (x->right != set->nil) x->right->parent = y;

    /* Set x's parent to y's parent */
    x->parent = y->parent;

    /* If y is the root, update the root to x */
    if (y->parent == set->nil) set->root = x;
    /* If y is the right child of its parent, update the right child of its parent to x */
    else if (y == y->parent->right) y->parent->right = x;
    /* update the left child of its parent to x */
    else y->parent->left = x;

    x->right = y;
    y->parent = x;
}

/**
 * \brief Performs fix-up operations after inserting a node into a red-black tree.
 *
 * \param[in] set The set structure containing the red-black tree.
 * \param[in] z The newly inserted node in the red-black tree.
 */
static void set_insert_fixup(set_t set, NODE* z) 
{
    NODE* y = NULL;

    while (z->parent->color == RED)
    {
        if (z->parent == z->parent->parent->left)
        {
            y = z->parent->parent->right;
            if (y->color == RED)
            {
                // Case 1: z's uncle y is RED
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    // Case 2: z's uncle y is BLACK and z is a right child
                    z = z->parent;
                    left_rotate(set, z);
                }
                // Case 3: z's uncle y is BLACK and z is a left child
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(set, z->parent->parent);
            }
        }
        else
        {
            y = z->parent->parent->left;
            if (y->color == RED)
            {
                // Case 1: z's uncle y is RED
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    // Case 2: z's uncle y is BLACK and z is a left child
                    z = z->parent;
                    right_rotate(set, z);
                }
                // Case 3: z's uncle y is BLACK and z is a right child
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(set, z->parent->parent);
            }
        }
    }
    set->root->color = BLACK;
}

/**
 * \brief Inserts a node into a red-black tree and performs fix-up operations to maintain the red-black tree properties.
 *
 * \param[in] set The set structure containing the red-black tree.
 * \param[in] z The node to be inserted into the red-black tree.
 * \return 1 if the node was successfully inserted, 0 if a node with the same index already exists.
 */
static int set_insert_node(set_t set, NODE* z)
{
    NODE* y = set->nil;  // Initialize y as the sentinel node
    NODE* x = set->root; // Start at the root of the red-black tree

    while (x != set->nil)
    {
        y = x;
        if (z->index < x->index)
        {
            x = x->left; // Move to the left child if z's index is less than the current node's index
        }
        else if (z->index > x->index)
        {
            x = x->right; // Move to the right child if z's index is greater than the current node's index
        }
        else
        {
            return 0; // Return 0 if a node with the same index already exists
        }
    }

    z->parent = y; // Set z's parent to y

    // Insert z as a child of y based on the comparison of their indices
    if (y == set->nil)
    {
        set->root = z; // If y is the sentinel node, set z as the root of the red-black tree
    }
    else if (z->index < y->index)
    {
        y->left = z; // Set z as the left child of y
    }
    else
    {
        y->right = z; // Set z as the right child of y
    }

    z->left = set->nil;  // Set z's left child to the sentinel node
    z->right = set->nil; // Set z's right child to the sentinel node
    z->color = RED;     // Set z's color to red (since it is inserted as a leaf node)

    set_insert_fixup(set, z); // Perform fix-up operations to maintain the red-black tree properties

    return 1; // Return 1 to indicate successful insertion
}

void* set_insert(set_t set, int index, void* data)
{
    NODE* node;

    /* Input value validity check */
    if (!set) return NULL;

    /* Allocate memory for the node */
    node = (NODE*)malloc(sizeof(NODE) + set->dsize);
    if (!node) return NULL;

    /* Record index */
    node->index = index;

    /* Assign values to data */
    if (data) memcpy(data(node), data, set->dsize);

    /* Insert node into tree */
    if (!set_insert_node(set, node)) 
    { 
        free(node); 
        return NULL; 
    }

    /* Update set status */
    set->size++;

    /* Return node data */
    return data(node);
}

/**
 * \brief Performs fix-up operations after deleting a node from a red-black tree.
 *
 * \param[in] set The set structure containing the red-black tree.
 * \param[in] x The replacement node in the red-black tree.
 */
static void set_erase_fixup(set_t set, NODE* x) 
{
    NODE* w = NULL;

    while ((x != set->root) && (x->color == BLACK))
    {
        if (x == x->parent->left)
        {
            w = x->parent->right;

            if (w->color == RED)
            {
                // Case 1: x's sibling w is RED
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(set, x->parent);
                w = x->parent->right;
            }

            if ((w->left->color == BLACK) && (w->right->color == BLACK))
            {
                // Case 2: x's sibling w is BLACK and both w's children are BLACK
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->right->color == BLACK)
                {
                    // Case 3: x's sibling w is BLACK, w's left child is RED, and w's right child is BLACK
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(set, w);
                    w = x->parent->right;
                }
                // Case 4: x's sibling w is BLACK and w's right child is RED
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(set, x->parent);
                x = set->root;
            }
        }
        else
        {
            w = x->parent->left;
            if (w->color == RED)
            {
                // Case 1: x's sibling w is RED
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(set, x->parent);
                w = x->parent->left;
            }

            if ((w->left->color == BLACK) && (w->right->color == BLACK))
            {
                // Case 2: x's sibling w is BLACK and both w's children are BLACK
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == BLACK)
                {
                    // Case 3: x's sibling w is BLACK, w's right child is RED, and w's left child is BLACK
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(set, w);
                    w = x->parent->left;
                }
                // Case 4: x's sibling w is BLACK and w's left child is RED
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(set, x->parent);
                x = set->root;
            }
        }
    }
    x->color = BLACK;
}

static NODE* node_min(set_t set, NODE* x)
{
    if (x == set->nil) return x;
    while (x->left != set->nil) x = x->left;
    return x;
}

static NODE* node_max(set_t set, NODE* x)
{
    if (x == set->nil) return x;
    while (x->right != set->nil) x = x->right;
    return x;
}

/**
 * \brief Finds the successor of a node in a red-black tree.
 *
 * \param[in] set The set structure containing the red-black tree.
 * \param[in] x The node for which to find the successor.
 * \return A pointer to the successor node, or the sentinel node if the successor does not exist.
 */
static NODE* set_successor(set_t set, NODE* x) 
{
    NODE* y = x->parent; // Initialize y as x's parent

    // If x has a right child, the successor is the minimum node in x's right subtree
    if (x->right != set->nil)
    {
        return node_min(set, x->right);
    }

    // If x does not have a right child, find the closest ancestor y such that x is in y's left subtree
    while ((y != set->nil) && (x == y->right))
    {
        x = y;
        y = y->parent;
    }

    return y; // Return the successor node (or the sentinel node if the successor does not exist)
}

/**
 * \brief Removes a node from a red-black tree and performs fix-up operations to maintain the red-black tree properties.
 *
 * \param[in] set The set structure containing the red-black tree.
 * \param[in] z The node to be erased from the red-black tree.
 * \return The removed node.
 */
static NODE* set_erase_node(set_t set, NODE* z) 
{
    NODE* y = set->nil; // Initialize y as the sentinel node
    NODE* x = set->nil; // Initialize x as the sentinel node

    // Determine the node to be removed (y) based on the number of children of z
    if ((z->left == set->nil) || (z->right == set->nil))
    {
        y = z;
    }
    else
    {
        y = set_successor(set, z);
    }

    // Determine the child of y (x) based on whether y has a left child or a right child
    if (y->left != set->nil)
    {
        x = y->left;
    }
    else if (y->right != set->nil)
    {
        x = y->right;
    }

    x->parent = y->parent; // Set x's parent to y's parent

    // Update y's parent's child pointer to x
    if (y->parent == set->nil)
    {
        set->root = x; // If y is the root of the red-black tree, set x as the new root
    }
    else if (y == y->parent->left)
    {
        y->parent->left = x; // If y is the left child of its parent, set x as the new left child
    }
    else
    {
        y->parent->right = x; // If y is the right child of its parent, set x as the new right child
    }

    // Replace z's data with y's data if the removed node is not z
    if (y != z)
    {
        z->index = y->index;
        memcpy(data(z), data(y), set->dsize);
    }

    if (y->color == BLACK)
    {
        set_erase_fixup(set, x); // Perform fix-up operations to maintain the red-black tree properties
    }

    return y; // Return the removed node
}

int set_erase(set_t set, int index)
{
    NODE* node = NULL;
    NODE* cur = NULL;

    /* Input value validity check */
    if (!set) return 0;

    /* Find the specified node */
    node = set_find_node(set, index);
    if (node == set->nil) return 0;

    /* Detach the node from the tree */
    cur = set_erase_node(set, node);

    /* Free the current node */
    free(cur);

    /* Update queue status */
    set->size--;

    return 1;
}

void set_clear(set_t set)
{
    /* Input value validity check */
    if (!set) return;

    /* Recursively delete each node */
    recursion_delete_node(set, set->root);

    /* Set root node to nil */
    set->root = set->nil;

    /* Update queue status */
    set->size = 0;
}

int set_size(set_t set)
{
    /* Input value validity check */
    if (!set) return 0;

    /* Return size */
    return set->size;
}

int set_dsize(set_t set)
{
    /* Input value validity check */
    if (!set) return 0;

    /* Return data size */
    return set->dsize;
}

int set_find(set_t set, int index)
{
    /* Input value validity check */
    if (!set) return 0;

    /* Determine if the index can be found */
    return set_find_node(set, index) == set->nil ? 0 : 1;
}

void* set_data(set_t set, int index)
{
    /* Input value validity check */
    if (!set) return NULL;

    /* Return data for node */
    return data(set_find_node(set, index));
}

void* set_error(set_t set)
{
    /* Input value validity check */
    if (!set) return NULL;

    /* Return data for nil node */
    return data(set->nil);
}

static NODE* node_next(set_t set, NODE* node)
{
    if (node->right != set->nil)
    {
        node = node->right;
        node = node_min(set, node);
    }
    else  
    {
        if (node == node->parent->left) node = node->parent;
        else node = node->parent->parent;
    }
    return node;
}

static NODE* node_prev(set_t set, NODE* node)
{
    if (node->left != set->nil)
    {
        node = node->left;
        node = node_max(set, node);
    }
    else  
    {
        if (node == node->parent->right) node = node->parent;
        else node = node->parent->parent;
    }
    return node;
}

void set_it_init(set_t set, int orgin)
{
    /* Input value validity check */
    if (!set) return;

    /* Update origin */
    set->orgin = (orgin == SET_HEAD) ? SET_HEAD : SET_TAIL;

    /* Locate the maximum or minimum node based on the origin */
    set->iterator = (set->orgin == SET_HEAD) ? (NODE*)node_min(set, set->root) : (NODE*)node_max(set, set->root);
}

void* set_it_get(set_t set, int *out_index)
{
    NODE *node;

    /* Input value validity check */
    if (!set) return NULL;

    /* Starting from the current iterator */
    node = set->iterator;

    /* Move iterator */
    set->iterator = (set->orgin == SET_HEAD) ? node_next(set, set->iterator) : node_prev(set, set->iterator);

    /* Output the current iterator index */
    if (out_index) *out_index = node->index;

    return data(node);
}
