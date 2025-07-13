/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  json.h
 *         \unit  json
 *        \brief  This is a C language version of json streamlined parser
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __json_H
#define __json_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdarg.h>
#include <limits.h>
#include <stdlib.h>

/* version infomation */

#define JSON_V_MAJOR                        1
#define JSON_V_MINOR                        0
#define JSON_V_PATCH                        0

/* json type definition, hiding structural members, not for external use */

typedef struct JSON* json_t;

/* json normal types define */

#define JSON_TYPE_UNKNOW                    (0) /* unknown type */
#define JSON_TYPE_NULL                      (1) /* null type */
#define JSON_TYPE_BOOL                      (2) /* bool type */
#define JSON_TYPE_INT                       (3) /* number int type */
#define JSON_TYPE_FLOAT                     (4) /* number float type */
#define JSON_TYPE_STRING                    (5) /* string type */
#define JSON_TYPE_ARRAY                     (6) /* array type */
#define JSON_TYPE_OBJECT                    (7) /* object type */

/* bool define */
#define JSON_FALSE                          (0) /* bool false */
#define JSON_TRUE                           (1) /* bool true */

/* error type define */
#define JSON_E_OK                           (0) /* ok, no error */
#define JSON_E_INVALID                      (1) /* invalid, not a valid expected value */
#define JSON_E_END                          (2) /* many invalid characters appear at the end */
#define JSON_E_KEY                          (3) /* parsing key, invalid key content found */
#define JSON_E_VALUE                        (4) /* parsing value, invalid value content found */
#define JSON_E_MEMORY                       (5) /* memory allocation failed */
#define JSON_E_SQUARE                       (6) /* mising ']' */
#define JSON_E_CURLY                        (7) /* mising '}' */

/* Load json */

json_t json_loads(const char* text);
json_t json_file_load(char* filename);

/* When loading fails, use this method to locate the error */

int json_error_info(int* line, int* column);

/* Dump json */

char* json_dumps(json_t json, int preset, int unformat, int* len);
int json_file_dump(json_t json, char* filename);

/* Create and delete json */

json_t json_create(void);
void json_delete(json_t json);

/* Get information of array/object json */

int json_size(json_t json);
int json_type(json_t json);

/* Get json key and value */
/* Before getting the value, it is recommended to judge whether it is the target type before getting it */

const char* json_key(json_t json);
int json_value_bool(json_t json);
int json_value_int(json_t json);
double json_value_float(json_t json);
const char* json_value_string(json_t json);
json_t json_value_array(json_t json);
json_t json_value_object(json_t json);

/* Modify json basic type value */
/* These methods have coverage and will overwrite the original data to change it to the specified data */

json_t json_set_key(json_t json, const char* key);
json_t json_set_null(json_t json);
json_t json_set_bool(json_t json, int b);
json_t json_set_int(json_t json, int num);
json_t json_set_float(json_t json, double num);
json_t json_set_string(json_t json, const char* string);
json_t json_set_object(json_t json, json_t object);
json_t json_set_array(json_t json, json_t array);

/* These functions set json as an array of basic types in C language */

json_t json_set_array_int(json_t json, const int* numbers, int count);
json_t json_set_array_float(json_t json, const float* numbers, int count);
json_t json_set_array_double(json_t json, const double* numbers, int count);
json_t json_set_array_string(json_t json, const char** strings, int count);

/* These methods obtain child json */

json_t json_get(json_t json, const char* key, int index);
json_t json_to_index_valist(json_t json, int index, ...);
json_t json_to_key_valist(json_t json, char* key, ...);

/* json storage structure adjustment method */
/* Usually used in conjunction with other methods */

json_t json_attach(json_t json, int index, json_t ins);
json_t json_detach(json_t json, const char* key, int index);

/* json format text minify */

void json_minify(char* text);

/* json deep copy */

json_t json_copy(json_t json);

/* These creation methods are composed of functions, */
/* that can create json basic types, which can be added to json arrays and child json objects */

#define json_create_null_for_object(key)        (json_set_key(json_set_null(json_create()),(key)))
#define json_create_true_for_object(key)        (json_set_key(json_set_bool(json_create(),JSON_TRUE),(key)))
#define json_create_false_for_object(key)       (json_set_key(json_set_bool(json_create(),JSON_FALSE),(key)))
#define json_create_bool_for_object(key, b)     (json_set_key(json_set_bool(json_create(),(b)),(key)))
#define json_create_int_for_object(key, n)      (json_set_key(json_set_int(json_create(),(n)),(key)))
#define json_create_float_for_object(key, n)    (json_set_key(json_set_float(json_create(),(n)),(key)))
#define json_create_string_for_object(key, s)   (json_set_key(json_set_string(json_create(),(s)),(key)))
#define json_create_array_for_object(key)       (json_set_key(json_set_array(json_create(),NULL),(key)))
#define json_create_object_for_object(key)      (json_set_key(json_set_object(json_create(),NULL),(key)))
#define json_create_null_for_array()            (json_set_null(json_create()))
#define json_create_true_for_array()            (json_set_bool(json_create(),JSON_TRUE))
#define json_create_false_for_array()           (json_set_bool(json_create(),JSON_FALSE))
#define json_create_bool_for_array(b)           (json_set_bool(json_create(),(b)))
#define json_create_int_for_array(n)            (json_set_int(json_create(),(n)))
#define json_create_float_for_array(n)          (json_set_float(json_create(),(n)))
#define json_create_string_for_array(s)         (json_set_string(json_create(),(s)))
#define json_create_array_for_array()           (json_set_array(json_create(),NULL))
#define json_create_object_for_array()          (json_set_object(json_create(),NULL))

