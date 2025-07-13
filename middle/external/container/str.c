/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  str.c
 *         \unit  str
 *        \brief  This is a general C language string container module
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "str.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

/* str define */
typedef struct STR
{
    char ident;                             /**< ident of str */
    char* base;                             /**< base address of string */
    int length;                             /**< length of str */
    int capacity;                           /**< capacity of str */
} STR;

/* str identification mark */
#define ident()                             (-1)

/* tool */
#define up_multiple(x, mul)                 ((x)+((mul)-((x)-1)%(mul))-1)
#define is_digit(c)                         ((c)>='0'&&(c)<='9')
#define MAX(x, y)                           ((x)>(y)?(x):(y))

/* error space */
static char error = 0;

/* str info define */
typedef struct 
{
    char *base;                             /**< base address of string */
    int length;                             /**< length of str */
} str_info_t;

/** 
 *  \brief calculate capacity according to gradient
 *  \param[in] size: size
 *  \return capacity
 */
static int gradient_capacity(int size)
{
    int capacity = 1;
    if (size <= 1) return 1;
    while (capacity < size) capacity <<= 1;
    capacity >>= 1;
    if (capacity < 4) capacity = capacity << 1;
    else if (capacity < 16) capacity = up_multiple(size, capacity >> 1);
    else if (capacity < 256) capacity = up_multiple(size, capacity >> 2);
    else capacity = up_multiple(size, 64);
    return capacity;
}

/** 
 *  \brief get string information
 *  \param[in] string: string, type can be array string or str
 *  \return information of string
 */
static str_info_t string_info(void *string)
{
    str_info_t info;

    if (((char *)string)[0] == 0) /* empty array string */
    {
        info.base = (char *)string;
        info.length = 0;
    }
    else if (((str_t)string)->ident == ident()) /* str type */
    {
        info.base = ((str_t)string)->base;
        info.length = ((str_t)string)->length;
    }
    else /* array string type */
    {
        info.base = (char *)string;
        info.length = strlen((char *)string);
    }

    return info;
}

/** 
 *  \brief alter the capacity of the str
 *  \param[in] str: str handler
 *  \param[in] length: length of str
 *  \return new capacity of str
 */
static int str_alter_capacity(str_t str, int length)
{
    int capacity = 0;
    char *base = NULL;

    /* check whether the capacity changes and need to adjust the capacity space */
    capacity = gradient_capacity(length);
    if (str->capacity != 0 && str->capacity == capacity) return 1;
    if (!str->base) /* allocate new space */
    {
        str->base = (char *)malloc(capacity + 1);
        if (!str->base) return 0;
    }
    else /* reallocate space */
    {
        base = (char *)realloc(str->base, capacity + 1);
        if (!base) return 0;
        str->base = base;
    }
    str->capacity = capacity;
    return 1;
}

str_t str_create(void *string)
{
    str_t str;

    /* Input value validity check */
    if (!string) return NULL;

    /* Allocate memory for the STR structure */
    str = (str_t)malloc(sizeof(STR));
    if (!str) return NULL;

    /* Initialize structural parameters */
    str->ident = ident();
    str->base = NULL;
    str->length = 0;
    str->capacity = 0;

    /* Assign initial value */
    if (!str_assign(str, string))
    {
        free(str);
        return NULL;
    }

    return str;
}

void str_delete(str_t str)
{
    /* Input value validity check */
    if (!str) return;

    /* If it is not a constant array, release the allocated space */
    if (str->base) free(str->base);

    /* Free str structure */
    free(str);
}

str_t str_assign(str_t str, void *string)
{
    str_info_t info;

    /* Input value validity check */
    if (!str) return NULL;
    if (!string) return NULL;

    /* Get basic information about the incoming string */
    info = string_info(string);

    /* Allocate space to the string */
    if (!str_alter_capacity(str, info.length)) return NULL;

    /* Assignment string */
    strcpy(str->base, info.base);

    /* Update str status */
    str->length = info.length;

    return str;
}

char* str_data(str_t str, int pos)
{
    /* Input value validity check */
    if (!str || pos < 0 || pos >= str->length) 
    {
        error = 0; /* reset error area */
        return &error;
    }

    /* Return the base address of the string storage */
    return &str->base[pos];
}

