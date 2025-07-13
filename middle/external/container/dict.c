/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  dict.c
 *         \unit  dict
 *        \brief  This is a general-purpose C language dict module, with common data structure, realized by hash table.
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "dict.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

/**< Minimum capacity of hash table */
#define MIN_CAPACITY            4

/**< Address method of dict hash table */
#define TBALE                   ((groove_t *)(dict->base))

/* Hash table groove definition */
typedef struct 
{
    /**< Hash value */
    unsigned int hash;
    
    /**< Key address */
    char *key;
    
    /**< Value address */
    void *value;                
} GROOVE, *groove_t;

/* dict type define */
typedef struct DICT
{
    /**< Base address for groove data, the data of the hash table exists in this address space */
    void *base;                 
    
    /**< Size of value, for example, sizeof(int), sizeof(char), ... */
    unsigned int vsize;
    
    /**< Size of dict, that is the count of dict members */
    unsigned int size;
    
    /**< Capacity of dict, actual hash table capacity */
    unsigned int capacity;
    
    /**< Iterator index, the static index required to record the iteration position during iteration traversal */
    unsigned int it;
    
    /**< Size of key, when it is a positive number, it is a fixed-length key, and 0 is a variable-length key. */
    unsigned int ksize;
    
    /**< Function to get indefinite key length */
    int (*klength)(void *key);
    
#ifdef DICT_USE_ERROR
    /**< Error space used due to misoperations in data reading and writing */
    void *error;
#endif 
} DICT;

static unsigned int hash_bkdr(void *data, unsigned int size)
{
    int i = 0;
    unsigned char *str = (unsigned char *)data;
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
    for (i = 0; i < size; i++)
    {
        hash = hash * seed + (*str++);
    }
    return (hash & INT_MAX);
}

static int default_klength(void *key)
{
    return strlen(key) + 1;
}

/* Default memory space comparison function */
static int kcompare(const void *s1, unsigned int n1, const void *s2, unsigned int n2)
{
    const unsigned char *p1 = s1, *p2 = s2;
    unsigned int i = 0;
    
    for (i = 0; i < n1 && i < n2; i++)
    {
        if (p1[i] != p2[i])
        {
            return (p1[i] < p2[i]) ? -1 : 1;
        }
    }
    
    if (n1 == n2) return 0;
    
    return (n1 < n2) ? -1 : 1;
}

dict_t dict_create(unsigned int vsize)
{
    dict_t dict;
    
    /* Allocate space */
    dict = (dict_t)malloc(sizeof(DICT));
    if (!dict) return NULL;
    
#ifdef DICT_USE_ERROR
    dict->error = malloc(vsize);
    if (!dict->error) { free(dict); return NULL; }
#endif 
    
    /* Initialize structure members */
    dict->base = NULL;
    dict->ksize = 0;
    dict->klength = default_klength;
    dict->vsize = vsize;
    dict->size = 0;
    dict->capacity = 0;
    dict->it = 0;
    
    return dict;
}

void dict_delete(dict_t dict)
{
    if (!dict) return;
    dict_clear(dict);
#ifdef DICT_USE_ERROR
    if (dict->error) free(dict->error);
#endif 
    free(dict);
}

/* But when the hash table stores a certain coefficient, the hash table needs to be readjusted to obtain more space. */
static int dict_resize(dict_t dict, unsigned int capacity)
{
    unsigned int i = 0, index = 0, hash = 0;
    int len = 0;
    groove_t *base;

    /* Newly allocate and initialize a space */
    base = malloc(capacity * sizeof(groove_t));
    if (!base) return 0;
    memset(base, 0, capacity * sizeof(groove_t));

    /* Copy old data to new hash table */
    for (i = 0; i < dict->capacity; i++)
    {
        /* Data is stored in the groove */
        if (TBALE[i])
        {
            /* The hash value of the current groove is marked as -1, 
             * indicating that this groove is still occupied, 
             * but no data is actually stored. 
             * The data originally stored will not be recorded in the new hash table, but will be released. */
            if (TBALE[i]->hash == (unsigned int)-1)
            {
                if (TBALE[i]->key) free(TBALE[i]->key);
                if (TBALE[i]->value) free(TBALE[i]->value);
                free(TBALE[i]);
            }
            /* Add the new valid hash table data to the new hash table one by one. */
            else  
            {
                len = (dict->ksize > 0) ? dict->ksize : dict->klength(TBALE[i]->key);
                hash = hash_bkdr((void *)TBALE[i]->key, len) % capacity;
                index = hash;
                while (base[index]) index = (index + 1) % capacity;
                base[index] = TBALE[i];
                base[index]->hash = hash;
            }
        }
    }

    /* Update hash table */
    if (dict->base) free(dict->base);
    dict->base = base;
    dict->capacity = capacity;

    return 1;
}

int dict_set_klength(dict_t dict, unsigned int ksize, int (*klength)(void *key))
{
    if (!dict) return 0;
    
    if (ksize == 0)
    {
        if (!klength) return 0; /* Functions that do not obtain indefinite length keys are not allowed */
        dict->ksize = 0;
        dict->klength = klength;
    }
    else 
    {
        dict->ksize = ksize;
        dict->klength = NULL;
    }

    return 1;
}

