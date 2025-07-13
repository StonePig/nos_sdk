/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  map.c
 *         \unit  map
 *        \brief  This is a general-purpose C language map module, with common data structure
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "map.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* key transfer function type define */
typedef void* (*ktrans_t)(map_t map, va_list args);

/* map key tpye define */
typedef struct 
{
    void* address;                      /**< address of key */
    int size;                           /**< size of key */
} MKEY;

/* map node type define */
typedef struct NODE
{
    struct NODE *parent;                /**< parent node */
    struct NODE *left;                  /**< left child node */
    struct NODE *right;                 /**< right child node */
    int color;                          /**< node color */
    MKEY key;                           /**< node key */
} NODE;
#define data(node) ((node)+1)           /**< data of node */

/* map type define */
typedef struct MAP
{
    NODE* root;                         /**< root node */
    NODE* nil;                          /**< nil node */
    NODE* iterator;                     /**< iterator of map */
    int orgin;                          /**< iterator orgin */
    int size;                           /**< map size */
    int vsize;                          /**< value size */
    int ksize;                          /**< key size */
    ktrans_t trans;                     /**< key transfer function */
} MAP;

/* map node color */
#define BLACK                           (0)
#define RED                             (1)

map_t map_create(int vsize, int ksize, void *trans)
{
    map_t map;

    /* Input value validity check */
    if (vsize <= 0) return NULL;
    if (ksize < 0) return NULL;

    /* Allocate memory for the MAP structure */
    map = (map_t)malloc(sizeof(MAP));
    if (!map) return NULL;

    /* Allocate memory for the nil node */
    map->nil = (NODE*)malloc(sizeof(NODE) + vsize);
    if (!map->nil)
    {
        free(map);
        return NULL;
    }

    /* Fixed length key */
    if (ksize != 0)
    {
        map->nil->key.address = malloc(ksize);
        if (!map->nil->key.address)
        {
            free(map->nil);
            free(map);
            return NULL;
        }
    }

    /* Initialize structural parameters */
    map->nil->key.size = ksize;
    map->nil->color = BLACK;
    map->nil->parent = map->nil;
    map->nil->left = map->nil;
    map->nil->right = map->nil;
    map->iterator = map->nil;
    map->root = map->nil;
    map->vsize = vsize;
    map->size = 0;
    map->ksize = ksize;
    map->trans = trans;

    return map;
}

static void recursion_delete_node(map_t map, NODE* node)
{
    if (node == map->nil) return;
    recursion_delete_node(map, node->left);
    recursion_delete_node(map, node->right);
    free(node->key.address);
    free(node);
}

void map_delete(map_t map)
{
    /* Input value validity check */
    if (!map) return;

    /* Clear map */
    map_clear(map);

    /* Free allocated space */
    if (map->ksize) free(map->nil->key.address);
    free(map->nil);
    free(map);
}

void* map_trans_key(void* map, void *address, int size)
{
    if (((map_t)map)->ksize == 0)
    {
        ((map_t)map)->nil->key.address = address;
        ((map_t)map)->nil->key.size = size;
    }
    else 
    {
        if (((map_t)map)->ksize == size)
        {
            memcpy(((map_t)map)->nil->key.address, address, ((map_t)map)->ksize);
            ((map_t)map)->nil->key.size = size;
        }
        else  
        {
            ((map_t)map)->nil->key.size = 0;
        }
    }
    return &(((map_t)map)->nil->key);
}

static int key_compare(MKEY key0, MKEY key1)
{
    unsigned char *add0 = (unsigned char *)key0.address;
    unsigned char *add1 = (unsigned char *)key1.address;
    while (key0.size && key1.size)
    {
        key0.size--;
        key1.size--;
        if (*add0 < *add1) return -1;
        else if (*add0 > *add1) return 1;
        add0++;
        add1++;
    }
    if (key0.size < key1.size) return -1;
    else if (key0.size > key1.size) return 1;
    return 0;
}