const char* str_c_str(str_t str)
{
    return (const char *)str_data(str, 0);
}

int str_length(str_t str)
{
    /* Input value validity check */
    if (!str) return 0;

    /* Return the length of the string */
    return str->length;
}

int str_capacity(str_t str)
{
    /* Input value validity check */
    if (!str) return 0;

    /* Return the capacity of the string */
    return str->capacity;
}

int str_empty(str_t str)
{
    /* Input value validity check */
    if (!str) return 1;

    /* Judging whether it is empty based on its length */
    return str->length == 0 ? 1 : 0;
}

void str_clear(str_t str)
{
    /* Directly assign a value to an empty string */
    str_assign(str, "");
}

str_t str_insert(str_t str, int pos, void *string)
{
    return str_replace(str, pos, 0, string);
}

str_t str_erase(str_t str, int pos, int len)
{
    return str_replace(str, pos, len, "");
}

str_t str_append_series(str_t str, ...)
{
    va_list args;
    void* s = NULL;

    /* Input value validity check */
    if (!str) return NULL;

    va_start(args, str);

    /* Loop to obtain indefinite parameters */
    s = va_arg(args, void*);
    while (s)
    {
        /* insert at the end */
        if (!str_insert(str, str->length, s))
        {
            va_end(args);
            return NULL;
        }
        s = va_arg(args, void*);
    }

    va_end(args);

    return str;
}

int str_push_back(str_t str, char c)
{
    char temp[2] = {0};

    /* Input value validity check */
    if (!str) return 0;

    /* Combine characters into a short string */
    temp[0] = c;

    /* Append characters to the end */
    if (!str_append_series(str, temp, NULL)) return 0;

    return 1;
}

char str_pop_back(str_t str)
{
    char c = 0;

    /* Input value validity check */
    if (!str) return 0;

    /* Check if the string is not empty */
    if (str->length <= 0) return 0;

    /* Record Tail */
    c = str->base[str->length - 1];

    /* Remove the tail */
    if (!str_erase(str, str->length - 1, 1)) return 0;

    return c;
}

int str_compare(str_t str, void *string)
{
    str_info_t info;
    int ret = 0;

    /* Input value validity check */
    if (!str) return -1;
    if (!string) return 1;

    /* Get basic information about the incoming string */
    info = string_info(string);

    /* Compare two strings */
    ret = strcmp(str->base, info.base);

    /* Update return value */
    if (ret > 0) ret = 1;
    else if (ret < 0) ret = -1;

    return ret;
}

str_t str_substr(str_t str, int pos, int len)
{
    str_t copy = NULL;

    /* Input value validity check */
    if (!str) return NULL;
    if (pos < 0 || pos >= str->length) return NULL;
    if (len <= 0) return NULL;

    /* Create an empty string */
    copy = str_create("");
    if (!copy) return NULL;

    /* Update the copied length */
    if (len > str->length - pos) len = str->length - pos;

    /* Allocate space to the string */
    if (!str_alter_capacity(copy, len))
    {
        str_delete(copy);
        return NULL;
    }

    /* Assignment string */
    memcpy(copy->base, &str->base[pos], len);
    copy->base[len] = 0;
    copy->length = len;

    return copy;
}

int str_find(str_t str, void *string, int pos)
{
    str_info_t info;
    char *find = NULL;
    int ret = 0;

    /* Input value validity check */
    if (!str) return str_npos;
    if (!string) return str_npos;
    if (pos < 0 || pos >= str->length) return str_npos;

    /* Get basic information about the incoming string */
    info = string_info(string);

    /* The length of the substring exceeds the limit, return directly */
    if (info.length > str->length) return str_npos;

    /* Find substrings */
    find = strstr(&str->base[pos], info.base);
    if (!find) return str_npos;

    /* Calculate find location */
    ret = find - str->base;

    return ret;
}

/** 
 *  \brief match substrings from the tail
 *  \param[in] *haystack: parent string
 *  \param[in] tail_index: specifies the end index of the parent string character array
 *  \param[in] *needle: child string
 *  \return matching segment in the parent string or NULL: fail
 */
