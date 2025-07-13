/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  ini.h
 *         \unit  ini
 *        \brief  This is a C language version of ini parser
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "ini.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* dump buffer define */
typedef struct
{
    char* address;              /**< buffer base address */
    unsigned int size;          /**< size of buffer */
    unsigned int end;           /**< end of buffer used */
} BUFFER;

/* iterator define */
typedef struct
{
    void *p;                    /**< iteration pointer */
    unsigned int i;             /**< iteration index */
} ITERATOR;

/* key-value define */
typedef struct PAIR
{
    struct PAIR *next;          /**< next pair */
    char* key;                  /**< key */
    char* value;                /**< value */
} PAIR;

/* section define */
typedef struct SECTION
{
    struct SECTION *next;       /**< next section */
    char* name;                 /**< section name */
    PAIR* pairs;                /**< pairs base */
    ITERATOR iterator;          /**< pair iterator */
    int count;                  /**< pair count */
} SECTION;

/* ini structure define */
typedef struct INI 
{
    SECTION* sections;          /**< sections base */
    ITERATOR iterator;          /**< section iterator */
    int count;                  /**< section count */
} INI;

static int etype = 0;           /**< error type */
static int eline = 0;           /**< error line */

#define E(type)                 etype=(type)
#define iscomment(c)            ((c) == '#' || (c) == ';')      /* ini supports `#` and `;` style annotations */

/**
 *  \brief Calculate the smallest power of 2 that is greater than or equal to a given number.
 * 
 *  \param[in] x The given number.
 *  \return The smallest power of 2 greater than or equal to x.
 */
static unsigned int pow2gt(unsigned int x)
{
    int b = sizeof(int) * 8;
    int i = 1;

    --x;
    while (i < b)
    {
        x |= (x >> i);
        i <<= 1;
    }

    return x + 1;
}

/**
 *  \brief Duplicate a given string.
 * 
 *  \param[in] str String to be duplicated.
 *  \param[in] len Length of the string.
 *  \return Pointer to the duplicated string if successful, NULL otherwise.
 */
static char* ini_strdup(const char* str, int len)
{
    char* s;

    /* Allocate memory for the new string */
    s = (char*)malloc(len + 1);
    if (!s) return NULL;

    /* Copy the given string into the allocated memory */
    memcpy(s, str, len);
    s[len] = '\0';

    return s;
}

/**
 *  \brief Compare the first n1 characters of two strings.
 *
 *  \param[in] s1 The first string to compare
 *  \param[in] n1 The number of characters to compare in s1
 *  \param[in] s2 The second string to compare
 *  \param[in] n2 The number of characters to compare in s2
 *
 *  \return Returns an integer less than, equal to, or greater than zero if the first n1 characters of s1 is found,
 *          respectively, to be less than, to match, or be greater than the first n2 characters of s2.
 */
static int ini_strnncmp(const char* s1, int n1, const char* s2, int n2)
{
    int i = 0;
    if (s1 == s2) return 0;
    if (!s1) return -1;
    if (!s2) return 1;
    if (n1 != n2) return n1 - n2;
    
    for (i = 0; i < n1; i++)
    {
        if (s1[i] != s2[i]) return (s1[i] - s2[i]);
    }

    return 0;
}

/**
 *  \brief Compare the first n1 characters of two strings case-insensitively.
 *
 *  \param[in] s1 The first string to compare
 *  \param[in] n1 The number of characters to compare in s1
 *  \param[in] s2 The second string to compare
 *  \param[in] n2 The number of characters to compare in s2
 *
 *  \return Returns an integer less than, equal to, or greater than zero if the first n1 characters of s1 is found,
 *          respectively, to be less than, to match, or be greater than the first n2 characters of s2.
 */
static int ini_strcsnncmp(const char* s1, int n1, const char* s2, int n2)
{
    int i = 0;
    if (s1 == s2) return 0;
    if (!s1) return -1;
    if (!s2) return 1;
    if (n1 != n2) return n1 - n2;
    
    for (i = 0; i < n1; i++)
    {
        if (tolower(s1[i]) != tolower(s2[i])) return (tolower(s1[i]) - tolower(s2[i]));
    }

    return 0;
}

/**
 *  \brief confirm whether buf still has the required capacity, otherwise add capacity.
 *  \param[in] buf: buf handle
 *  \param[in] needed: required capacity
 *  \return 1 success or 0 fail
 */