static NODE* map_find_node(map_t map, MKEY key)
{
    NODE* node = map->root;
    int cmp = 0;

    /* Loop through the left and right branches until they match the key */
    while (node != map->nil)
    {
        cmp = key_compare(key, node->key);
        if (cmp < 0) node = node->left;
        else if (cmp > 0) node = node->right;
        else return node;
    }
    return map->nil;
}

static void left_rotate(map_t map, NODE* x) 
{
    NODE* y = x->right; /* Get the right child of x */

    /* Set x's right child to y's left child */
    x->right = y->left;
    /* Update the parent of y's left child if it is not nil */
    if (y->left != map->nil) y->left->parent = x;

    /* Set y's parent to x's parent */
    y->parent = x->parent;

    /* If x is the root, update the root to y */
    if (x->parent == map->nil) map->root = y;
    /* If x is the left child of its parent, update the left child of its parent to y */
    else if (x == x->parent->left) x->parent->left = y;
    /* update the right child of its parent to y */
    else x->parent->right = y;

    y->left = x;
    x->parent = y;
}

static void right_rotate(map_t map, NODE* y) 
{
    NODE* x = y->left; /* Get the left child of y */

    /* Set y's left child to x's right child */
    y->left = x->right;
    /* Update the parent of x's right child if it is not nil */
    if (x->right != map->nil) x->right->parent = y;

    /* Set x's parent to y's parent */
    x->parent = y->parent;

    /* If y is the root, update the root to x */
    if (y->parent == map->nil) map->root = x;
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
 * \param[in] map The map structure containing the red-black tree.
 * \param[in] z The newly inserted node in the red-black tree.
 */
static void map_insert_fixup(map_t map, NODE* z) 
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
                    left_rotate(map, z);
                }
                // Case 3: z's uncle y is BLACK and z is a left child
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(map, z->parent->parent);
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
                    right_rotate(map, z);
                }
                // Case 3: z's uncle y is BLACK and z is a right child
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(map, z->parent->parent);
            }
        }
    }
    map->root->color = BLACK;
}

/**
 * \brief Inserts a node into a red-black tree and performs fix-up operations to maintain the red-black tree properties.
 *
 * \param[in] map The map structure containing the red-black tree.
 * \param[in] z The node to be inserted into the red-black tree.
 * \return 1 if the node was successfully inserted, 0 if a node with the same index already exists.
 */
static int map_insert_node(map_t map, NODE* z)
{
    NODE* y = map->nil;  // Initialize y as the sentinel node
    NODE* x = map->root; // Start at the root of the red-black tree

    while (x != map->nil)
    {
        y = x;
        // Move to the left child if z's index is less than the current node's index
        if (key_compare(z->key, x->key) < 0) x = x->left;
        // Move to the right child if z's index is greater than the current node's index
        else if (key_compare(z->key, x->key) > 0) x = x->right;
        // Return 0 if a node with the same index already exists
        else return 0;
    }

    z->parent = y; // Set z's parent to y

    // Insert z as a child of y based on the comparison of their indices
    // If y is the sentinel node, set z as the root of the red-black tree
    if (y == map->nil) map->root = z;
    // Set z as the left child of y
    else if (key_compare(z->key, y->key) < 0) y->left = z;
    // Set z as the right child of y
    else y->right = z;

    z->left = map->nil;  // Set z's left child to the sentinel node
    z->right = map->nil; // Set z's right child to the sentinel node
    z->color = RED;     // Set z's color to red (since it is inserted as a leaf node)

    map_insert_fixup(map, z); // Perform fix-up operations to maintain the red-black tree properties

    return 1; // Return 1 to indicate successful insertion
}

