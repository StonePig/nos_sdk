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
#include "tree.h"
#include <stdio.h>
#include <string.h>

/* type of tree */
typedef struct TREE
{
    tree_t parent;                      /**< parent node */
    tree_t *child;                      /**< child tree */
    int csize;                          /**< size of child */
    void *data;                         /**< address of data */
    int dsize;                          /**< data size */
    void *attribute;                    /**< address of attribute */
    int asize;                          /**< attribute size */
} TREE;

tree_t tree_create(void)
{
    tree_t tree;

    /* Allocate memory for the TREE structure */
    tree = (tree_t)malloc(sizeof(TREE));
    if (!tree) return NULL;

    /* Initialize structural parameters */
    memset(tree, 0, sizeof(TREE));

    return tree;
}

void tree_delete(tree_t tree, void (*func)(tree_t tree))
{
    int i;

    /* Input value validity check */
    if (!tree) return;

    /* Recursively delete each child tree */
    for (i = 0; i < tree->csize; i++) tree_delete(tree->child[i], func);

    /* Additional execution function */
    if (func) func(tree);

    /* Free childs */
    if (tree->child) free(tree->child);

    /* Free data */
    if (tree->data) free(tree->data);

    /* Free attribute */
    if (tree->attribute) free(tree->attribute);

    /* Free tree structure */
    free(tree);
}

int tree_insert(tree_t tree, int index)
{
    tree_t *array;

    /* Input value validity check */
    if (!tree) return 0;
    if (index < 0 || index > tree->csize) return 0;

    /* Readjust the number of child */
    array = realloc(tree->child, (tree->csize + 1) * sizeof(tree_t));
    if (!array) return 0;

    /* Update child array */
    tree->child = array;

    /* Move the childs after the index back */
    if (index < tree->csize) memmove(&array[index + 1], &array[index], sizeof(tree_t) * (tree->csize - index));

    /* Reset child */
    array[index] = NULL;

    /* Update queue status */
    tree->csize++;

    return 1;
}

int tree_erase(tree_t tree, int index)
{
    tree_t *array, temp;

    /* Input value validity check */
    if (!tree) return 0;
    if (index < 0 || index >= tree->csize) return 0;
    if (tree->child[index]) return 0;

    /* Only one child, free it directly */
    if (tree->csize == 1)
    {
        free(tree->child);
        tree->child = NULL;
        tree->csize = 0;
        return 1;
    }

    /* Record the last child first, then reduce the space to prevent crossing the boundary */
    temp = tree->child[tree->csize - 1];
    array = realloc(tree->child, tree->csize - 1);
    if (!array) return 0;

    /* Update child array */
    tree->child = array;

    /* Move the childs after the index front */
    memmove(&array[index], &array[index + 1], sizeof(tree_t) * (tree->csize - index - 2));

    /* Make up for the last child as well */
    array[tree->csize - 2] = temp;

    /* Update queue status */
    tree->csize--;

    return 1;
}

int tree_attach(tree_t tree, int index, tree_t attach)
{
    /* Input value validity check */
    if (!tree) return 0;
    if (index < 0 || index >= tree->csize) return 0;
    if (!attach) return 0;
    if (attach->parent) return 0;

    /* Linking parent and child */
    tree->child[index] = attach;
    attach->parent = tree;

    return 1;
}

tree_t tree_detach(tree_t tree, int index)
{
    tree_t detach;

    /* Input value validity check */
    if (!tree) return NULL;
    if (index < 0 || index >= tree->csize) return NULL;
    if (!tree->child[index]) return NULL;

    /* Record the child that needs to be detached */
    detach = tree->child[index];

    /* Leave the position blank */
    tree->child[index] = NULL;

    /* The child parent of the datach needs to be left blank */
    detach->parent = NULL;

    return detach;
}

tree_t tree_parent(tree_t tree)
{
    /* Input value validity check */
    if (!tree) return NULL;

    /* Return parent */
    return tree->parent;
}

tree_t tree_child(tree_t tree, int index)
{
    /* Input value validity check */
    if (!tree) return NULL;
    if (index < 0 || index >= tree->csize) return NULL;

    /* Return child */
    return tree->child[index];
}

int tree_csize(tree_t tree)
{
    /* Input value validity check */
    if (!tree) return 0;

    /* Return the child size */
    return tree->csize;
}

const void* tree_data(tree_t tree)
{
    /* Input value validity check */
    if (!tree) return NULL;

    /* Return tree data */
    return tree->data;
}

int tree_dsize(tree_t tree)
{
    /* Input value validity check */
    if (!tree) return 0;

    /* Return tree data size */
    return tree->dsize;
}

const void* tree_attribute(tree_t tree)
{
    /* Input value validity check */
    if (!tree) return NULL;

    /* Return tree attribute */
    return tree->attribute;
}