static int expansion(BUFFER* buf, unsigned int needed)
{
    char* address;
    unsigned int size;
    if (!buf || !buf->address) return 0;
    needed += buf->end;
    if (needed <= buf->size) return 1; /* there is still enough space in the current buf */
    size = pow2gt(needed);
    address = (char*)realloc(buf->address, size);
    if (!address) return 0;
    buf->size = size;
    buf->address = address;
    return 1;
}
#define buf_append(n)       expansion(buf, (n))                 /* append n size space for buf */
#define buf_putc(c)         (buf->address[buf->end++]=(c))      /* put a non zero character into buf */
#define buf_end()           (buf->address[buf->end])            /* obtain the tail of buf */

/**
 *  \brief Get the section at the specified index in an INI file.
 *
 *  \param[in] ini The INI file object
 *  \param[in] index The index of the section to retrieve
 *
 *  \return Returns a pointer to the section at the specified index, or NULL if the index is out of range
 */
static SECTION* ini_section(ini_t ini, int index)
{
    if (index >= ini->count) return NULL;
    if (index < ini->iterator.i || !ini->iterator.p || index == 0)
    {
        ini->iterator.i = 0;
        ini->iterator.p = ini->sections;
    }
    while (ini->iterator.p && ini->iterator.i < index)
    {
        ini->iterator.p = ((SECTION *)(ini->iterator.p))->next;
        ini->iterator.i++;
    }
    return ini->iterator.p;
}

/**
 *  \brief Get the pair at the specified index in a section of an INI file.
 *
 *  \param[in] sect The section object
 *  \param[in] index The index of the pair to retrieve
 *
 *  \return Returns a pointer to the pair at the specified index, or NULL if the index is out of range
 */
static PAIR* section_pair(SECTION *sect, int index)
{
    if (index >= sect->count) return NULL;
    if (index < sect->iterator.i || !sect->iterator.p || index == 0)
    {
        sect->iterator.i = 0;
        sect->iterator.p = sect->pairs;
    }
    while (sect->iterator.p && sect->iterator.i < index)
    {
        sect->iterator.p = ((SECTION *)(sect->iterator.p))->next;
        sect->iterator.i++;
    }
    return sect->iterator.p;
}

/**
 *  \brief create an ini object.
 *  \param[in] none
 *  \return ini object
 */
ini_t ini_create(void)
{
    ini_t ini = NULL;

    /* Allocate ini structure space */
    ini = (ini_t)malloc(sizeof(INI));
    if (!ini) return NULL;

    /* Initialize ini structure member variables */
    ini->sections = NULL;
    ini->count = 0;
    ini->iterator.p = NULL;
    ini->iterator.i = 0;

    return ini;
}

/**
 *  \brief Free the memory allocated for a section in an INI file.
 *
 *  \param[in] sect The section object to be freed
 */
static void section_free(SECTION *sect)
{
    PAIR *pair = NULL, *next = NULL;

    if (!sect) return;

    /* Traverse and free each pair */
    pair = sect->pairs;
    while (pair)
    {
        next = pair->next;
        if (pair->key) free(pair->key);
        if (pair->value) free(pair->value);
        free(pair);
        pair = next;
    }
    if (sect->name) free(sect->name);
    free(sect);
}

/**
 *  \brief delete an ini object.
 *  \param[in] ini object
 *  \return none
 */
void ini_delete(ini_t ini)
{
    SECTION *sect = NULL, *next = NULL;

    if (!ini) return;

    /* Traverse and free each section */
    sect = ini->sections;
    while (sect)
    {
        next = sect->next;

        section_free(sect);

        sect = next;
    }

    free(ini);
}

/**
 *  \brief Find the index of a section in an INI file with a specified name.
 *
 *  \param[in] ini The INI file object
 *  \param[in] section The name of the section to find
 *  \param[in] len The length of the section name
 *
 *  \return Returns the index of the section if found, or -1 if the section is not found
 */
static int find_section(ini_t ini, const char* section, int len)
{
    int i;
    SECTION* sect = NULL;

    if (!ini) return -1;

    /* Find if a section with the same name already exists */
    for (i = 0; i < ini->count; i++)
    {
        sect = ini_section(ini, i);
        if (ini_strnncmp(sect->name, strlen(sect->name), section, len) == 0)
        {
            return i;
        }
    }

    return -1;
}