void* map_insert(map_t map, ...)
{
    NODE* node;
    int child = 0;
    MKEY *key;
    void* value;
    va_list args;

    va_start(args, value);
    value = va_arg(args, void*);
    key = (MKEY*)(map->trans(map, args));
    va_end(args);

    /* Input value validity check */
    if (key->size == 0) return NULL;
    if (!map) return NULL;

    /* Allocate memory for the node */
    node = (NODE*)malloc(sizeof(NODE) + map->vsize);
    if (!node) return NULL;

    /* Allocate memory for the key */
    node->key.address = malloc(key->size);
    if (!node->key.address) { free(node); return NULL; }

    /* Assign key */
    node->key.size = key->size;
    memcpy(node->key.address, key->address, key->size);

    /* Assign value */
    if (value) memcpy(data(node), value, map->vsize);

    /* Insert node into tree */
    if (!map_insert_node(map, node)) 
    { 
        free(node->key.address); 
        free(node); 
        return NULL; 
    }

    /* Update map status */
    map->size++;

    /* Return node data */
    return data(node);
}

/**
 * \brief Performs fix-up operations after deleting a node from a red-black tree.
 *
 * \param[in] map The map structure containing the red-black tree.
 * \param[in] x The replacement node in the red-black tree.
 */
static void map_erase_fixup(map_t map, NODE* x) 
{
    NODE* w = NULL;

    while ((x != map->root) && (x->color == BLACK))
    {
        if (x == x->parent->left)
        {
            w = x->parent->right;

            if (w->color == RED)
            {
                // Case 1: x's sibling w is RED
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(map, x->parent);
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
                    right_rotate(map, w);
                    w = x->parent->right;
                }
                // Case 4: x's sibling w is BLACK and w's right child is RED
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(map, x->parent);
                x = map->root;
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
                right_rotate(map, x->parent);
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
                    left_rotate(map, w);
                    w = x->parent->left;
                }
                // Case 4: x's sibling w is BLACK and w's left child is RED
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(map, x->parent);
                x = map->root;
            }
        }
    }
    x->color = BLACK;
}

static NODE* node_min(map_t map, NODE* x)
{
    if (x == map->nil) return x;
    while (x->left != map->nil) x = x->left;
    return x;
}

static NODE* node_max(map_t map, NODE* x)
{
    if (x == map->nil) return x;
    while (x->right != map->nil) x = x->right;
    return x;
}

/**
 * \brief Finds the successor of a node in a red-black tree.
 *
 * \param[in] map The map structure containing the red-black tree.
 * \param[in] x The node for which to find the successor.
 * \return A pointer to the successor node, or the sentinel node if the successor does not exist.
 */
static NODE* map_successor(map_t map, NODE* x) 
{
    NODE* y = x->parent;  // Initialize y as x's parent

    // If x has a right child, the successor is the minimum node in x's right subtree
    if (x->right != map->nil) return node_min(map, x->right);

    // If x does not have a right child, find the closest ancestor y such that x is in y's left subtree
    while ((y != map->nil) && (x == y->right))
    {
        x = y;
        y = y->parent;
    }

    return y; // Return the successor node (or the sentinel node if the successor does not exist)
}

/**
 * \brief Removes a node from a red-black tree and performs fix-up operations to maintain the red-black tree properties.
 *
 * \param[in] map The map structure containing the red-black tree.
 * \param[in] z The node to be erased from the red-black tree.
 * \return The removed node.
 */
static NODE* map_erase_node(map_t map, NODE* z) 
{
    NODE* y = map->nil; // Initialize y as the sentinel node
    NODE* x = map->nil; // Initialize x as the sentinel node

    // Determine the node to be removed (y) based on the number of children of z
    if ((z->left == map->nil) || (z->right == map->nil))
    {
        y = z;
    }
    else 
    {
        y = map_successor(map, z);
    }

    // Determine the child of y (x) based on whether y has a left child or a right child
    if (y->left != map->nil) 
    {
        x = y->left;
    }
    else if (y->right != map->nil) 
    {
        x = y->right;
    }

    x->parent = y->parent; // Set x's parent to y's parent
    // Update y's parent's child pointer to x
    if (y->parent == map->nil) 
    {
        map->root = x; // If y is the root of the red-black tree, set x as the new root
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
        MKEY temp;
        temp = z->key;
        z->key = y->key;
        y->key = temp;
        memcpy(data(z), data(y), map->vsize);
    }

    if (y->color == BLACK) map_erase_fixup(map, x); // Perform fix-up operations to maintain the red-black tree properties

    return y; // Return the removed node
}