/* Add json item to array and object */
/* The default location for adding is at the tail. 
/* Because json objects use a unidirectional linked list storage structure, which can be time-consuming for continuous tail insertion. */ 
/* Head insertion can be carried out according to the actual situation */

#define json_add_null_to_object(json, key)      (json_isobject(json)?json_attach(json,json_size(json),json_create_null_for_object(key)):0)
#define json_add_true_to_object(json, key)      (json_isobject(json)?json_attach(json,json_size(json),json_create_true_for_object(key)):0)
#define json_add_false_to_object(json, key)     (json_isobject(json)?json_attach(json,json_size(json),json_create_false_for_object(key)):0)
#define json_add_bool_to_object(json, key, b)   (json_isobject(json)?json_attach(json,json_size(json),json_create_bool_for_object(key,b)):0)
#define json_add_int_to_object(json, key, n)    (json_isobject(json)?json_attach(json,json_size(json),json_create_int_for_object(key,n)):0)
#define json_add_float_to_object(json, key, n)  (json_isobject(json)?json_attach(json,json_size(json),json_create_float_for_object(key,n)):0)
#define json_add_string_to_object(json, key, s) (json_isobject(json)?json_attach(json,json_size(json),json_create_string_for_object(key,s)):0)
#define json_add_array_to_object(json, key)     (json_isobject(json)?json_attach(json,json_size(json),json_create_array_for_object(key)):0)
#define json_add_object_to_object(json, key)    (json_isobject(json)?json_attach(json,json_size(json),json_create_object_for_object(key)):0)
#define json_add_null_to_array(json)            (json_isarray(json)?json_attach(json,json_size(json),json_create_null_for_array()):0)
#define json_add_true_to_array(json)            (json_isarray(json)?json_attach(json,json_size(json),json_create_true_for_array()):0)
#define json_add_false_to_array(json)           (json_isarray(json)?json_attach(json,json_size(json),json_create_false_for_array()):0)
#define json_add_bool_to_array(json, b)         (json_isarray(json)?json_attach(json,json_size(json),json_create_bool_for_array(b)):0)
#define json_add_int_to_array(json, n)          (json_isarray(json)?json_attach(json,json_size(json),json_create_int_for_array(n)):0)
#define json_add_float_to_array(json, n)        (json_isarray(json)?json_attach(json,json_size(json),json_create_float_for_array(n)):0)
#define json_add_string_to_array(json, s)       (json_isarray(json)?json_attach(json,json_size(json),json_create_string_for_array(s)):0)
#define json_add_array_to_array(json)           (json_isarray(json)?json_attach(json,json_size(json),json_create_array_for_array()):0)
#define json_add_object_to_array(json)          (json_isarray(json)?json_attach(json,json_size(json),json_create_object_for_array()):0)

/* These methods are used to detach and erase child json from its parent json */
/* Divided into the form of indexe or key */

#define json_detach_by_index(json, index)       json_detach(json,NULL,(index))
#define json_detach_by_key(json, key)           json_detach(json,(key),0)
#define json_erase(json, key, index)            json_delete(json_detach((json),(key),(index)))
#define json_erase_by_index(json, index)        json_erase(json,NULL,index)
#define json_erase_by_key(json, key)            json_erase(json,key,0)

/* Determine whether json is the specified basic type */

#define json_isnull(json)                       (json_type(json)==JSON_TYPE_NULL)
#define json_isbool(json)                       (json_type(json)==JSON_TYPE_BOOL)
#define json_isint(json)                        (json_type(json)==JSON_TYPE_INT)
#define json_isfloat(json)                      (json_type(json)==JSON_TYPE_FLOAT)
#define json_isnumber(json)                     (json_isint(json)||json_isfloat(json))
#define json_isstring(json)                     (json_type(json)==JSON_TYPE_STRING)
#define json_isarray(json)                      (json_type(json)==JSON_TYPE_ARRAY)
#define json_isobject(json)                     (json_type(json)==JSON_TYPE_OBJECT)

/* Continuously input index or key to recursively obtain child json */

#define json_to_index(json, i, ...)             (json_to_index_valist(json,(i),##__VA_ARGS__,INT_MIN))
#define json_to_key(json, key, ...)             (json_to_key_valist(json,(key),##__VA_ARGS__,NULL))

#ifdef __cplusplus
}
#endif

#endif