void* dict_insert(dict_t dict, void *key, void *value)
{
    groove_t groove = NULL;
    unsigned int hash = 0, index;
    int len = 0;

    if (!dict) return NULL;
    if (!key) return NULL;

    /* The current capacity affects the search rate and needs to be expanded */
    if (dict->size >= ((dict->capacity >> 2) + (dict->capacity >> 1))) /* size exceeds 3/4 of capacity */
    {
        /* Allocate new hash table space */
        if (!dict_resize(dict, dict->capacity < MIN_CAPACITY ? MIN_CAPACITY : dict->capacity << 1)) return NULL;
    }
    
    /* find a free groove */
    len = (dict->ksize > 0) ? dict->ksize : dict->klength(key);
    hash = hash_bkdr(key, len) % dict->capacity;
    index = hash;
    while (TBALE[index] && TBALE[index]->hash != -1)
    {
        index = (index + 1) % dict->capacity;
        if (index == hash) return NULL;
    }

    /* space allocation */
    groove = TBALE[index];
    if (!groove) groove = (groove_t)malloc(sizeof(GROOVE));
    if (!groove) return NULL;
    groove->key = malloc(len);
    if (!groove->key) { if (!TBALE[index]) free(groove); return NULL; }
    groove->value = malloc(dict->vsize);
    if (!groove->value) { free(groove->key), groove->key = NULL; if (!TBALE[index]) free(groove); return NULL; }

    /* assign */
    groove->hash = hash;
    memcpy(groove->key, key, len);
    if (value) memcpy(groove->value, value, dict->vsize);

    /* insert */
    TBALE[index] = groove;
    dict->size++;

    return groove->value;
}

static unsigned int find_index(dict_t dict, void *key)
{
    unsigned int hash = 0, index;
    int len = 0;

    if (dict->capacity == 0) return -1;

    len = (dict->ksize > 0) ? dict->ksize : dict->klength(key);
    hash = hash_bkdr(key, len) % dict->capacity;
    index = hash;
    
    /* Traverse the entire hash table and find the groove where both hash value and key match */
    while (TBALE[index])
    {
        if (TBALE[index]->hash == hash && !kcompare(key, len, TBALE[index]->key, (dict->ksize > 0) ? dict->ksize : dict->klength(TBALE[index]->key))) break;
        index = (index + 1) % dict->capacity; /* linear search */
    }
    if (!TBALE[index]) return -1;
    
    return index;
}

int dict_erase(dict_t dict, void *key)
{
    groove_t groove;
    unsigned int index, next;

    if (!dict) return 0;
    if (!key) return 0;

    index = find_index(dict, key);
    if (index == -1) return 0;
    groove = TBALE[index];

    /* Release the data stored in the groove and mark it as occupied */
    if (groove->key) { free(groove->key); groove->key = NULL; }
    if (groove->value) { free(groove->value); groove->value = NULL; }
    groove->hash = -1;
    dict->size--;

    /* Size less than 1/4 of capacity */
    if (dict->capacity > MIN_CAPACITY && dict->size <= (dict->capacity >> 2)) 
    {
        dict_resize(dict, dict->capacity >> 1);
    }

    return 1;
}

void dict_clear(dict_t dict)
{
    int i = 0;
    if (!dict) return;
    for (i = 0; i < dict->capacity; i++)
    {
        if (TBALE[i])
        {
            if (TBALE[i]->key) free(TBALE[i]->key);
            if (TBALE[i]->value) free(TBALE[i]->value);
            free(TBALE[i]);
            TBALE[i] = NULL;
        }
    }
    if (dict->base) free(dict->base);
    dict->base = NULL;
    dict->size = 0;
    dict->capacity = 0;
}

int dict_size(dict_t dict)
{
    if (!dict) return 0;
    return dict->size;
}

int dict_vsize(dict_t dict)
{
    if (!dict) return 0;
    return dict->vsize;
}

int dict_find(dict_t dict, void *key)
{
    if (!dict) return 0;
    if (!key) return 0;
    return (find_index(dict, key) == -1) ? 0 : 1;
}

void* dict_value(dict_t dict, void *key)
{
    unsigned int index;
    groove_t groove;

    if (!dict) return NULL;
    if (!key) return dict_error(dict);
 
    index = find_index(dict, key);
    if (index == -1) return dict_error(dict);

    return TBALE[index]->value;
}

#ifdef DICT_USE_ERROR
void* dict_error(dict_t dict)
{
    if (!dict) return NULL;
    return dict->error;
}
#endif 

void dict_it_init(dict_t dict)
{
    if (!dict) return;
    dict->it = 0;
}

void* dict_it_get(dict_t dict, char **key)
{
    groove_t groove = NULL;
    if (!dict) return NULL;
    if (dict->it >= dict->capacity) return dict_error(dict);
    while (dict->it < dict->capacity)
    {
        if (TBALE[dict->it] && TBALE[dict->it]->hash != -1)
        {
            groove = TBALE[dict->it];
            dict->it++;
            break;
        }
        dict->it++;
    }
    if (!groove) return dict_error(dict);
    if (key) *key = groove->key;
    return groove->value;
}
