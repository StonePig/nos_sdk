/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  valloc.c
 *         \unit  valloc
 *        \brief  Test how much space is allocated
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* memory alloc info type define */
typedef struct 
{
    char *file;
    int line;
    int size;
} minfo;

/* rbtree node type define */
typedef struct NODE
{
    struct NODE *parent;
    struct NODE *left;
    struct NODE *right;
    int color;
    void* pointer;
    minfo info;
} NODE;

/* rbtree type define */
typedef struct
{
    NODE* root;
    NODE* nil;
    int size;
} RBTREE, *rbtree_t;

/* rbtree node color */
#define BLACK           (0)
#define RED             (1)

static rbtree_t mrbtree = NULL;

static rbtree_t rbtree_create(void)
{
    rbtree_t rbtree;
    rbtree = (rbtree_t)malloc(sizeof(RBTREE));
    if (!rbtree) return NULL;
    rbtree->nil = (NODE*)malloc(sizeof(NODE));
    if (!rbtree->nil)
    {
        free(rbtree);
        return NULL;
    }
    rbtree->nil->color = BLACK;
    rbtree->root = rbtree->nil;
    rbtree->size = 0;
    return rbtree;
}

static void recursion_delete_node(rbtree_t rbtree, NODE* node)
{
    if (node == rbtree->nil) return;
    recursion_delete_node(rbtree, node->left);
    recursion_delete_node(rbtree, node->right);
    free(node);
}

static void rbtree_delete(rbtree_t rbtree)
{
    if (!rbtree) return;
    recursion_delete_node(rbtree, rbtree->root);;
    free(rbtree->nil);
    free(rbtree);
}

static NODE* rbtree_find_node(rbtree_t rbtree, void* pointer)
{
    NODE* node = rbtree->root;
    while (node != rbtree->nil)
    {
        if (pointer < node->pointer) node = node->left;
        else if (pointer > node->pointer) node = node->right;
        else return node;
    }
    return rbtree->nil;
}

static void left_rotate(rbtree_t rbtree, NODE* x) 
{
    NODE* y = x->right;

    x->right = y->left;
    if (y->left != rbtree->nil) y->left->parent = x;

    y->parent = x->parent;
    if (x->parent == rbtree->nil) rbtree->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;

    y->left = x;
    x->parent = y;
}

static void right_rotate(rbtree_t rbtree, NODE* y) 
{
    NODE* x = y->left;

    y->left = x->right;
    if (x->right != rbtree->nil) x->right->parent = y;

    x->parent = y->parent;
    if (y->parent == rbtree->nil) rbtree->root = x;
    else if (y == y->parent->right) y->parent->right = x;
    else y->parent->left = x;

    x->right = y;
    y->parent = x;
}

static void rbtree_insert_fixup(rbtree_t rbtree, NODE* z) 
{
    NODE* y = NULL;

    while (z->parent->color == RED)
    {
        if (z->parent == z->parent->parent->left)
        {
            y = z->parent->parent->right;
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    left_rotate(rbtree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(rbtree, z->parent->parent);
            }
        }
        else
        {
            y = z->parent->parent->left;
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    right_rotate(rbtree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(rbtree, z->parent->parent);
            }
        }
    }
    rbtree->root->color = BLACK;
}

static void rbtree_insert_node(rbtree_t rbtree, NODE* z)
{
    NODE* y = rbtree->nil;
    NODE* x = rbtree->root;

    while (x != rbtree->nil)
    {
        y = x;
        if (z->pointer < x->pointer) x = x->left;
        else if (z->pointer > x->pointer) x = x->right;
        else return;
    }

    z->parent = y;
    if (y == rbtree->nil) rbtree->root = z;
    else if (z->pointer < y->pointer) y->left = z;
    else y->right = z;

    z->left = rbtree->nil;
    z->right = rbtree->nil;
    z->color = RED;

    rbtree_insert_fixup(rbtree, z);
}

static minfo* rbtree_insert(rbtree_t rbtree, void* pointer, minfo info)
{
    NODE* node;
    int child = 0;
    if (!rbtree) return NULL;
    node = (NODE*)malloc(sizeof(NODE));
    if (!node) return NULL;
    node->pointer = pointer;
    node->info = info;
    rbtree_insert_node(rbtree, node);
    rbtree->size++;
    return &(node->info);
}

static void rbtree_erase_fixup(rbtree_t rbtree, NODE* x) 
{
    NODE* w = NULL;

    while ((x != rbtree->root) && (x->color == BLACK))
    {
        if (x == x->parent->left)
        {
            w = x->parent->right;

            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(rbtree, x->parent);
                w = x->parent->right;
            }

            if ((w->left->color == BLACK) && (w->right->color == BLACK))
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(rbtree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(rbtree, x->parent);
                x = rbtree->root;
            }

        }
        else
        {
            w = x->parent->left;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(rbtree, x->parent);
                w = x->parent->left;
            }

            if ((w->left->color == BLACK) && (w->right->color == BLACK))
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(rbtree, w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(rbtree, x->parent);
                x = rbtree->root;
            }
        }
    }
    x->color = BLACK;
}

static NODE* node_min(rbtree_t rbtree, NODE* x)
{
    if (x == rbtree->nil) return x;
    while (x->left != rbtree->nil) x = x->left;
    return x;
}