static char* strrstr(char *haystack, int end_index, char *needle)
{
    char *find = NULL;
    int index = 0;
    int i = 0;
    int hay_len = end_index;
    int need_len = strlen(needle);
    for (index = hay_len; index >= 0; index--)
    {
        if (haystack[index] == needle[0])
        {
            find = &haystack[index];
            for (i = 0; i < need_len; i++)
            {
                if (find[i] != needle[i]) break;
            }
            if (i == need_len) return find;
        }
    }

    return NULL;
}

int str_rfind(str_t str, void *string, int pos)
{
    str_info_t info;
    char *find = NULL;
    int ret = 0;

    /* Input value validity check */
    if (!str) return str_npos;
    if (!string) return str_npos;
    if (pos < 0 || pos >= str->length) return str_npos;

    /* Get basic information about the incoming string */
    info = string_info(string);

    /* The length of the substring exceeds the limit, return directly */
    if (info.length > str->length) return str_npos;

    /* Find substrings */
    find = strrstr(str->base, pos, info.base);
    if (!find) return str_npos;

    /* Calculate find location */
    ret = find - str->base;

    return ret;
}

/** str_find_of
 *  \brief find function of "of" type
 *  \param[in] str: str handler
 *  \param[in] *string: string, type can be array string or str
 *  \param[in] pos: the position to start find
 *  \param[in] from: 0: first, 1: last
 *  \param[in] of: 0: not of, 1: of
 *  \return position of the specified character or str_npos: fail
 */
static int str_find_of(str_t str, void *string, int pos, int from, int of)
{
    str_info_t info;
    int i = 0;
    if (!str) return str_npos;
    if (!string) return str_npos;
    if (pos < 0 || pos >= str->length) return str_npos;
    info = string_info(string);
    if (info.length <= 0) return str_npos;

    /* matching */
    if (from == 0) /* first */
    {
        for (i = pos; i < str->length; i++)
        {
            if ((of == 1 && strchr(info.base, str->base[i])) ||
                (of == 0 && !strchr(info.base, str->base[i])))
            {
                return i;
            }
        }
    }
    else /* last */
    {
        for (i = pos; i >= 0; i--)
        {
            if ((of == 1 && strchr(info.base, str->base[i])) ||
                (of == 0 && !strchr(info.base, str->base[i])))
            {
                return i;
            }
        }
    }

    return str_npos;
}

int str_find_first_of(str_t str, void *string, int pos)
{
    return str_find_of(str, string, pos, 0, 1);
}

int str_find_first_not_of(str_t str, void *string, int pos)
{
    return str_find_of(str, string, pos, 0, 0);
}

int str_find_last_of(str_t str, void *string, int pos)
{
    return str_find_of(str, string, pos, 1, 1);
}

int str_find_last_not_of(str_t str, void *string, int pos)
{
    return str_find_of(str, string, pos, 1, 0);
}

str_t str_reverse(str_t str, int begin, int end)
{
    int i = 0;
    char c = 0;

    /* Input value validity check */
    if (!str) return NULL;
    if (begin < 0) begin = 0;
    if (end >= str->length) end = str->length - 1;
    if (begin >= end) return NULL;

    /* Reverse characters with symmetrical positions in a loop */
    for (i = 0; i <= (end - begin) / 2; i++)
    {
        c = str->base[begin + i];
        str->base[begin + i] = str->base[end - i];
        str->base[end - i] = c;
    }

    return str;
}