/**
 *  \brief find the index where the section is located.
 *  \param[in] ini: ini handle
 *  \param[in] *section: section name
 *  \return index or negative number not found
 */
int ini_section_index(ini_t ini, const char* section)
{
    int i;
    SECTION* sect = NULL;

    if (!ini) return -1;
    if (!section) return -1;

    if (ini->iterator.p)
    {
        /* at the current iteration position */
        if (strcmp(((SECTION*)(ini->iterator.p))->name, section) == 0)
        {
            return ini->iterator.i;
        }
    }

    /* Find if a section with the same name already exists */
    for (i = 0; i < ini->count; i++)
    {
        sect = ini_section(ini, i);
        if (strcmp(sect->name, section) == 0)
        {
            return i;
        }
    }

    return -1;
}

/**
 *  \brief get the section name of the specified index.
 *  \param[in] ini: ini handle
 *  \param[in] index: index
 *  \return section name or NULL fail
 */
const char* ini_section_name(ini_t ini, int index)
{
    SECTION* sect = NULL;

    if (!ini) return NULL;
    if (index < 0 || index >= ini->count) return NULL;

    sect = ini_section(ini, index);
    if (!sect) return NULL;

    return sect->name;
}

/**
 * \brief Add a new section to an INI file with the specified name.
 *
 * \param[in] ini The INI file object
 * \param[in] section The name of the section to add
 * \param[in] len The length of the section name
 *
 * \return Returns a pointer to the newly added section, or NULL if the section cannot be added
 */
static SECTION* add_section(ini_t ini, const char* section, int len)
{
    SECTION* sect = NULL;

    /* Create a new section and add it to the INI file structure */
    sect = (SECTION*)malloc(sizeof(SECTION));
    if (!sect) return NULL;

    /* Allocate space for section name */
    sect->name = ini_strdup(section, len);
    if (!sect->name) 
    {
        free(sect);
        return NULL;
    }

    /* Initialize section structure variables */
    sect->pairs = NULL;
    sect->count = 0;
    sect->iterator.p = NULL;
    sect->iterator.i = 0;
    sect->next = NULL;

    /* Link the new section to ini */
    if (ini->count == 0) ini->sections = sect;
    else ini_section(ini, ini->count - 1)->next = sect;

    ini->count++;

    return sect;
}

/**
 * \brief Find the index of a pair in a section of an INI file with a specified key.
 *
 * \param[in] sect The section object
 * \param[in] key The key of the pair to find
 * \param[in] len The length of the key
 *
 * \return Returns the index of the pair if found, or -1 if the pair is not found
 */
static int find_pair(SECTION *sect, const char *key, int len)
{
    int i = 0;
    PAIR *pair = NULL;

    if (sect->iterator.p)
    {
        /* at the current iteration position */
        if (ini_strcsnncmp(((PAIR*)(sect->iterator.p))->key, strlen(((PAIR*)(sect->iterator.p))->key), key, len) == 0)
        {
            return sect->iterator.i;
        }
    }

    /* Traverse the pairs in the section and match the same key */
    for (i = 0; i < sect->count; i++)
    {
        pair = section_pair(sect, i);
        if (ini_strcsnncmp(pair->key, strlen(pair->key), key, len) == 0)
        {
            return i;
        }
    }

    return -1; /* No match found */
}

/**
 * \brief Add a new key-value pair to a section in an INI file.
 *
 * \param[in] sect The section object
 * \param[in] key The key to add
 * \param[in] key_len The length of the key
 * \param[in] value The value to add
 * \param[in] value_len The length of the value
 *
 * \return Returns a pointer to the newly added pair, or NULL if the pair cannot be added
 */
static PAIR* add_pair(SECTION* sect, const char* key, int key_len, const char* value, int value_len)
{
    PAIR* pair;

    if (!sect) return NULL;

    /* Allocate pair space and initialize */
    pair = (PAIR *)malloc(sizeof(PAIR));
    if (!pair) return NULL;
    pair->next = NULL;
    pair->key = NULL;
    pair->value = NULL;

    /* duplicate the key */
    if (key)
    {
        pair->key = ini_strdup(key, key_len);
        if (!pair->key) goto FAIL;
    }

    /* Duplicate the value */
    if (value)
    {
        pair->value = ini_strdup(value, value_len);
        if (!pair->value) goto FAIL;
    }

    /* Link the new pair to section */
    if (sect->count > 0) section_pair(sect, sect->count - 1)->next = pair;
    else sect->pairs = pair;

    sect->count++;

    return pair;

FAIL:
    /* Free the allocated space before exiting the function */
    if (pair)
    {
        if (pair->key) free(pair->key);
        if (pair->value) free(pair->value);
        free(pair);
    }
    return NULL;
}