static NODE* node_max(rbtree_t rbtree, NODE* x)
{
    if (x == rbtree->nil) return x;
    while (x->right != rbtree->nil) x = x->right;
    return x;
}

static NODE* rbtree_successor(rbtree_t rbtree, NODE* x) 
{
    NODE* y = x->parent;
    if (x->right != rbtree->nil) return node_min(rbtree, x->right);
    while ((y != rbtree->nil) && (x == y->right))
    {
        x = y;
        y = y->parent;
    }
    return y;
}

static NODE* rbtree_erase_node(rbtree_t rbtree, NODE* z) 
{
    NODE* y = rbtree->nil;
    NODE* x = rbtree->nil;

    if ((z->left == rbtree->nil) || (z->right == rbtree->nil)) y = z;
    else y = rbtree_successor(rbtree, z);

    if (y->left != rbtree->nil) x = y->left;
    else if (y->right != rbtree->nil) x = y->right;

    x->parent = y->parent;
    if (y->parent == rbtree->nil) rbtree->root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;

    if (y != z)
    {
        z->pointer = y->pointer;
        z->info = y->info;
    }

    if (y->color == BLACK) rbtree_erase_fixup(rbtree, x);

    return y;
}

static int rbtree_erase(rbtree_t rbtree, void* pointer)
{
    NODE* node = NULL;
    NODE* cur = NULL;
    if (!rbtree) return 0;
    node = rbtree_find_node(rbtree, pointer);
    if (node == rbtree->nil) return 0;
    cur = rbtree_erase_node(rbtree, node);
    free(cur);
    rbtree->size--;
    return 1;
}

static int rbtree_size(rbtree_t rbtree)
{
    if (!rbtree) return 0;
    return rbtree->size;
}

static int rbtree_find(rbtree_t rbtree, void* pointer)
{
    if (!rbtree) return 0;
    return rbtree_find_node(rbtree, pointer)==rbtree->nil?0:1;
}

static minfo* rbtree_data(rbtree_t rbtree, void* pointer)
{
    if (!rbtree) return NULL;
    return &(rbtree_find_node(rbtree, pointer)->info);
}


static NODE* node_next(rbtree_t rbtree, NODE* node)
{
    if (node->right != rbtree->nil)
    {
        node = node->right;
        node = node_min(rbtree, node);
    }
    else  
    {
        if (node == node->parent->left) node = node->parent;
        else node = node->parent->parent;
    }
    return node;
}

static NODE* node_prev(rbtree_t rbtree, NODE* node)
{
    if (node->left != rbtree->nil)
    {
        node = node->left;
        node = node_max(rbtree, node);
    }
    else  
    {
        if (node == node->parent->right) node = node->parent;
        else node = node->parent->parent;
    }
    return node;
}

void* vm_malloc(size_t size, char *file, int line)
{
    void* p;
    minfo info;
    if (!mrbtree)
    {
        mrbtree = rbtree_create();
        if (!mrbtree) return NULL;
    }
    p = malloc(size);
    if (!p) return NULL;
    info.size = size;
    info.file = file;
    info.line = line;
    if (!rbtree_insert(mrbtree, p, info))
    {
        free(p);
        return NULL;
    }
    return p;
}

void* vm_calloc(size_t num, size_t size, char *file, int line)
{
    void* p;
    p = vm_malloc(num * size, file, line);
    if (!p) return NULL;
    memset(p, 0, num * size);
    return p;
}

void vm_free(void* block)
{
    if (!block) return;
    if (rbtree_erase(mrbtree, block)) free(block);
    if (rbtree_size(mrbtree) == 0) 
    {
        rbtree_delete(mrbtree);
        mrbtree = NULL;
    }
}

void* vm_realloc(void* block, size_t size, char *file, int line)
{
    void* p;
    if (!block) 
    {
        return vm_malloc(size, file, line);
    }
    if (size == 0)
    {
        vm_free(block);
        return NULL;
    }
    if (!rbtree_find(mrbtree, block)) return NULL;
    p = realloc(block, size);
    if (!p) return NULL;
    if (p == block)
    {
        minfo *info;
        info = rbtree_data(mrbtree, block);
        info->file = file;
        info->line = line;
        info->size = size;
    }
    else  
    {
        minfo info;
        rbtree_erase(mrbtree, block);
        info.file = file;
        info.line = line;
        info.size = size;
        rbtree_insert(mrbtree, p, info);
    }
    return p;
}

void v_check_unfree(void)
{
    int size, i;
    void *p;
    NODE* node;
    if (!mrbtree) return;
    size = rbtree_size(mrbtree);
    node = node_min(mrbtree, mrbtree->root);
    for (i = 0; i < size; i++)
    {
        printf("address: %p, size: %d, file: %s, line: %d\r\n", node->pointer, node->info.size, node->info.file, node->info.line);
        node = node_next(mrbtree, node);
    }
}

int v_check_count(void)
{
    if (!mrbtree) return 0;
    return rbtree_size(mrbtree);
}

int v_check_used(void)
{
    int size, i;
    int used = 0;
    NODE* node;
    if (!mrbtree) return 0;
    size = rbtree_size(mrbtree);
    node = node_min(mrbtree, mrbtree->root);
    for (i = 0; i < size; i++)
    {
        used += node->info.size;
        node = node_next(mrbtree, node);
    }
    return used;
}