int tree_asize(tree_t tree)
{
    /* Input value validity check */
    if (!tree) return 0;

    /* Return tree attribute size */
    return tree->asize;
}

int tree_size(tree_t tree)
{
    int i, size = 0;

    /* Input value validity check */
    if (!tree) return 0;

    /* Traverse each child and recursively obtain its number of child */
    for (i = 0; i < tree->csize; i++) size += tree_size(tree->child[i]);

    /* Update size */
    return size + tree->csize;
}

int tree_depth(tree_t tree)
{
    int i, depth = 0, temp = 0;

    /* Input value validity check */
    if (!tree) return 0;

    /* Traverse each child and recursively obtain its depth of child */
    for (i = 0; i < tree->csize; i++)
    {
        temp = tree_depth(tree->child[i]);

        /* Record maximum depth */
        if (temp > depth) depth = temp;
    }

    /* Update depth */
    return depth + 1;
}

tree_t tree_to_valist(tree_t tree, int index, ...)
{
    tree_t node = tree;
    int i = index;
    va_list args;

    /* Input value validity check */
    if (!tree) return NULL;

    va_start(args, index);

    /* Loop to obtain the children of each level */
    while (node && i >= 0)
    {
        if (i >= node->csize) return NULL;
        node = node->child[i];
        i = va_arg(args, int);
    }

    va_end(args);

    return node;
}

int tree_set_data(tree_t tree, void* data, int size)
{
    void* d = NULL;

    /* Input value validity check */
    if (!tree) return 0;
    if (size < 0) return 0;

    /* If the incoming data size is 0, set the air sensitive data directly */
    if (size == 0)
    {
        if (tree->data) free(tree->data);
        tree->data = NULL;
        tree->dsize = 0;
        return 1;
    }

    /* If the data size is inconsistent, update the data storage space */
    if (size != tree->dsize)
    {
        d = realloc(tree->data, size);
        if (!d) return 0;
    }
    tree->data = d;

    /* Data assignment */
    if (data) memcpy(tree->data, data, size);

    /* Update data size */
    tree->dsize = size;

    return 1;
}

int tree_get_data(tree_t tree, void* data, int size)
{
    /* Input value validity check */
    if (!tree) return 0;
    if (!data) return 0;
    if (size < tree->dsize) return 0;

    /* Data assignment */
    memcpy(data, tree->data, tree->dsize);

    return 1;
}

int tree_set_attribute(tree_t tree, void* attribute, int size)
{
    void* d = NULL;

    /* Input value validity check */
    if (!tree) return 0;
    if (size < 0) return 0;

    /* If the incoming attribute size is 0, set the air sensitive attribute directly */
    if (size == 0)
    {
        if (tree->attribute) free(tree->attribute);
        tree->attribute = NULL;
        tree->asize = 0;
        return 1;
    }

    /* If the attribute size is inconsistent, update the attribute storage space */
    if (size != tree->asize)
    {
        d = realloc(tree->attribute, size);
        if (!d) return 0;
    }
    tree->attribute = d;

    /* Attribute assignment */
    if (attribute) memcpy(tree->attribute, attribute, size);

    /* Update attribute size */
    tree->asize = size;

    return 1;
}

int tree_get_attribute(tree_t tree, void* attribute, int size)
{
    /* Input value validity check */
    if (!tree) return 0;
    if (!attribute) return 0;
    if (size < tree->asize) return 0;

    /* Attribute assignment */
    memcpy(attribute, tree->attribute, tree->asize);

    return 1;
}

static void expand_tree(tree_t tree, int depth, int limit, char* scope, void (*print)(tree_t tree))
{
    int i = 0;
    if (limit && depth >= limit) return; /* exceeded layer depth limit */
    if (depth > 0) /* skip indent for layer 0 */
    {
        for (i = 0; i < depth - 1; i++) { putchar(scope[i]?'|':' '); putchar(' '); putchar(' '); putchar(' '); }
        putchar(scope[i]?'|':'\'');
        putchar('-');
        if (!tree) { printf("O\r\n"); return; } /* empty child */
        putchar('-');
        putchar('-');
    }
    printf("> ");
    if (print) print(tree);
    printf("\r\n");
    for (i = 0; i < tree->csize; i++)
    {
        scope[depth] = (i < tree->csize - 1) ? 1 : 0;
        expand_tree(tree->child[i], depth + 1, limit, scope, print);
    }
}

static void tree_print_to_depth(tree_t tree, int depth, int limit, void (*print)(tree_t tree))
{
    char scope[depth];
    memset(scope, 0, depth);
    expand_tree(tree, 0, limit, scope, print);
}

void tree_print(tree_t tree, int depth, void (*print)(tree_t tree))
{
    /* Input value validity check */
    if (!tree) return;
    if (depth < 0) return;

    /* Expand the tree and print it out */
    tree_print_to_depth(tree, tree_depth(tree), depth, print);
}