/**
 *  \brief add section to ini.
 *  \param[in] ini: ini handle
 *  \param[in] *section: section name
 *  \return 1 success or 0 fail
 */
int ini_add_section(ini_t ini, const char* section)
{
    int i;

    if (!ini) return 0;
    if (!section) return 0;

    /* Find if a section with the same name already exists */
    if (ini_section_index(ini, section) >= 0) return 0;

    /* Calculate the length of section name */
    for (i = 0;;i++)
    {
        if (section[i] == '\0') break;
        // if (!(section[i] >= ' ' && section[i] <= '~')) return 0; /* Not a printable character */
    }

    return add_section(ini, section, i) == NULL ? 0 : 1;
}

/**
 *  \brief remove section from ini.
 *  \param[in] ini: ini handle
 *  \param[in] *section: section name
 *  \return 1 success or 0 fail
 */
int ini_remove_section(ini_t ini, const char* section)
{
    int i;
    SECTION* sect = NULL;
    SECTION* prev = NULL;

    if (!ini) return 0;
    if (!section) return 0;

    /* Find if a section with the same name already exists */
    for (i = 0; i < ini->count; i++)
    {
        sect = ini_section(ini, i);
        if (strcmp(sect->name, section) == 0) break;
        prev = sect;
    }
    if (i == ini->count || sect == NULL) return 0;

    /* Adjusting linked list */
    if (prev) prev->next = sect->next;
    else ini->sections = sect->next;

    section_free(sect);

    ini->count--;

    /* reset iterator */
    ini->iterator.p = NULL;

    return 1;
}

/**
 *  \brief set the specified value, it will be created if not exist.
 *  \param[in] ini: ini handle
 *  \param[in] *section: section name
 *  \param[in] *key: key
 *  \param[in] *value: value
 *  \return 1 success or 0 fail
 */
int ini_set_value(ini_t ini, const char* section, const char* key, const char* value)
{
    int i;
    char *tvalue = NULL;
    SECTION *sect = NULL;
    PAIR *pair = NULL;

    if (!ini) return 0;
    if (!section) return 0;
    if (!key) return 0;
    if (!value) return 0;

    i = ini_section_index(ini, section);
    if (i < 0) /* Add this section without it */
    {
        /* Calculate the length of section name */
        for (i = 0;;i++)
        {
            if (section[i] == '\0') break;
            // if (!(section[i] >= ' ' && section[i] <= '~')) return 0; /* Not a printable character */
        }
        if (add_section(ini, section, i) == NULL) return 0;
        sect = ini_section(ini, ini->count - 1);
    }
    else  
    {
        sect = ini_section(ini, i);
    }

    i = find_pair(sect, key, strlen(key));
    if (i < 0) /* Add this pair without it */
    {
        /* Calculate the length of section name */
        for (i = 0;;i++)
        {
            if (key[i] == '\0') break;
            // if (!(key[i] >= ' ' && key[i] <= '~')) return 0; /* Not a printable character */
        }
        if (add_pair(sect, key, i, value, strlen(value)) == NULL) return 0;
    }
    else
    {
        pair = section_pair(sect, i);

        /* Duplicate the value to be set first */
        tvalue = ini_strdup(value, strlen(value));
        if (!tvalue) return 0;

        /* The original value needs to be free */
        if (pair->value) free(pair->value);

        /* Update value */
        pair->value = tvalue;
    }

    return 1;
}

/**
 *  \brief get the specified value.
 *  \param[in] ini: ini handle
 *  \param[in] *section: section name
 *  \param[in] *key: key
 *  \return address of value or NULL fail
 */
const char* ini_get_value(ini_t ini, const char* section, const char* key)
{
    int i = 0;
    SECTION *sect = NULL;
    PAIR *pair = NULL;

    if (!ini) return NULL;
    if (!section) return NULL;
    if (!key) return NULL;

    /* Obtain the corresponding index based on the section name */
    i = ini_section_index(ini, section);
    if (i < 0) return NULL;

    /* Obtain the section handle based on the index */
    sect = ini_section(ini, i);
    if (!sect) return NULL;

    /* Obtain the corresponding index based on the key name */
    i = find_pair(sect, key, strlen(key));
    if (i < 0) return NULL;

    /* Obtain the pair handle based on the index */
    pair = section_pair(sect, i);
    if (!pair) return NULL;

    return pair->value;
}

