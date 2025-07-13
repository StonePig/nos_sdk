/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  str.h
 *         \unit  str
 *        \brief  This is a general C language string container module
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __str_H
#define __str_H

#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>

/* version infomation */

#define STR_V_MAJOR                         1
#define STR_V_MINOR                         0
#define STR_V_PATCH                         0

/* str type definition, hiding structural members, not for external use */

typedef struct STR *str_t;

/** 
 *  \brief create str
 *  \param[in] string: string, type can be array string or str
 *  \return handler of new str
 */
str_t str_create(void *string);

/** 
 *  \brief delete str
 *  \param[in] str: str handler
 *  \return none
 */
void str_delete(str_t str);

/** 
 *  \brief assign value to str
 *  \param[in] str: str handler
 *  \param[in] string: string, type can be array string or str
 *  \return str itself
 */
str_t str_assign(str_t str, void *string);

/**
 *  \brief get the data actually stored in str
 *  \param[in] str: str handler
 *  \param[in] pos: position of str
 *  \return address of string data
 */
char* str_data(str_t str, int pos);

/** 
 *  \brief get the string of str
 *  \param[in] str: str handler
 *  \return address of str string
 */
const char* str_c_str(str_t str);

/** 
 *  \brief get the length of str
 *  \param[in] str: str handler
 *  \return length of str
 */
int str_length(str_t str);

/** 
 *  \brief get the capacity of the space for storing str data
 *  \param[in] str: str handler
 *  \return capacity of str
 */
int str_capacity(str_t str);

/** 
 *  \brief judge whether the str is empty
 *  \param[in] str: str handler
 *  \return 1: empty or 0: not empty
 */
int str_empty(str_t str);

/** 
 *  \brief clear str
 *  \param[in] str: str handler
 *  \return none
 */
void str_clear(str_t str);

/** 
 *  \brief Insert string to str
 *  \param[in] str: str handler
 *  \param[in] pos: insertion position
 *  \param[in] string: string, type can be array string or str
 *  \return str itself
 */
str_t str_insert(str_t str, int pos, void *string);

/** 
 *  \brief Erase string in str
 *  \param[in] str: str handler
 *  \param[in] pos: start position of erasure
 *  \param[in] len: erased length
 *  \return str itself
 */
str_t str_erase(str_t str, int pos, int len);

/** 
 *  \brief insert a character at the end of str
 *  \param[in] str: str handler
 *  \param[in] c: char
 *  \return 1: success or 0: fail
 */
int str_push_back(str_t str, char c);

/** 
 *  \brief erase a character at the end of str
 *  \param[in] str: str handler
 *  \return char of erase
 */
char str_pop_back(str_t str);

/** 
 *  \brief append series string to the end of str, it is recommended to directly use the macro definition method of 'str_append()'
 *  \param[in] str: str handler
 *  \param[in] ...: strings, at least one, type can be array string or str
 *  \return str itself
 */
str_t str_append_series(str_t str, ...);

/** 
 *  \brief compare two strings
 *  \param[in] str: str handler
 *  \param[in] string: string, type can be array string or str
 *  \return 0: equal, -1: str < string, 1: str > string
 */
int str_compare(str_t str, void *string);

/** 
 *  \brief create substring
 *  \param[in] str: str handler
 *  \param[in] string: string, type can be array string or str
 *  \return new substring
 */
str_t str_substr(str_t str, int pos, int len);

/** 
 *  \brief find the specified string in str
 *  \param[in] str: str handler
 *  \param[in] string: string, type can be array string or str
 *  \param[in] pos: the position to start find
 *  \return find the first occurrence position of the specified string or str_npos: fail
 */
int str_find(str_t str, void *string, int pos);

/** 
 *  \brief find the specified string in str from tail
 *  \param[in] str: str handler
 *  \param[in] string: string, type can be array string or str
 *  \param[in] pos: the position to start find
 *  \return find the first occurrence position of the specified string or str_npos: fail
 */
int str_rfind(str_t str, void *string, int pos);

/** 
 *  \brief the first find contains the characters in the specified string
 *  \param[in] str: str handler
 *  \param[in] string: string, type can be array string or str
 *  \param[in] pos: the position to start find
 *  \return position of the specified character or str_npos: fail
 */
int str_find_first_of(str_t str, void *string, int pos);

/** 
 *  \brief the first find contains the characters not in the specified string
 *  \param[in] str: str handler
 *  \param[in] string: string, type can be array string or str
 *  \param[in] pos: the position to start find
 *  \return position of the specified character or str_npos: fail
 */
int str_find_first_not_of(str_t str, void *string, int pos);

/** 
 *  \brief the last find contains the characters in the specified string
 *  \param[in] str: str handler
 *  \param[in] string: string, type can be array string or str
 *  \param[in] pos: the position to start find
 *  \return position of the specified character or str_npos: fail
 */
int str_find_last_of(str_t str, void *string, int pos);

/** 
 *  \brief the last find contains the characters not in the specified string
 *  \param[in] str: str handler
 *  \param[in] string: string, type can be array string or str
 *  \param[in] pos: the position to start find
 *  \return position of the specified character or str_npos: fail
 */
int str_find_last_not_of(str_t str, void *string, int pos);

/** 
 *  \brief format str
 *  \param[in] str: str handler
 *  \param[in] begin: begin position
 *  \param[in] end: end position
 *  \return str itself
 */
str_t str_reverse(str_t str, int begin, int end);

/** 
 *  \brief str replace
 *  \param[in] str: str handler
 *  \param[in] pos: start position of be replace
 *  \param[in] len: erased length
 *  \param[in] string: string, type can be array string or str
 *  \return str itself
 */
str_t str_replace(str_t str, int pos, int len, void *string);

/** 
 *  \brief swap two str
 *  \param[in] str: str handler
 *  \param[in] swap: str handler of swap
 *  \return none
 */
void str_swap(str_t str, str_t swap);

/** 
 *  \brief swap two str
 *  \param[in] str: str handler
 *  \param[in] pos: start position of be copy
 *  \param[in] len: copy length
 *  \param[out] buf: array copied
 *  \return number of characters actually copied
 */
int str_copy(str_t str, int pos, int len, char *buf);

/** 
 *  \brief format str
 *  \param[in] str: str handler
 *  \param[in] format: format
 *  \param[in] ...: indefinite parameter
 *  \return str itself
 */
str_t str_format(str_t str, char *format, ...);

/**
 *  \brief A simple method for `str_create`.
 *  \param[in] str: string, type can be array string or str
 *  \return handler of new str
 */
#define str(str)                            str_create(str)

/**
 *  \brief A simple method for `str_delete`.
 *  \param[in] str: str handler
 *  \return none
 */
#define _str(str)                           do{str_delete(str);(str)=NULL;}while(0)

/**
 *  \brief append series string to the end of str.
 *  \param[in] str: str handler
 *  \param[in] ...: strings, at least one, type can be array string or str
 *  \return str itself
 */
#define str_append(str, ...)                str_append_series((str), ##__VA_ARGS__, NULL)

/**
 *  \brief Random access method for str char.
 *  \param[in] str: str handler
 *  \param[in] i: index of str
 *  \return Reference to str char
 */
#define str_at(str, i)                      (*str_data(str, i))

/** 
 *  \brief multiplexing `str_c_str()`, get the string of str
 *  \param[in] str: str handler
 *  \return reference to the back char
 */
#define _S(str)                             ((const char*)str_c_str(str))

/* non-existent position */
#define str_npos                            INT_MAX

#endif