str_t str_replace(str_t str, int pos, int len, void *string)
{
    str_info_t info;
    char *overlap = NULL;

    /* Input value validity check */
    if (!str) return NULL;
    if (!string) return NULL;
    if (pos < 0 || pos > str->length) return NULL;

    /* Update the copied length */
    if (len > str->length - pos) len = str->length - pos;

    /* Get basic information about the incoming string */
    info = string_info(string);
    
    /* Check if addresses overlap */
    if (str->base <= info.base && info.base <= str->base + str->length && pos < str->length)
    {
        /* Allocate temporary space */
        overlap = (char *)malloc(info.length + 1);
        if (!overlap) return NULL;
        strcpy(overlap, info.base);
        info.base = overlap;
    }

    if (info.length > len) /* lengthen */
    {
        if (str_alter_capacity(str, str->length + (info.length - len)) == 0)
        {
            if (overlap) free(overlap);
            return NULL;
        }
        memmove(&str->base[pos + info.length], &str->base[pos + len], str->length - (pos + len));
        memcpy(&str->base[pos], info.base, info.length);
    }
    else if (info.length < len) /* shorten */
    {
        memmove(&str->base[pos + info.length], &str->base[pos + len], str->length - (pos + len));
        memcpy(&str->base[pos], info.base, info.length);
        str_alter_capacity(str, str->length + (info.length - len));
    }
    else  
    {
        memcpy(&str->base[pos], info.base, info.length);
    }

    str->length += (info.length - len);
    str->base[str->length] = 0;

    /* Free temporary space */
    if (overlap) free(overlap);

    return str;
}

void str_swap(str_t str, str_t swap)
{
    STR temp;

    /* Input value validity check */
    if (!str) return;
    if (!swap) return;

    /* Swap string structures */
    temp = *str;
    *str = *swap;
    *swap = temp;
}

int str_copy(str_t str, int pos, int len, char *buf)
{
    /* Input value validity check */
    if (!str) return 0;
    if (pos < 0 || pos > str->length) return 0;
    if (len > str->length - pos) len = str->length - pos;

    /* Copy out a string */
    memcpy(buf, &str->base[pos], len);

    return len;
}

/** 
 *  \brief get digit from string
 *  \param[in] s: char string
 *  \param[out] out_digit: out digit
 *  \return length of digit
 */
static int get_digit(char *s, int *out_digit)
{
    int digit = 0;
    int len = 0;
    while (is_digit(*s))
    {
        digit = digit * 10 + *(s++) - '0';
        len++;
    }
    *out_digit = digit;
    return len;
}