/**
 *  \brief get the index of the specified key under the section.
 *  \param[in] ini: ini handle
 *  \param[in] *section: section name
 *  \param[in] *key: key
 *  \return index or negative number not found
 */
int ini_key_index(ini_t ini, const char* section, const char* key)
{
    int i = 0;
    SECTION *sect = NULL;
    PAIR *pair = NULL;

    if (!ini) return -1;
    if (!section) return -1;
    if (!key) return -1;

    /* Obtain the corresponding index based on the section name */
    i = ini_section_index(ini, section);
    if (i < 0) return -1;

    /* Obtain the section handle based on the index */
    sect = ini_section(ini, i);
    if (!sect) return -1;

    /* Matching key */
    i = find_pair(sect, key, strlen(key));
    if (i < 0) return -1;

    return i;
}

/**
 *  \brief get the key name of the specified index under the section.
 *  \param[in] ini: ini handle
 *  \param[in] *section: section name
 *  \param[in] index: index
 *  \return key name or NULL fail
 */
const char* ini_key_name(ini_t ini, const char* section, int index)
{
    int i = 0;
    SECTION *sect = NULL;
    PAIR *pair = NULL;

    if (!ini) return NULL;
    if (!section) return NULL;
    if (index < 0) return NULL;

    /* Obtain the corresponding index based on the section name */
    i = ini_section_index(ini, section);
    if (i < 0) return NULL;

    /* Obtain the section handle based on the index */
    sect = ini_section(ini, i);
    if (!sect) return NULL;

    /* Matching index */
    pair = section_pair(sect, index);
    if (!pair) return NULL;

    return pair->key;
}

/**
 *  \brief get the section count of ini.
 *  \param[in] ini: ini handle
 *  \return section count of ini
 */
int ini_section_count(ini_t ini)
{
    if (!ini) return 0;
    return ini->count;
}

/**
 *  \brief get the pair count of ini section.
 *  \param[in] ini: ini handle
 *  \param[in] *section: section name
 *  \return pair count of ini section
 */
int ini_pair_count(ini_t ini, const char* section)
{
    SECTION *sect = NULL;
    int index = 0;

    if (!ini) return 0;

    /* Obtain the corresponding index based on the section name */
    index = ini_section_index(ini, section);
    if (index < 0) return 0;

    /* Obtain the section handle based on the index */
    sect = ini_section(ini, index);
    if (!sect) return 0;

    return sect->count;
}

/**
 *  \brief remove key from ini section.
 *  \param[in] ini: ini handle
 *  \param[in] *section: section name
 *  \param[in] *key: key
 *  \return 1 success or 0 fail
 */
int ini_remove_key(ini_t ini, const char* section, const char* key)
{
    int i = 0;
    SECTION *sect = NULL;
    PAIR *pair = NULL, *prev = NULL;

    if (!ini) return 0;
    if (!section) return 0;
    if (!key) return 0;

    /* Obtain the corresponding index based on the section name */
    i = ini_section_index(ini, section);
    if (i < 0) return 0;

    /* Obtain the section handle based on the index */
    sect = ini_section(ini, i);
    if (!sect) return 0;

    /* Traverse section matching key */
    for (i = 0; i < sect->count; i++)
    {
        pair = section_pair(sect, i);
        if (ini_strcsnncmp(pair->key, strlen(pair->key), key, strlen(key)) == 0) break;
        prev = pair;
    }
    if (i == sect->count) return 0; /* Not matched */

    /* Adjusting linked list */
    if (prev) prev->next = pair->next;
    else sect->pairs = pair->next;

    /* Free pair space */
    if (pair->key) free(pair->key);
    if (pair->value) free(pair->value);
    free(pair);

    sect->count--;

    /* Reset iterator */
    sect->iterator.p = NULL;
    
    return 1;
}

/**
 * \brief Print the contents of an INI file to a buffer.
 *
 * \param[in] ini The INI file object
 * \param[in] buf The buffer to print the INI contents to
 *
 * \return Returns 1 if the printing is successful, 0 otherwise
 */
