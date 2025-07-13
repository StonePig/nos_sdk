/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  json.c
 *         \unit  json
 *        \brief  This is a C language version of json streamlined parser
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "json.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <float.h>

/* dump buffer define */
typedef struct
{
    char* address;                          /**< buffer base address */
    unsigned int size;                      /**< size of buffer */
    unsigned int end;                       /**< end of buffer used */
} BUFFER;

/* json define */
typedef struct JSON {
    struct JSON* next;                      /**< next json */
    char* key;                              /**< the key of json is empty when the type is array */
    int type;                               /**< json base type, @ref JSON_TYPE_xxx */
    union
    {
        int bool_;                          /**< bool type */
        double float_;                      /**< float number type */
        int int_;                           /**< int number type */
        char* string_;                      /**< string type */
        struct JSON* child_;                /**< array or object type */
    } value;
} JSON;

static const char* lbegin = 0;              /**< beginning of line */
static int eline = 0;                       /**< line of error message */
static int ecolumn = 0;                     /**< column of error message */
static int etype = 0;                       /**< type of error message */

/* predeclare these prototypes. */
static const char* parse_text(json_t json, const char* text);
static int print_json(json_t json, BUFFER* buf, int depth, int format);

/* set error message and type */
#define E(type)                             (etype=(type),ecolumn=text-lbegin)

/**
 *  \brief for analysing failed parses
 *  \param[out] line: error line
 *  \param[out] column: error column
 *  \return error type
 */
int json_error_info(int* line, int* column)
{
    /* No error occurred, return directly */
    if (etype == JSON_E_OK) return JSON_E_OK;

    /* Output the line and column where the error is located */
    if (line) *line = eline;
    if (column) *column = ecolumn;

    /* Return error type */
    return etype;
}

/**
 *  \brief Compare two strings case-insensitively.
 *
 *  \param[in] s1 The first string to compare
 *  \param[in] s2 The second string to compare
 *
 *  \return Returns an integer less than, equal to, or greater than zero if s1 is found, respectively,
 *          to be less than, to match, or be greater than s2.
 */
static int json_strccmp(const char* s1, const char* s2)
{
    if (!s1) return (s1 == s2) ? 0 : 1;
    if (!s2) return 1;

    /* Compare the similarities and differences of characters one by one */
    for (; tolower(*s1) == tolower(*s2); ++s1, ++s2)
    {
        if (*s1 == 0) return 0;
    }

    return tolower(*(const unsigned char*)s1) - tolower(*(const unsigned char*)s2);
}

/**
 *  \brief Duplicate a given string.
 * 
 *  \param[in] str String to be duplicated.
 *  \param[in] len Length of the string.
 *  \return Pointer to the duplicated string if successful, NULL otherwise.
 */
static char* json_strdup(const char* str, int len)
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
 *  \brief get the smallest power of 2 not greater than x.
 *  \param[in] x: positive integer
 *  \return the smallest power of 2 not greater than x
 */
static int pow2gt(int x)
{
    int b = sizeof(int) * 8;
    int i = 1;
    --x;
    while (i < b) { x |= (x >> i); i <<= 1; }
    return x + 1;
}

/**
 *  \brief confirm whether buf still has the required capacity, otherwise add capacity.
 *  \param[in] buf: buf handle
 *  \param[in] needed: required capacity
 *  \return 1 success or 0 fail
 */