int map_erase(map_t map, ...)
{
    NODE* node = NULL;
    NODE* cur = NULL;
    va_list args;
    MKEY key;

    va_start(args, map);
    key = *(MKEY *)(map->trans(map, args));
    va_end(args);

    /* Input value validity check */
    if (!map) return 0;

    /* Find the specified node */
    node = map_find_node(map, key);
    if (node == map->nil) return 0;

    /* Detach the node from the tree */
    cur = map_erase_node(map, node);

    /* Free the current node */
    free(cur->key.address);
    free(cur);

    /* Update queue status */
    map->size--;

    return 1;
}

void map_clear(map_t map)
{
    /* Input value validity check */
    if (!map) return;

    /* Recursively delete each node */
    recursion_delete_node(map, map->root);

    /* Map root node to nil */
    map->root = map->nil;

    /* Update queue status */
    map->size = 0;
}

int map_size(map_t map)
{
    /* Input value validity check */
    if (!map) return 0;

    /* Return size */
    return map->size;
}

int map_ksize(map_t map)
{
    /* Input value validity check */
    if (!map) return 0;

    /* Return key size */
    return map->ksize;
}

int map_vsize(map_t map)
{
    /* Input value validity check */
    if (!map) return 0;

    /* Return value size */
    return map->vsize;
}

int map_find(map_t map, ...)
{
    va_list args;
    MKEY key;

    /* Input value validity check */
    if (!map) return 0;

    va_start(args, map);
    key = *(MKEY *)(map->trans(map, args));
    va_end(args);

    /* Determine if the index can be found */
    return map_find_node(map, key) == map->nil ? 0 : 1;
}

void* map_data(map_t map, ...)
{
    va_list args;
    MKEY key;

    /* Input value validity check */
    if (!map) return NULL;

    va_start(args, map);
    key = *(MKEY *)(map->trans(map, args));
    va_end(args);

    /* Return data for node */
    return data(map_find_node(map, key));
}

void* map_error(map_t map)
{
    /* Input value validity check */
    if (!map) return NULL;

    /* Return data for nil node */
    return data(map->nil);
}

static NODE* node_next(map_t map, NODE* node)
{
    if (node->right != map->nil)
    {
        node = node->right;
        node = node_min(map, node);
    }
    else  
    {
        if (node == node->parent->left) node = node->parent;
        else node = node->parent->parent;
    }
    return node;
}

static NODE* node_prev(map_t map, NODE* node)
{
    if (node->left != map->nil)
    {
        node = node->left;
        node = node_max(map, node);
    }
    else  
    {
        if (node == node->parent->right) node = node->parent;
        else node = node->parent->parent;
    }
    return node;
}

void map_it_init(map_t map, int orgin)
{
    /* Input value validity check */
    if (!map) return;

    /* Update origin */
    map->orgin = (orgin == MAP_HEAD) ? MAP_HEAD : MAP_TAIL;

    /* Locate the maximum or minimum node based on the origin */
    map->iterator = (map->orgin == MAP_HEAD) ? (NODE*)node_min(map, map->root) : (NODE*)node_max(map, map->root);
}

void* map_it_get(map_t map, void **kaddress, int *ksize)
{
    NODE *node;

    /* Input value validity check */
    if (!map) return NULL;

    /* Starting from the current iterator */
    node = map->iterator;

    /* Move iterator */
    map->iterator = (map->orgin == MAP_HEAD) ? node_next(map, map->iterator) : node_prev(map, map->iterator);
    
    /* Output the current iterator key */
    if (kaddress) *kaddress = node->key.address;
    if (ksize) *ksize = node->key.size;

    return data(node);
}