static int print_ini(ini_t ini, BUFFER* buf)
{
    int i, j, k;
    int olen = 0;
    SECTION* sect = NULL;
    PAIR* pair = NULL;

    /* Traverse each section */
    for (i = 0; i < ini->count; i++) 
    {
        /* Get section handle */
        sect = ini_section(ini, i);
        if (!sect) return 0;

        /* Print section name */
        olen = strlen(sect->name);
        if (!buf_append(olen + 3)) return 0;
        buf_putc('[');
        for (k = 0; k < olen; k++)
        {
            buf_putc(sect->name[k]);
        }
        buf_putc(']');
        buf_putc('\n');

        /* Traverse each pair */
        for (j = 0; j < sect->count; j++)
        {
            /* Get pair handle */
            pair = section_pair(sect, j);

            /* Print key */
            olen = strlen(pair->key);
            if (!buf_append(olen + 3)) return 0;
            for (k = 0; k < olen; k++)
            {
                buf_putc(tolower(pair->key[k]));
            }
            buf_putc(' ');
            buf_putc('=');
            buf_putc(' ');

            /* Get length of value */
            olen = 0;
            k = 0;
            while (1)
            {
                if (!pair->value[olen]) break;
                if (pair->value[olen] == '\n') k++; /* Print `\t` before line breaks */
                olen++;
            }
            if (!buf_append(olen + k + 1)) return 0;

            /* Print value */
            for (k = 0; k < olen; k++)
            {
                buf_putc(pair->value[k]);
                if (pair->value[k] == '\n') buf_putc('\t');
            }
            buf_putc('\n');
        }

        /* Add a blank line to distinguish each section */
        if (!buf_append(1)) return 0;
        buf_putc('\n');
    }

    /* End of text */
    if (!buf_append(1)) return 0;
    buf_end() = '\0';

    return 1;
}

/**
 *  \brief dump the ini object into a string.
 *  \param[in] ini: ini handle
 *  \param[in] preset: preset string length
 *  \param[out] *len: the length of the string actually dumped
 *  \return dumped string, please release this string space after use
 */
char* ini_dumps(ini_t ini, int preset, int *len)
{
    BUFFER p;

    if (!ini) return NULL;

    /* initialize buffer */
    if (preset < 1) preset = 1;
    p.address = (char*)malloc(preset);
    if (!p.address) return NULL;
    p.size = preset;
    p.end = 0;

    /* Print the ini data to the BUFFER structure */
    if (!print_ini(ini, &p))
    {
        free(p.address);
        return NULL;
    }

    /* Store the length of the generated ini string if the len parameter is provided */
    if (len) *len = p.end;

    return p.address;
}

/**
 *  \brief dump the ini object into a file.
 *  \param[in] ini: ini handle
 *  \param[in] *filename: file name
 *  \return length of the dump or negative fail
 */
int ini_file_dump(ini_t ini, char* filename)
{
    FILE* f;
    char* out;
    int len;

    if (!ini) return -1;

    out = (char *)ini_dumps(ini, 0, &len);
    if (!out) return -1;

    f = fopen(filename, "w");
    if (!f)
    {
        free(out);
        return -1;
    }

    fwrite(out, 1, len - 1, f);
    fclose(f);

    free(out);

    return len;
}

/**
 * \brief Skip leading spaces, tabs, and carriage returns in a string.
 *
 * \param[in] text The input string to be skipped
 *
 * \return Returns a pointer to the first non-space, non-tab, non-carriage return character in the string
 */
static const char* skip(const char* text)
{
    while (*text && (*text == ' ' || *text == '\r' || *text == '\t'))
    {
        text++;  // Skip spaces, tabs, and carriage returns
    }
    return text;
}

/**
 * \brief Skip trailing spaces, tabs, and carriage returns in a string, starting from the end and moving backwards.
 *
 * \param[in] text The input string to be skipped
 * \param[in] base The base of the string to limit the backward movement
 *
 * \return Returns a pointer to the last non-space, non-tab, non-carriage return character in the string
 */
static const char* rskip(const char* text, const char* base)
{
    while ((text >= base) && (*text == ' ' || *text == '\r' || *text == '\t'))
    {
        text--;
    }
    return text;
}

/**
 * \brief Find the end of the current line in a string.
 *
 * \param[in] text The input string
 *
 * \return Returns a pointer to the end of the current line in the string
 */