static int expansion(BUFFER *buf, unsigned int needed)
{
    char* address;
    int size;
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

static unsigned int parse_hex4(const char* str)
{
    unsigned int h = 0;
    if (*str >= '0' && *str <= '9') h += (*str) - '0';
    else if (*str >= 'A' && *str <= 'F') h += 10 + (*str) - 'A';
    else if (*str >= 'a' && *str <= 'f') h += 10 + (*str) - 'a';
    else return 0;
    h = h << 4; str++;
    if (*str >= '0' && *str <= '9') h += (*str) - '0';
    else if (*str >= 'A' && *str <= 'F') h += 10 + (*str) - 'A';
    else if (*str >= 'a' && *str <= 'f') h += 10 + (*str) - 'a';
    else return 0;
    h = h << 4; str++;
    if (*str >= '0' && *str <= '9') h += (*str) - '0';
    else if (*str >= 'A' && *str <= 'F') h += 10 + (*str) - 'A';
    else if (*str >= 'a' && *str <= 'f') h += 10 + (*str) - 'a';
    else return 0;
    h = h << 4; str++;
    if (*str >= '0' && *str <= '9') h += (*str) - '0';
    else if (*str >= 'A' && *str <= 'F') h += 10 + (*str) - 'A';
    else if (*str >= 'a' && *str <= 'f') h += 10 + (*str) - 'a';
    else return 0;
    return h;
}

/**
 *  \brief Convert and write a UTF-8 character sequence to an output buffer.
 *
 *  \param[in] in The input string containing the UTF-8 sequence
 *  \param[in,out] out The output buffer to write the converted UTF-8 sequence
 */
static void json_utf(const char **in, char **out)
{
    const char* ptr = *in;
    char* ptr2 = *out;
    int len = 0;
    unsigned int uc, uc2;
    unsigned char mask_first_byte[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
    uc = parse_hex4(ptr + 1); ptr += 4; /* get the unicode char. */
    if ((uc >= 0xDC00 && uc <= 0xDFFF) || uc == 0) return; /* check for invalid.    */
    if (uc >= 0xD800 && uc <= 0xDBFF) /* UTF16 surrogate pairs. */
    {
        if (ptr[1] != '\\' || ptr[2] != 'u') return; /* missing second-half of surrogate */
        uc2 = parse_hex4(ptr + 3);
        ptr += 6;
        if (uc2 < 0xDC00 || uc2>0xDFFF)    return; /* invalid second-half of surrogate */
        uc = 0x10000 + (((uc & 0x3FF) << 10) | (uc2 & 0x3FF));
    }
    len = 4;
    if (uc < 0x80) len = 1;
    else if (uc < 0x800) len = 2;
    else if (uc < 0x10000) len = 3;
    ptr2 += len;
    switch (len)
    {
    case 4: *--ptr2 = ((uc | 0x80) & 0xBF); uc >>= 6;
    case 3: *--ptr2 = ((uc | 0x80) & 0xBF); uc >>= 6;
    case 2: *--ptr2 = ((uc | 0x80) & 0xBF); uc >>= 6;
    case 1: *--ptr2 = (uc | mask_first_byte[len]);
    }
    ptr2 += len;
}

/**
 *  \brief Skip leading whitespace characters in a string, including newline characters.
 *
 *  \param[in] in The input string to be skipped
 *
 *  \return Returns a pointer to the first non-whitespace character in the string
 */
static const char* skip(const char* in)
{
    while (in && *in && (unsigned char)*in <= ' ')
    {
        /* when a newline character is encountered, record the current parsing line */
        if (*in == '\n') 
        { 
            eline++; 
            lbegin = in; /* Record line start position */
        } 

        in++;
    }

    return in;
}

/**
 *  \brief create a null json object.
 *  \return json handle or NULL fail
 */
json_t json_create(void)
{
    json_t json;

    /* Allocate json structure space and initialize */
    json = (json_t)malloc(sizeof(JSON));
    if (json) memset(json, 0, sizeof(JSON));

    return json;
}

/**
 *  \brief delete the json entity and its sub-entities.
 *  \param[in] json: json handle
 *  \return none
 */
void json_delete(json_t json)
{
    json_t next;
    while (json)
    {
        next = json->next;

        /* For arrays or object types, recursively delete child json */
        if (json->type == JSON_TYPE_ARRAY || json->type == JSON_TYPE_OBJECT) json_delete(json->value.child_);
        /* String type, then free the string */
        else if (json->type == JSON_TYPE_STRING) free(json->value.string_);

        /* Free the key of json */
        if (json->key) free(json->key);

        /* Delete self */
        free(json); 

        json = next;
    }
}

/**
 *  \brief get the number of children of a json array or object.
 *  \param[in] json: json handle
 *  \return json size
 */
int json_size(json_t json)
{
    json_t c;
    int i = 0;

    if (!json) return 0;

    /* Only array and object have child objects */
    if (json->type != JSON_TYPE_ARRAY && json->type != JSON_TYPE_OBJECT) return 0; 

    /* Traverse for statistics */
    c = json->value.child_;
    while (c) 
    { 
        i++; 
        c = c->next; 
    }

    return i;
}

/**
 *  \brief get the type of json.
 *  \param[in] json: json handle
 *  \return json type
 */
int json_type(json_t json)
{
    if (!json) return JSON_TYPE_UNKNOW;
    return json->type;
}

/**
 *  \brief get the key of json.
 *  \param[in] json: json handle
 *  \return json key
 */
const char* json_key(json_t json)
{
    if (!json) return NULL;
    return json->key;
}

/**
 *  \brief get the bool of json.
 *  \param[in] json: json handle
 *  \return json bool
 */
int json_value_bool(json_t json)
{
    if (!json) return 0;
    if (json->type != JSON_TYPE_BOOL) return 0;
    return json->value.bool_;
}

/**
 *  \brief get the int of json.
 *  \param[in] json: json handle
 *  \return json int
 */
int json_value_int(json_t json)
{
    if (!json) return 0;
    if (json->type != JSON_TYPE_INT) return 0;
    return json->value.int_;
}

/**
 *  \brief get the float of json.
 *  \param[in] json: json handle
 *  \return json float
 */
double json_value_float(json_t json)
{
    if (!json) return 0.0;
    if (json->type != JSON_TYPE_FLOAT) return 0.0;
    return json->value.float_;
}

/**
 *  \brief get the string of json.
 *  \param[in] json: json handle
 *  \return json string
 */
const char* json_value_string(json_t json)
{
    if (!json) return NULL;
    if (json->type != JSON_TYPE_STRING) return NULL;
    return json->value.string_;
}

/**
 *  \brief get the array of json.
 *  \param[in] json: json handle
 *  \return json array
 */
json_t json_value_array(json_t json)
{
    if (!json) return NULL;
    if (json->type != JSON_TYPE_ARRAY) return NULL;
    return json->value.child_;
}

/**
 *  \brief get the object of json.
 *  \param[in] json: json handle
 *  \return json object
 */
json_t json_value_object(json_t json)
{
    if (!json) return NULL;
    if (json->type != JSON_TYPE_OBJECT) return NULL;
    return json->value.child_;
}

/**
 *  \brief set the key of json.
 *  \param[in] json: json handle
 *  \param[in] key: new key
 *  \return json itself success or NULL fail
 */
json_t json_set_key(json_t json, const char* key)
{
    char* k;

    if (!json) return NULL;

    /* The current key and the one to be set can be the same, and can be set successfully directly */
    if (json->key && (json->key == key || !strcmp(json->key, key))) return json;

    /* If the passed in key is not empty, duplicate a backup */
    if (key)
    {
        k = json_strdup(key, strlen(key));
        if (!k) return NULL;
    }
    /* Otherwise, clear the json key */
    else k = NULL;

    /* Release the old key to update the new one */
    if (json->key) free(json->key);
    json->key = k;

    return json;
}

/**
 *  \brief set the value of json to null.
 *  \param[in] json: json handle
 *  \return json itself success or NULL fail
 */
json_t json_set_null(json_t json)
{
    if (!json) return NULL;

    /* delete string value */
    if (json->type == JSON_TYPE_STRING) free(json->value.string_); 
    /* delete child objects */
    else if (json->type == JSON_TYPE_ARRAY || json->type == JSON_TYPE_OBJECT) json_delete(json->value.child_); 
    
    /* Change the type to null and reset the value */
    json->type = JSON_TYPE_NULL;
    memset(&json->value, 0, sizeof(json->value));

    return json;
}

/**
 *  \brief set the value of json to bool.
 *  \param[in] json: json handle
 *  \param[in] b: new bool
 *  \return json itself success or NULL fail
 */
json_t json_set_bool(json_t json, int b)
{
    if (!json) return NULL;

    /* If the current type does not match, set the type to null first */
    if (json->type != JSON_TYPE_BOOL) json_set_null(json);

    /* Change the type to bool and set the bool value */
    json->type = JSON_TYPE_BOOL;
    json->value.bool_ = (b == JSON_FALSE ? JSON_FALSE : JSON_TRUE);

    return json;
}

/**
 *  \brief set the value of json to int.
 *  \param[in] json: json handle
 *  \param[in] num: new number
 *  \return json itself success or NULL fail
 */
json_t json_set_int(json_t json, int num)
{
    if (!json) return NULL;

    /* If the current type does not match, set the type to null first */
    if (json->type != JSON_TYPE_INT || json->type != JSON_TYPE_FLOAT) json_set_null(json);

    /* Change the type to int and set the int value */
    json->type = JSON_TYPE_INT;
    json->value.int_ = num;

    return json;
}

/**
 *  \brief set the value of json to float.
 *  \param[in] json: json handle
 *  \param[in] num: new number
 *  \return json itself success or NULL fail
 */
json_t json_set_float(json_t json, double num)
{
    if (!json) return NULL;

    /* If the current type does not match, set the type to null first */
    if (json->type != JSON_TYPE_INT || json->type != JSON_TYPE_FLOAT) json_set_null(json);

    /* Change the type to float and set the float value */
    json->type = JSON_TYPE_FLOAT;
    json->value.float_ = num;

    return json;
}

/**
 *  \brief set the value of json to string.
 *  \param[in] json: json handle
 *  \param[in] string: new string
 *  \return json itself success or NULL fail
 */
json_t json_set_string(json_t json, const char* string)
{
    char* s;

    if (!json) return NULL;

    /* If the current type does not match, set the type to null first */
    if (json->type != JSON_TYPE_STRING) json_set_null(json);

    /* Change the type to string */
    json->type = JSON_TYPE_STRING;

    /* The current string and the one to be set can be the same, and can be set successfully directly */
    if (json->value.string_ && (json->value.string_ == string || !strcmp(json->value.string_, string))) return json;

    /* If the passed in string is not empty, duplicate a backup */
    if (string) 
    {
        s = json_strdup(string, strlen(string));
        if (!s) return NULL;
    }
    /* Otherwise, clear the json string */
    else s = NULL;

    /* Release the old string to update the new one */
    if (json->value.string_) free(json->value.string_);
    json->value.string_ = s;

    return json;
}

/**
 *  \brief set the value of json to array.
 *  \param[in] json: json handle
 *  \param[in] object: object handle
 *  \return json itself success or NULL fail
 */
json_t json_set_object(json_t json, json_t object)
{
    if (!json) return NULL;

    /* If the current type does not match, set the type to null first */
    if (json->type != JSON_TYPE_OBJECT) json_set_null(json);

    /* Change the type to object */
    json->type = JSON_TYPE_OBJECT;

    /* The current object and the one to be set can be the same, and can be set successfully directly */
    if (json->value.child_ && json->value.child_ == object) return json;

    /* Release the old object to update the new one */
    if (json->value.child_) json_delete(json->value.child_);
    json->value.child_ = object;

    return json;
}

/**
 *  \brief set the value of json to array.
 *  \param[in] json: json handle
 *  \param[in] array: array handle
 *  \return json itself success or NULL fail
 */
json_t json_set_array(json_t json, json_t array)
{
    if (!json) return NULL;

    /* If the current type does not match, set the type to null first */
    if (json->type != JSON_TYPE_ARRAY) json_set_null(json);

    /* Change the type to array */
    json->type = JSON_TYPE_ARRAY;

    /* The current array and the one to be set can be the same, and can be set successfully directly */
    if (json->value.child_ && json->value.child_ == array) return json;

    /* Release the old array to update the new one */
    if (json->value.child_) json_delete(json->value.child_);
    json->value.child_ = array;

    return json;
}

/**
 *  \brief set the int values of json to array.
 *  \param[in] json: json handle
 *  \param[in] numbers: array
 *  \param[in] count: size of array
 *  \return json itself success or NULL fail
 */
json_t json_set_array_int(json_t json, const int* numbers, int count)
{
    json_t array = NULL, n = NULL, p = NULL;
    int i;

    /* Input parameter validity check */
    if (!json) return NULL;
    if (!numbers) return NULL;
    if (count <= 0) return NULL;

    /* Add data one by one to the array */
    for (i = 0; i < count; i++)
    {
        /* Create a single json data item */
        n = json_create();
        if (!n) 
        {
            json_delete(array); 
            return NULL;
        }

        /* Set array items as input data */
        json_set_int(n, numbers[i]);

        /* When i is 0, which is the first item of the array, the json array points to it */
        if (!i) array = n;
        /* When it is not the first item, link it directly */
        else p->next = n;

        p = n;
    }

    /* Set the current json to generate an array */
    json_set_array(json, array);

    return json;
}

/**
 *  \brief set the float values of json to array.
 *  \param[in] json: json handle
 *  \param[in] numbers: array
 *  \param[in] count: size of array
 *  \return json itself success or NULL fail
 */
json_t json_set_array_float(json_t json, const float* numbers, int count)
{
    json_t array = NULL, n = NULL, p = NULL;
    int i;

    /* Input parameter validity check */
    if (!json) return NULL;
    if (!numbers) return NULL;
    if (count <= 0) return NULL;

    /* Add data one by one to the array */
    for (i = 0; i < count; i++)
    {
        /* Create a single json data item */
        n = json_create();
        if (!n) 
        {
            json_delete(array); 
            return NULL;
        }

        /* Set array items as input data */
        json_set_float(n, numbers[i]);

        /* When i is 0, which is the first item of the array, the json array points to it */
        if (!i) array = n;
        /* When it is not the first item, link it directly */
        else p->next = n;

        p = n;
    }

    /* Set the current json to generate an array */
    json_set_array(json, array);

    return json;
}

/**
 *  \brief set the double values of json to array.
 *  \param[in] json: json handle
 *  \param[in] numbers: array
 *  \param[in] count: size of array
 *  \return json itself success or NULL fail
 */
json_t json_set_array_double(json_t json, const double* numbers, int count)
{
    json_t array = NULL, n = NULL, p = NULL;
    int i;

    /* Input parameter validity check */
    if (!json) return NULL;
    if (!numbers) return NULL;
    if (count <= 0) return NULL;

    /* Add data one by one to the array */
    for (i = 0; i < count; i++)
    {
        /* Create a single json data item */
        n = json_create();
        if (!n) 
        {
            json_delete(array); 
            return NULL;
        }

        /* Set array items as input data */
        json_set_float(n, numbers[i]);

        /* When i is 0, which is the first item of the array, the json array points to it */
        if (!i) array = n;
        /* When it is not the first item, link it directly */
        else p->next = n;

        p = n;
    }

    /* Set the current json to generate an array */
    json_set_array(json, array);

    return json;
}

/**
 *  \brief set the string values of json to array.
 *  \param[in] json: json handle
 *  \param[in] strings: array
 *  \param[in] count: size of array
 *  \return json itself success or NULL fail
 */
json_t json_set_array_string(json_t json, const char** strings, int count)
{
    json_t array = NULL, n = NULL, p = NULL;
    int i;

    /* Input parameter validity check */
    if (!json) return NULL;
    if (!strings) return NULL;
    if (count <= 0) return NULL;

    /* Add data one by one to the array */
    for (i = 0; i < count; i++)
    {
        /* Create a single json data item */
        n = json_create();
        if (!n) 
        {
            json_delete(array); 
            return NULL;
        }

        /* Set array items as input data.
         * When designing new space allocation using `json_set_string()`, there may be allocation failures.
         * Space allocation failed, we need to delete all the previously created json objects.
         */
        if (!json_set_string(n, strings[i]))
        {
            json_delete(n);
            json_delete(array); 
            return NULL;
        }

        /* When i is 0, which is the first item of the array, the json array points to it */
        if (!i) array = n;
        /* When it is not the first item, link it directly */
        else p->next = n;

        p = n;
    }

    /* Set the current json to generate an array */
    json_set_array(json, array);

    return json;
}

/**
 *  \brief Get the child element of a json object or array based on the specified key or index.
 *
 *  \param[in] json The json object or array
 *  \param[in] key The key to search for (ignored for json arrays)
 *  \param[in] index The index of the child element to retrieve
 *  \param[out] out_prev Optional pointer to store the previous child element
 *
 *  \return Returns the child element at the specified key or index, or NULL if not found or invalid input
 */
static json_t json_get_child(json_t json, const char* key, int index, json_t *out_prev)
{
    json_t c, prev = NULL;

    if (!json) return NULL;

    /* json of value type cannot have a key */
    if (key && json->type == JSON_TYPE_ARRAY) return NULL;

    /* Only array and object have child objects */
    if (json->type != JSON_TYPE_ARRAY && json->type != JSON_TYPE_OBJECT) return NULL;

    /* Traversing and matching json that match */
    c = json->value.child_; 
    while (c)
    {
        /* When no key is specified, only update the next match based on the index.
         * When specifying the key, it is necessary to update the next match only when the key is the same.
         */
        if (!key || !json_strccmp(c->key, key))
        {
            index--;
            if (index < 0) break; /* Out of valid indexes, exit matching */
        }

        /* Update traversal link */
        prev = c;
        c = c->next;
    }
    /* The index is still used up, which means it doesn't match the specified one */
    if (index >= 0) return NULL;

    /* The matching json object is invalid, exit directly */
    if (!c) return NULL;

    /* Output, matching the previous object of json */
    if (out_prev) *out_prev = prev;

    return c;
}

/**
 *  \brief Get the child element of a json object or array based on the specified key or index.
 *          When no key is specified, only update the next match based on the index.
 *          When specifying the key, it is necessary to update the next match only when the key is the same.
 *
 *  \param[in] json The json object or array
 *  \param[in] key The key to search for (ignored for json arrays)
 *  \param[in] index The index of the child element to retrieve
 *
 *  \return Returns the child element at the specified key or index, or NULL if not found or invalid input
 */
json_t json_get(json_t json, const char* key, int index)
{
    return json_get_child(json, key, index, NULL);
} 

/**
 *  \brief get the child of json object by index continuously.
 *  \param[in] json: json handle
 *  \param[in] index: index
 *  \param[in] ...: unexposed parameters, continuously enter index until the INT_MIN ends
 *  \return the specify child json object, or NULL fail
 */
json_t json_to_index_valist(json_t json, int index, ...)
{
    json_t c = json;
    va_list args;
    int i = index;

    if (!json) return NULL;

    /* First time get */
    c = json_get(c, NULL, i);
    if (!c) return NULL;

    va_start(args, index);

    /* Continuously obtain the index of indefinite parameter inputs to recursively obtain json sub objects */
    i = va_arg(args, int);
    while (c && i != INT_MIN) /* Stop getting when the index is `INT_MIN` */
    {
        c = json_get(c, NULL, i);
        i = va_arg(args, int);
    }

    va_end(args);

    return c;
}

/**
 *  \brief get the child of json object by key continuously.
 *  \param[in] json: json handle
 *  \param[in] key: address of key
 *  \param[in] ...: unexposed parameters, continuously enter key until the NULL ends
 *  \return the specify child json object, or NULL fail
 */
json_t json_to_key_valist(json_t json, char* key, ...)
{
    json_t c = json;
    va_list args;
    char* s = key;

    if (!json) return NULL;

    /* First time get */
    c = json_get(c, s, 0);
    if (!c) return c;

    va_start(args, key);

    /* Continuously obtain the index of indefinite parameter inputs to recursively obtain json sub objects */
    s = va_arg(args, char*);
    while (c && s != NULL) /* Stop getting when the key is `NULL` */
    {
        c = json_get(c, s, 0);
        s = va_arg(args, char*);
    }

    va_end(args);

    return c;
}

/**
 *  \brief insert a json object inito json by index.
 *  \param[in] json: json handle
 *  \param[in] index: index
 *  \param[in] item: another json handle
 *  \return ins itself success or NULL fail
 */
json_t json_attach(json_t json, int index, json_t ins)
{
    json_t c;
    json_t prev = NULL;

    /* Input parameter validity check */
    if (!json) return NULL;
    if (!ins) return NULL;

    /* Check if the `ins` has a key and determine if it matches the type of `json` */
    if (!(json->type == JSON_TYPE_ARRAY && !ins->key) && !(json->type == JSON_TYPE_OBJECT && ins->key)) return NULL;

    /* Traverse and iterate to the specified index */
    c = json->value.child_;
    while (c && index > 0)
    {
        prev = c;
        c = c->next;
        index--;
    }

    /* When the json child object is still empty, ins is taken as the first item */
    if (!c && !prev) 
    { 
        json->value.child_ = ins; 
        return ins; 
    }

    /* Link ins to its child */
    if (prev) prev->next = ins;
    ins->next = c;

    /* Head insertion */
    if (c == json->value.child_) json->value.child_ = ins;

    return ins;
}

/**
 *  \brief detach json object by key.
 *  \param[in] json: json handle
 *  \param[in] key: address of key
 *  \return detached object success or NULL fail
 */
json_t json_detach(json_t json, const char* key, int index)
{
    json_t c, prev = NULL;

    /* Getting for json object that require detach */
    c = json_get_child(json, key, index, &prev);
    if (!c) return NULL;

    /* Detach `c` from the child linked list */
    if (prev) prev->next = c->next;
    if (c == json->value.child_) json->value.child_ = c->next;

    /* Rest `c` link */
    c->next = NULL;

    return c;
}

/**
 *  \brief copy json with options.
 *  \param[in] json: json handle
 *  \param[in] recurse: whether to copy the sub-object, 0-no, other-yes
 *  \return new json object
 */
json_t json_copy(json_t json)
{
    json_t copy, cptr, nptr = NULL, child;

    if (!json) return NULL;

    /* Create a backup empty json object */
    copy = json_create();
    if (!copy) return NULL;

    copy->type = json->type;
    copy->value = json->value;

    /* If it is a string type, copy the string */
    if (json->type == JSON_TYPE_STRING && json->value.string_)
    {
        copy->value.string_ = json_strdup(json->value.string_, strlen(json->value.string_));
        if (!copy->value.string_) 
        { 
            json_delete(copy); 
            return NULL; 
        }
    }

    /* If there is a key, copy the key */
    if (json->key)
    {
        copy->key = json_strdup(json->key, strlen(json->key));
        if (!copy->key) 
        { 
            json_delete(copy); 
            return NULL; 
        }
    }

    /* walk the ->next chain for the child. */
    if (json->type == JSON_TYPE_OBJECT || json->type == JSON_TYPE_ARRAY) 
    {
        cptr = json->value.child_;
        while (cptr)
        {
            /* copy (with recurse) each json in the ->next chain */
            child = json_copy(cptr);
            if (!child) 
            { 
                json_delete(copy); 
                return NULL;
            }

            /* if newitem->child already set, then crosswire ->prev and ->next and move on */
            if (nptr) 
            {
                nptr->next = child;
                nptr = child;
            }
            /* set newitem->child and move to it */
            else 
            {
                copy->value.child_ = child;
                nptr = child;
            }

            cptr = cptr->next;
        }
    }

    return copy;
}

/**
 *  \brief Parse and convert a number in a json string.
 *
 *  \param[in,out] json The json object to store the parsed number
 *  \param[in] text The input string containing the number
 *
 *  \return Returns a pointer to the next character after the parsed number in the string, or NULL if there was an error in parsing the number
 */
static const char* parse_number(json_t json, const char* text)
{
    double number = 0;                  /* Converted number */
    int sign = 1, scale = 0;            /* Sign and scale of integer part */
    int e_sign = 1, e_scale = 0;        /* Sign and scale of exponent part */
    int isint = 1;                      /* Flag for whether there is only an integer part within a string */

    /* First, check if there is a negative number with `-` before it */
    if (*text == '-')
    {
        sign = -1;
        text++;

        /* Check if the first character is a valid number */
        if (!(*text >= '0' && *text <= '9')) 
        { 
            E(JSON_E_VALUE);
            return NULL; 
        }
    }

    /* Skip invalid `0` in the previous section */
    while (*text == '0') text++;
    
    /* Integer part */
    if (*text >= '1' && *text <= '9')
    {
        do
        {
            number = (number * 10.0) + (*text++ - '0'); /* carry addition */
        } while (*text >= '0' && *text <= '9');
    }

    /* Decimal part */
    if (*text == '.')
    {
        text++;

        /* Check if the first character is a valid number */
        if (!(*text >= '0' && *text <= '9')) 
        { 
            E(JSON_E_VALUE); 
            return NULL; 
        }

        /* The number of decimal parts is also increased by 10 times first, and then reduced according to the scale later on */
        do
        {
            number = (number * 10.0) + (*text++ - '0');
            scale--;
        } while (*text >= '0' && *text <= '9');

        /* Decimal part present, marked as non integer */
        isint = 0;
    }

    /* Exponent part */
    if (*text == 'e' || *text == 'E')
    {
        text++;

        /* Symbol `+` skip */
        if (*text == '+') text++;
        /* Symbol `-` with sign */
        else if (*text == '-')
        {
            e_sign = -1;
            text++;
        }

        /* Check if the first character is a valid number */
        if (!(*text >= '0' && *text <= '9')) 
        { 
            E(JSON_E_VALUE); 
            return NULL; 
        }

        /* Conversion exponent part */
        while (*text >= '0' && *text <= '9') 
        { 
            e_scale = (e_scale * 10) + (*text++ - '0'); /* number */
        }

        /* Exponent part present, marked as non integer */
        isint = 0;
    }

    /* Calculated conversion result */
    number = (double)sign * number * pow(10.0, (scale + e_scale * e_sign));

    /* As an integer and within the scope of an integer */
    if (isint && INT_MIN <= number && number <= INT_MAX) 
    {
        json->type = JSON_TYPE_INT;
        json->value.int_ = (int)number;
    }
    /* As a floating-point number */
    else
    {
        json->type = JSON_TYPE_FLOAT;
        json->value.float_ = number;
    }

    return text;
}

/**
 *  \brief parse the input text to buffer, and fill the results into buf.
 *  \param[in] text: number text
 *  \param[out] buf: the address used to receive the parsed string pointer
 *  \return the new address of the transformed text
 */
static const char* parse_string_buffer(char** buf, const char* text)
{
    const char* ptr = text + 1;
    char* ptr2;
    char* out;
    int len = 0;
    
    *buf = NULL;

    /* Not a string! */
    if (*text != '\"') 
    { 
        E(JSON_E_INVALID); 
        return NULL; 
    } 

    /* Get the length of the string */
    while (*ptr && *ptr != '\"')
    {
        if (*ptr++ == '\\') ptr++; /* skip escaped quotes. */
        len++;
    }

    /* Allocate storage space based on the calculated string length */
    out = (char*)malloc(len + 1);
    if (!out) 
    {
        E(JSON_E_MEMORY);
        return NULL;
    }

    /* Copy text to new space */
    ptr = text + 1;
    ptr2 = out;
    while (*ptr && *ptr != '\"')
    {
        /* Normal character */
        if (*ptr != '\\') { *ptr2++ = *ptr++; } 
        /* Escape character */
        else 
        {
            ptr++;
            if (*ptr == 'b') { *ptr2++ = '\b'; }
            else if (*ptr == 'f') { *ptr2++ = '\f'; }
            else if (*ptr == 'n') { *ptr2++ = '\n'; }
            else if (*ptr == 'r') { *ptr2++ = '\r'; }
            else if (*ptr == 't') { *ptr2++ = '\t'; }
            else if (*ptr == 'u') { json_utf(&ptr, &ptr2); }
            else { *ptr2++ = *ptr; }
            ptr++;
        }
    }

    *ptr2 = 0;
    if (*ptr == '\"') ptr++;

    *buf = out;

    return ptr;
}

/**
 *  \brief parse the input text and fill the result into json.
 *  \param[in,out] json: json handle
 *  \param[in] text: string text
 *  \return the new address of the transformed text
 */
static const char* parse_string(json_t json, const char* text)
{
    json->value.string_ = NULL;
    json->type = JSON_TYPE_STRING;
    return parse_string_buffer(&(json->value.string_), text);
}

/**
 *  \brief Parse an array in a json string and create a corresponding json object.
 *
 *  \param[in,out] json The json object to store the parsed array
 *  \param[in] text The input string containing the array
 *
 *  \return Returns a pointer to the next character after the parsed array in the string, or NULL if there was an error in parsing the array
 */
static const char* parse_array(json_t json, const char* text)
{
    json_t child, prev = NULL;

    /* Not an array! */
    if (*text != '[') 
    { 
        E(JSON_E_INVALID); 
        return NULL; 
    } 

    /* First, record the json type as array */
    json->type = JSON_TYPE_ARRAY;

    /* Skip useless characters */
    text = skip(text + 1);

    /* Encountered `]`, indicating that it is an empty array, return directly */
    if (*text == ']') return text + 1;

    /* Loop parsing each member of an array */
    do 
    {
        /* skip ',' */
        if (prev) text++; 

        /* Create json objects as array member */
        child = json_create();
        if (!child) 
        { 
            E(JSON_E_MEMORY); 
            return NULL; 
        }

        /* skip meaningless character parsing. */
        text = skip(parse_text(child, skip(text))); 
        /* parse_text has already logged the error message */
        if (!text) 
        {
            if (child) json_delete(child);
            return NULL;
        }

        /* Linking array members to an array linked list */
        if (prev) prev->next = child;
        else json->value.child_ = child;

        /* Update the previous member's pointing */
        prev = child;

    } while (*text == ','); /* Array members are separated by the symbol `,` */

    /* Not a complete array, not formed as a closed `[]` */
    if (*text != ']') 
    { 
        E(JSON_E_SQUARE); 
        return NULL; 
    }

    return text + 1; /* end of array */
}

/**
 *  \brief Parse an object in a json string and create a corresponding json object.
 *
 *  \param[out] json The json object to store the parsed object
 *  \param[in] text The input string containing the object
 *
 *  \return Returns a pointer to the next character after the parsed object in the string, or NULL if there was an error in parsing the object
 */
static const char* parse_object(json_t json, const char* text)
{
    char* key = NULL;
    json_t child, prev = NULL;

    /* Not an object! */
    if (*text != '{') 
    { 
        E(JSON_E_INVALID); 
        return NULL; 
    } 

    /* First, record the json type as object */
    json->type = JSON_TYPE_OBJECT;

    /* Skip useless characters */
    text = skip(text + 1);

    /* Encountered `}`, indicating that it is an empty object, return directly */
    if (*text == '}') return text + 1;

    /* Loop parsing each member of an object */
    do 
    {
        /* skip ',' */
        if (prev) text++;

        /* First, parse the key section */
        text = skip(parse_string_buffer(&key, skip(text)));
        if (!text)
        {
            if (key) free(key);
            E(JSON_E_VALUE);
            return NULL;
        }

        /* Not the correct key-value delimiter */
        if (*text != ':') /* fail! */
        {
            if (key) free(key);
            E(JSON_E_KEY);
            return NULL;
        }

        /* Create json objects as object member */
        child = json_create();
        if (!child) 
        { 
            E(JSON_E_MEMORY); 
            return NULL; 
        }

        /* skip any spacing, get the text. */
        text = skip(parse_text(child, skip(text + 1)));
        /* parse_text has already logged the error message */
        if (!text) 
        {
            if (key) free(key);
            if (child) json_delete(child);
            return NULL;
        }

        /* Point to key */
        child->key = key;

        /* Linking array members to an object linked list */
        if (prev) prev->next = child;
        else json->value.child_ = child;

        /* Update the previous member's pointing */
        prev = child;

    } while (*text == ','); /* Object members are separated by the symbol `,` */

    /* Not a complete object, not formed as a closed `{}` */
    if (*text != '}') { E(JSON_E_CURLY); return NULL; }

    return text + 1; /* end of object */    
}

/**
 *  \brief Parse the text part of a json value in a json string.
 *
 *  \param[in,out] json The json object to store the parsed value
 *  \param[in] text The input string containing the text part of the value
 *
 *  \return Returns a pointer to the next character after the parsed value in the string, or NULL if there was an error in parsing the value
 */
static const char* parse_text(json_t json, const char* text)
{
    if (!strncmp(text, "null", 4))
    {
        json->type = JSON_TYPE_NULL;
        return text + 4;
    }
    if (!strncmp(text, "false", 5))
    {
        json->type = JSON_TYPE_BOOL;
        json->value.bool_ = JSON_FALSE;
        return text + 5;
    }
    if (!strncmp(text, "true", 4))
    {
        json->type = JSON_TYPE_BOOL;
        json->value.bool_ = JSON_TRUE;
        return text + 4;
    }
    if (*text == '-' || (*text >= '0' && *text <= '9')) return parse_number(json, text);
    if (*text == '\"') return parse_string(json, text);
    if (*text == '[') return parse_array(json, text);
    if (*text == '{') return parse_object(json, text);
    E(JSON_E_INVALID);
    return NULL;
}

/**
 *  \brief convert numbers in json to text and append to buf.
 *  \param[in] json: json handle
 *  \param[in] buf: buf handle
 *  \return 1 success or 0 fail
 */
static int print_number(json_t json, BUFFER* buf)
{
    double num = json->value.float_;
    int len = 0;

    /* Number is 0 */
    if (num == 0)
    {
        if (!buf_append(1)) return 0;
        buf_putc('0');
    }
    /* The number type is an integer type */
    else if (json->type == JSON_TYPE_INT)
    {
        if (!buf_append(20)) return 0;
        len = sprintf(&buf_end(), "%d", (int)json->value.int_);
        buf->end += len;
    }
    /* The number type is a floating point type */
    else if (json->type == JSON_TYPE_FLOAT)
    {
        if (!buf_append(64)) return 0;
        /* use full transformation within bounded space */
        if (fabs(floor(num) - num) <= DBL_EPSILON && fabs(num) < 1.0e60) len = sprintf(&buf_end(), "%.1lf", num);
        /* use exponential form conversion beyond the limited range */
        else if (fabs(num) < 1.0e-6 || fabs(num) > 1.0e9) len = sprintf(&buf_end(), "%e", num);
        /* default conversion */
        else
        {
            len = sprintf(&buf_end(), "%lf", num);
            while (len > 0 && (&buf_end())[len-1] == '0' && (&buf_end())[len-2] != '.') len--; /* remove the invalid 0 in the decimal part */
        }
        buf->end += len;
    }
    /* Not of number type */
    else return 0;

    return 1;
}

/**
 *  \brief store c string conversion to buf.
 *  \param[in] str: address of string
 *  \param[in] buf: buf handle
 *  \return 1 success or 0 fail
 */
static int print_string_buffer(const char* str, BUFFER* buf)
{
    const char* p;
    int len = 0, escape = 0;

    /* Empty string */
    if (!str)
    {
        if (!buf_append(2)) return 0;
        buf_putc('\"');
        buf_putc('\"');
        return 1;
    }

    /* Get the length of string */
    p = str;
    while (*p)
    {
        len++;
        if (*p == '\"' || *p == '\\' || *p == '\b' || *p == '\f' || *p == '\n' || *p == '\r' || *p == '\t') /* escape character */
        {
            escape = 1;
            len++;
        }
        else if ((unsigned char)(*p) < ' ') /* control character */
        {
            escape = 1;
            len += 5; // utf
        }
        p++;
    }

    if (!buf_append(len + 2)) return 0; // \" \"
    buf_putc('\"');

    /* Without escape characters */
    if (!escape)
    {
        while (len--) buf_putc(*str++);
        buf_putc('\"');
        return 1;
    }

    p = str;
    while (*p)
    {
        if ((unsigned char)(*p) >= ' ' && *p != '\"' && *p != '\\')
        {
            buf_putc(*p++);
        }
        else
        {
            /* escape and print */
            buf_putc('\\');
            if (*p == '\\') buf_putc('\\');
            else if (*p == '\"') buf_putc('\"');
            else if (*p == '\b') buf_putc('b');
            else if (*p == '\f') buf_putc('f');
            else if (*p == '\n') buf_putc('n');
            else if (*p == '\r') buf_putc('r');
            else if (*p == '\t') buf_putc('t');
            else 
            {
                sprintf(&buf_end(), "u%04x", (unsigned char)(*p));
                buf->end += 5;
            }
            p++;
        }
    }

    buf_putc('\"');

    return 1;
}

/**
 *  \brief store json string conversion to buf.
 *  \param[in] json: json handle
 *  \param[in] buf: buf handle
 *  \return 1 success or 0 fail
 */
static int print_string(json_t json, BUFFER* buf)
{
    return print_string_buffer(json->value.string_, buf);
}

/**
 *  \brief render a array to text.
 *  \param[in] json: json handle
 *  \param[in] buf: buf handle
 *  \param[in] depth: print depth, indentation
 *  \param[in] format: 0 gives unformatted, otherwise gives formatted
 *  \return 1 success or 0 fail
 */
static int print_array(json_t json, BUFFER* buf, int depth, int format)
{
    int i = 0, count = 0;
    json_t child = json->value.child_;

    /* Empty array */
    if (!child)
    {
        if (!buf_append(2)) return 0;
        buf_putc('[');
        buf_putc(']');
        return 1;
    }

    if (format)
    {
        while (child)
        {
            /* check if there are arrays or objects in the children */
            if ((child->type == JSON_TYPE_ARRAY || child->type == JSON_TYPE_OBJECT) && child->value.child_) 
            { 
                count++; 
                break; 
            }

            child = child->next;
        }
    }

    if (!buf_append((format && count) ? 2 : 1)) return 0;
    buf_putc('[');
    if (format && count) buf_putc('\n');

    /* print children */
    child = json->value.child_;
    while (child)
    {
        /* print starting indent */
        if (format && count)
        {
            if (!buf_append(depth + 1)) return 0;
            for (i = 0; i < depth + 1; i++) buf_putc('\t');
        }

        /* print value */
        if (!print_json(child, buf, depth + 1, format)) return 0;
        
        /* print member separator ',' */
        if (child->next)
        {
            if (!buf_append(format ? 2 : 1)) return 0;
            buf_putc(',');
            if (format)
            {
                if (count) buf_putc('\n');
                else buf_putc(' ');
            }
        }

        child = child->next;
    }

    /* print ending indent */
    if (!buf_append((format && count) ? depth + 2 : 1)) return 0;
    if (format && count) 
    {
        buf_putc('\n');
        for (i = 0; i < depth; i++) buf_putc('\t');
    }
    buf_putc(']');

    return 1;
}

/**
 *  \brief render a object to text.
 *  \param[in] json: json handle
 *  \param[in] buf: buf handle
 *  \param[in] depth: print depth, indentation
 *  \param[in] format: 0 gives unformatted, otherwise gives formatted
 *  \return 1 success or 0 fail
 */
static int print_object(json_t json, BUFFER* buf, int depth, int format)
{
    int i;
    json_t child = json->value.child_;

    /* empty object */
    if (!child)
    {
        if (!buf_append(2)) return 0;
        buf_putc('{');
        buf_putc('}');
        return 1;
    }

    if (!buf_append(format ? 2 : 1)) return 0;
    buf_putc('{');
    if (format) buf_putc('\n');

    /* print children */
    while (child)
    {
        /* print starting indent */
        if (format)
        {
            if (!buf_append(depth + 1)) return 0;
            for (i = 0; i < depth + 1; i++) buf_putc('\t');
        }

        /* print key */
        if (!print_string_buffer(child->key, buf)) return 0;

        /* print indicator ':' */
        if (!buf_append(format ? 2 : 1)) return 0;
        buf_putc(':');
        if (format) buf_putc('\t');

        /* print value */
        if (!print_json(child, buf, depth + 1, format)) return 0;

        /* print separator ',' */
        if (!buf_append((child->next ? 1 : 0) + (format ? 1 : 0))) return 0;
        if (child->next) buf_putc(',');
        if (format) buf_putc('\n');

        child = child->next;
    }

    /* print ending indent */
    if (!buf_append(format ? depth + 1 : 1)) return 0;
    if (format) 
    { 
        for (i = 0; i < depth; i++) buf_putc('\t'); 
    }
    buf_putc('}');

    return 1;
}

/**
 *  \brief Print a json value to a buffer.
 *
 *  \param[in] json The json value to be printed
 *  \param[in] buf The buffer to print the json value to
 *  \param[in] depth The current depth of the json value (used for pretty printing)
 *  \param[in] format The formatting option (compact or indented)
 *
 *  \return Returns 1 if the printing is successful, 0 otherwise
 */
static int print_json(json_t json, BUFFER* buf, int depth, int format)
{
    switch (json->type)
    {
    case JSON_TYPE_NULL:
    {
        /* Print "null" */
        if (!buf_append(4)) return 0;
        buf_putc('n');
        buf_putc('u');
        buf_putc('l');
        buf_putc('l');
        break;
    }
    case JSON_TYPE_BOOL:
    {
        if (json->value.bool_ == JSON_FALSE)
        {
            /* Print "false" */
            if (!buf_append(5)) return 0;
            buf_putc('f');
            buf_putc('a');
            buf_putc('l');
            buf_putc('s');
            buf_putc('e');
        }
        else
        {
            /* Print "true" */
            if (!buf_append(4)) return 0;
            buf_putc('t');
            buf_putc('r');
            buf_putc('u');
            buf_putc('e');
        }
        break;
    }
    case JSON_TYPE_INT:
    case JSON_TYPE_FLOAT: return print_number(json, buf);
    case JSON_TYPE_STRING: return print_string(json, buf);
    case JSON_TYPE_ARRAY: return print_array(json, buf, depth, format);
    case JSON_TYPE_OBJECT: return print_object(json, buf, depth, format);
    }
    return 1;
}

/**
 *  \brief convert json to text, using a buffered strategy.
 *  \param[in] json: json handle
 *  \param[in] preset: preset is a guess at the final size, guessing well reduces reallocation
 *  \param[in] unformat: unformat=0 gives formatted, otherwise gives unformatted
 *  \param[out] len: address that receives the length of printed characters
 *  \return address of converted text, free the char* when finished
 */
char* json_dumps(json_t json, int preset, int unformat, int* len)
{
    BUFFER p, *buf = &p;

    if (!json) return NULL;

    /* Allocate buffer and initialize */
    if (preset < 1) preset = 1;
    p.address = (char*)malloc(preset);
    if (!p.address) return NULL;
    p.size = preset;
    p.end = 0;

    /* Start printing json */
    if (!print_json(json, &p, 0, !unformat)) 
    {
        free(p.address); 
        return NULL;
    }

    /* At the end of the text */
    if (!buf_append(1))
    {
        free(p.address); 
        return NULL;
    }
    buf_end() = '\0';

    /* Output conversion length */
    if (len) *len = p.end;
    
    return p.address;
}

/**
 *  \brief according to the json object, generate a file.
 *  \param[in] json: json handle
 *  \param[in] filename: file name
 *  \return file length or negative fail
 */
int json_file_dump(json_t json, char* filename)
{
    FILE* f;
    char* out;
    int len;

    if (!json) return -1;

    /* Convert characters */
    out = json_dumps(json, 0, 0, &len);
    if (!out) return -1;

    /* Open file */
    f = fopen(filename, "w");
    if (!f) 
    {
        free(out); 
        return -1;
    }

    /* Write the string to the file */
    fwrite(out, 1, len, f);

    /* Close file */
    fclose(f);

    /* free the sapce of string */
    free(out);

    return len;
}

/**
 *  \brief json text parser.
 *  \param[in] text: address of text
 *  \return the address of the next meaningful character
 */
json_t json_loads(const char* text)
{
    json_t json = NULL;

    /* reset error info */
    lbegin = text;
    eline = 1;
    etype = JSON_E_OK;

    /* create json object and parse */
    json = json_create();
    if (!json) 
    { 
        E(JSON_E_MEMORY); 
        return NULL;
    }

    text = parse_text(json, skip(text));
    /* parse failure. error is set. */
    if (!text) 
    { 
        json_delete(json); 
        return NULL; 
    }

    /* check whether there are meaningless characters after the text after parsing */
    text = skip(text);
    if (*text) 
    { 
        json_delete(json); 
        E(JSON_E_END); 
        return NULL; 
    }

    return json;
}

/**
 *  \brief load a json file, parse and generate json objects.
 *  \param[in] filename: file name
 *  \return json handle or NULL fail
 */
json_t json_file_load(char* filename)
{
    FILE* f;
    json_t json = NULL;
    long len;
    char* data;

    /* Open file */
    f = fopen(filename, "rb");
    if (!f) return NULL;

    /* Get the length of file */
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    /* read file */
    data = (char*)malloc(len + 1);
    if (data) 
    { 
        /* Read the file content into `data` */
        fread(data, 1, len, f); 
        fclose(f); 
    }
    else 
    { 
        fclose(f); 
        return NULL; 
    }

    /* Add string terminator */
    data[len] = 0;

    /* Load json string */
    json = json_loads(data);

    /* Free up space */
    free(data);

    return json;
}

/**
 *  \brief minify json text, remove the character that does not affect the analysis.
 *  \param[in] text: the address of the source text
 *  \return none
 */
void json_minify(char* text)
{
    char* into = text;
    while (*text)
    {
        /* whitespace characters. */
        if (*text == ' ' || *text == '\t' || *text == '\r' || *text == '\n')
        {
            text++;
        }
        /* double-slash comments, to end of line. */
        else if (*text == '/' && text[1] == '/')
        {
            while (*text && *text != '\n') text++;
        }
        /* multiline comments. */
        else if (*text == '/' && text[1] == '*') 
        {
            while (*text && !(*text == '*' && text[1] == '/')) text++;
            text += 2;
        }
        /* string literals, which are \" sensitive. */
        else if (*text == '\"') 
        {
            *into++ = *text++;
            while (*text && *text != '\"')
            {
                if (*text == '\\') *into++ = *text++;
                *into++ = *text++;
            }
            *into++ = *text++;
        }
        /* all other characters. */
        else *into++ = *text++; 
    }
    *into = 0; /* and null-terminate. */
}