str_t str_format(str_t str, char *format, ...)
{
    va_list args;
    int len = 0;                        /* length of sub string */
    char *s = NULL;                     /* temp string */
    str_info_t info;                    /* str info */
    double dbl = 0.0;                   /* double precision floating point */
    char *begin = NULL;                 /* begin of format */
    char qualifier = 0;                 /* conversion qualifier for integer as 'h','l','L' */
    int width = 0;                      /* transition field width */
    int precision = 0;                  /* minimum digits of integers and maximum digits of strings */
    char tfmt[16] = "%";                /* temporary format */

    /* Input value validity check */
    if (!str) return NULL;

    /* Reset str */
    str_assign(str, "");

    va_start(args, format);

    /* Traverse format string */
    for (; *format; format++)
    {
        if (*format != '%')
        {
            if (!begin) begin = format;
            continue;
        }
        if (begin)
        {
            len = format - begin;
            if (!str_alter_capacity(str, str->length + len)) goto FAIL_CAPACITY;
            while (len--) str->base[str->length++] = *begin++;
            begin = NULL;
        }
        begin = format;
        while (1)
        {
            /* skips the first '%' also */
            format++;
            if ((*format != '0') &&
                (*format != ' ') && 
                (*format != '+') && 
                (*format != '-') && 
                (*format != '#')) break;
        }

        /* get field width */
        width = -1;
        if (is_digit(*format)) 
        {
            format += get_digit(format, &width);
        }
        else if (*format == '*')
        {
            format++;
            width = va_arg(args, int);
            if (width < 0) width = -width;
        }

        /* get the precision */
        precision = -1;
        if (*format == '.')
        {
            format++;
            if (is_digit(*format)) format += get_digit(format, &precision);
            else if (*format == '*')
            {
                format++;
                precision = va_arg(args, int);
            }
            if (precision < 0) precision = 0;
        }

        /* get the conversion qualifier */
        qualifier = 0;
        if (*format == 'h' || *format == 'l' || *format == 'L')
        {
            qualifier = *format;
            format++;
            if (qualifier == 'l' && *format == 'l')
            {
                qualifier = 'L';
                format++;
            }
        }

        /* format distribution */
        switch (*format)
        {
        case 'c':
            len = MAX(width, 1);
            if (!str_alter_capacity(str, str->length + len)) goto FAIL_CAPACITY;
            memcpy(&tfmt[1], begin + 1, format - begin);
            tfmt[format - begin + 1] = 0;
            begin = NULL;
            len = sprintf(&str->base[str->length], tfmt, va_arg(args, int));
            if (len > 0) str->length += len;
            break;
        case 's':
            info = string_info(va_arg(args, void*));
            s = info.base;
            len = info.length;
            if (precision > 0 && len > precision) len = precision;
            len = MAX(width, len);
            if (!str_alter_capacity(str, str->length + len)) goto FAIL_CAPACITY;
            memcpy(&tfmt[1], begin + 1, format - begin);
            tfmt[format - begin + 1] = 0;
            begin = NULL;
            len = sprintf(&str->base[str->length], tfmt, s);
            if (len > 0) str->length += len;
            break;
        case 'p':
            if (width == -1) width = sizeof(void*) * 2; /* if no width is specified, the default width will be used */
            len = width;
            if (!str_alter_capacity(str, str->length + len)) goto FAIL_CAPACITY;
            memcpy(&tfmt[1], begin + 1, format - begin);
            tfmt[format - begin + 1] = 0;
            begin = NULL;
            len = sprintf(&str->base[str->length], tfmt, (long)va_arg(args, void*));
            if (len > 0) str->length += len;
            break;
            /* float number formats */
        case 'a':
        case 'A':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
        case 'f':
            dbl = va_arg(args, double);
            if (*format == 'f')
            {
                /* get the gradient length of the integer part of the floating point number */
                if (fabs(dbl) < powl(10, 8)) len = 8;
                else if (fabs(dbl) < powl(10, 16)) len = 16;
                else if (fabs(dbl) < powl(10, 32)) len = 32;
                else if (fabs(dbl) < powl(10, 64)) len = 64;
                else if (fabs(dbl) < powl(10, 128)) len = 128;
                else if (fabs(dbl) < powl(10, 256)) len = 256;
                else len = 309;
            }
            else len = 24;

            len += ((precision==-1?6:precision) + 1);
            len = MAX(width, len);
            if (!str_alter_capacity(str, str->length + len)) goto FAIL_CAPACITY;
            memcpy(&tfmt[1], begin + 1, format - begin);
            tfmt[format - begin + 1] = 0;
            begin = NULL;
            len = sprintf(&str->base[str->length], tfmt, dbl);
            if (len > 0) str->length += len;
            break;
            /* integer number formats */
        case 'o':
        case 'X':
        case 'x':
        case 'd':
        case 'i':
        case 'u':
            len = qualifier=='L'?32:16;
            len = MAX(width, len);
            if (!str_alter_capacity(str, str->length + len)) goto FAIL_CAPACITY;
            memcpy(&tfmt[1], begin + 1, format - begin);
            tfmt[format - begin + 1] = 0;
            begin = NULL;
            /* format conversion */
            if (qualifier == 'L')        len = sprintf(&str->base[str->length], tfmt, va_arg(args, long long));
            else if (qualifier == 'l')    len = sprintf(&str->base[str->length], tfmt, va_arg(args, int));
            else if (qualifier == 'h')    len = sprintf(&str->base[str->length], tfmt, (short)va_arg(args, int));
            else                        len = sprintf(&str->base[str->length], tfmt, (int)va_arg(args, int));
            if (len > 0) str->length += len;
            break;
        case '%':
            str_push_back(str, '%'); /* push % */
            begin = NULL;
            break;
        default:
            str_push_back(str, '%'); /* push % */
            begin = NULL;
            if (*format) str_push_back(str, *format);
            else format--;
            break;
        }
    }

    /* copy tail string to str */
    if (begin)
    {
        len = format - begin;
        if (!str_alter_capacity(str, str->length + len)) goto FAIL_CAPACITY;
        while (len--) str->base[str->length++] = *begin++;
    }

    if (!str_alter_capacity(str, str->length)) goto FAIL_CAPACITY;

    str->base[str->length] = '\0';

    va_end(args);

    return str;

FAIL_CAPACITY:
    str->base[str->length] = '\0';
    va_end(args);
    return NULL;
}