static const char* lend(const char* text)
{
    while (*text && *text != '\n')
    {
        text++;  // Move to the next character until the end of the line or the end of the string is reached
    }
    return text;  // Return the pointer to the end of the line
}

/**
 * \brief Find the scope of a value in a string, considering the given depth.
 *
 * \param[in] text The input string
 * \param[in] depth The depth of the scope to consider
 *
 * \return Returns a pointer to the end of the scope of the value in the string
 */
static const char* value_scope(const char* text, int depth)
{
    const char *s = text;
    const char *sentinel = NULL;

    s = lend(s);
    while (*s)
    {   
        /* Sentinel locate non empty characters on a new line */
        sentinel = skip(s + 1);

        /* skip comments */
        if (iscomment(*sentinel))
        {
            s = lend(sentinel);
            continue;
        }

        /* The depth of the current row is greater than the depth of the section, indicating that it still belongs to that section */
        if (sentinel - s - 1 > depth)
        {
            s = sentinel;
            s = lend(s);
        }
        /* Exceeding the depth, determine whether to stop detection based on the current sentinel character */
        else  
        {
            /* If it is not at the end of the text or line, it indicates that this is a valid character. 
             * If it exceeds the scope of the section, it will not belong to the scope of this value and can exit the detection.
             */
            if (*sentinel != 0 && *sentinel != '\n') break;
            /* As an empty line, it is still within the scope of the value */
            else s = sentinel;
        }
    }

    /* Starting from the position of the sentry, probe back and remove empty lines (as there are no actual characters) */
    while (*s == 0 || *s == '\n')
    {
        /* Starting from the line break position and detecting backwards */
        sentinel = rskip(s - 1, text);
        if (*sentinel != 0 && *sentinel != '\n') break;

        /* Fallback by an empty line */
        s = sentinel;
    }

    return s;
}

/**
 *  \brief load ini from string.
 *  \param[in] *text: string text
 *  \return ini handle or NULL fail
 */
ini_t ini_loads(const char* text)
{
    ini_t ini;
    const char *s = NULL, *tail = NULL;
    const char *scope = NULL;
    char *value = NULL;
    int len = 0;
    int depth = 0; /* current line depth */
    SECTION *sect = NULL;
    PAIR *pair = NULL;

    if (!text) return NULL;
    ini = ini_create();
    if (!ini) return NULL;

    eline = 1;
    etype = INI_E_OK;

    while (*text)
    {
        /* skip useless characters */
        s = skip(text);
        depth = s - text;
        text = s;

        /* end of text, exit parsing */
        if (*text == 0) break;

        /* skip comments */
        if (iscomment(*text))
        {
            text = lend(text);
            if (*text == '\n') 
            {
                text++;
                eline++;
                continue;
            }
        }
        
        /* Exceeded the range of the previous value and does not belong to the previous value. Starting a new parsing */
        if (text >= scope)
        {
            /* Blank line, skip */
            if (*text == '\n') 
            {
                text++;
                eline++;
                continue;
            }

            /* Parsing section */
            if (*text == '[')
            {
                /* Locate the end of the line to determine the range of section names */
                s = lend(text);
                tail = rskip(s - 1, text);

                /* The square brackets `[]` do not appear in pairs, and the section name does not hold */
                if (*tail != ']')
                {
                    E(INI_E_BRACKETS);
                    goto FAIL;
                }

                /* Only one pair of empty square brackets `[]` without a section name */
                if (tail - text - 1 <= 0)
                {
                    E(INI_E_SECTION);
                    goto FAIL;
                }

                /* Check for duplicates and determine if the section already exists */
                if (find_section(ini, text + 1, tail - text - 1) >= 0)
                {
                    E(INI_E_RESECTION);
                    goto FAIL;
                }

                /* Add section to ini */
                sect = add_section(ini, text + 1, tail - text - 1);
                if (!sect)
                {
                    E(INI_E_MEMORY);
                    goto FAIL;
                }

                /* Update parsing location */
                text = s;
                pair = NULL;
            }
            /* Parsing key-value pair */
            else  
            {   
                s = text;
                tail = NULL;

                /* Parse the key and determine its scope */
                while (*s)
                {
                    /* The key value pair is incomplete as it breaks a line before parsing the delimiter */
                    if (*s == '\n')
                    {
                        E(INI_E_DELIM);
                        goto FAIL;
                    }

                    /* Parsed key value delimiter, the scope of the key has been determined */
                    if (*s == '=' || *s == ':')
                    {
                        tail = s - 1;
                        tail = rskip(tail, text);
                        break;
                    }

                    s++;
                }

                /* Confirmed the scope of the key and determined whether the key is empty */
                if (!tail || tail < text)
                {
                    E(INI_E_KEY);
                    goto FAIL;
                }

                /* When there is no record of the current section, even if the key is parsed, it is invalid */
                if (!sect)
                {
                    E(INI_E_SECTION);
                    goto FAIL;
                }

                /* Check for duplicates and determine if the key already exists */
                if (find_pair(sect, text, tail - text + 1) >= 0)
                {
                    E(INI_E_REKEY);
                    goto FAIL;
                }

                /* Add pair to section */
                pair = add_pair(sect, text, tail - text + 1, "", 0);
                if (!pair)
                {
                    E(INI_E_MEMORY);
                    goto FAIL;
                }

                /* Update parsing location */
                len = 0;
                text = s + 1; /* skip delimiter '=' and ':' */

                /* After determining the key, we need to explore the scope of the value.
                 *
                 * The indentation depth of the incoming section is used to determine 
                 * whether the depth of the value is within the depth range of the section. 
                 */
                scope = value_scope(text, depth); 
            }
        }
        /* This line belongs to the previous line, still within the range of the previous value */
        else 
        {
            /* There is no record of pair, which means there is no key, and the key value pair is not valid */
            if (!pair)
            {
                E(INI_E_MEMORY);
                goto FAIL;
            }

            /* Value can contain multiple lines.
             * If the first line value after the key is empty, record the `\n` first 
             */
            if (*text == '\n')
            {
                value = realloc(pair->value, len + 1);
                if (!value)
                {
                    E(INI_E_MEMORY);
                    goto FAIL;
                }
                pair->value = value;
                pair->value[len++] = '\n';
                pair->value[len] = 0;
            }

            text = skip(text);

            /* Empty line, skip first */
            if (*text == '\n') 
            {
                text++;
                eline++;
                continue;
            }
 
            /* Go to the end of the current line and temporarily do not record blank line endings. 
             * 
             * If the next line is still within the scope of value, space will be reallocated next time, 
             * and these blank characters will be added.
             */
            tail = lend(text);
            s = rskip(tail - 1, text);

            /* Reassign space and append the content of the current line */
            value = realloc(pair->value, len + (s - text + 1));
            if (!value)
            {
                E(INI_E_MEMORY);
                goto FAIL;
            }
            pair->value = value;

            /* Assign and update length */
            memcpy(&pair->value[len], text, (s - text + 1));
            len += (s - text + 1);
            pair->value[len] = 0;

            /* Update parsing location */
            text = tail;
        }
    }

    return ini;

FAIL:
    ini_delete(ini);
    return NULL;
}

/**
 *  \brief load ini from file.
 *  \param[in] *filename: file name
 *  \return ini handle or NULL fail
 */
ini_t ini_file_load(const char* filename)
{
    FILE* f = NULL;
    ini_t ini = NULL;
    long len;
    char* text = NULL;

    if (!filename) return NULL;

    eline = 0;

    /* open file and get the length of file */
    f = fopen(filename, "rb");
    if (!f)
    {
        E(INI_E_OPEN);
        goto FAIL;
    }
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    /* read file */
    text = (char*)malloc(len + 1);
    if (text)
    {
        fread(text, 1, len, f);
        fclose(f);
        f = NULL;
    }
    else
    {
        E(INI_E_MEMORY);
        goto FAIL;
    }
    text[len] = 0;

    /* load text */
    ini = ini_loads(text);
    if (!ini)
    {
        goto FAIL;
    }
    free(text);

    return ini;

FAIL:
    // printf("ini parse error! line %d, error %d.\r\n", eline, etype);
    if (f) fclose(f);
    if (text) free(text);
    return NULL;
}

/**
 *  \brief obtain parsing error information.
 *  \param[out] *line: line number where the error occurred
 *  \param[out] *type: error type, ref INI_E_XXX
 *  \return 1 has an error or 0 does not exist
 */
int ini_error_info(int* line, int* type)
{
    if (etype == INI_E_OK) return 0;
    if (line) *line = eline;
    if (type) *type = etype;
    return 1;
}

