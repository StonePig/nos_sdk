/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  vstring.c
 *         \unit  vstring
 *        \brief  Similar to the C standard library string
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "vstring.h"
#include "vctype.h"
#include "vstdlib.h"

void *v_memcpy(void *dest, const void *src, size_t n) 
{
    char *dst = dest;
    const char *s = src;
    while (n--) 
    {
        *dst++ = *s++;
    }
    return dest;
}

void *v_mempcpy(void *dest, const void *src, size_t n)
{
    char *p1 = (char *)dest;
    const char *p2 = (const char *)src;
    size_t i;
    for (i = 0; i < n; i++)
    {
        p1[i] = p2[i];
    }
    return (void *)(p1 + i);
}

void *v_memmove(void *dest, const void *src, size_t n)
{
    char *dst = dest;
    const char *s = src;
    if (dst <= s) 
    {
        while (n--) 
        {
            *dst++ = *s++;
        }
    } 
    else 
    {
        dst += n;
        s += n;
        while (n--) 
        {
            *--dst = *--s;
        }
    }
    return dest;
}

void *v_memccpy(void *dest, const void *src, int c, size_t n) 
{
    char *dst = dest;
    const char *s = src;
    while (n--) 
    {
        *dst++ = *s;
        if (*s++ == c) 
        {
            return dst;
        }
    }
    return NULL;
}

void *v_memset(void *s, int c, size_t n) 
{
    char *p = s;
    while (n--) 
    {
        *p++ = (char) c;
    }
    return s;
}

int v_memcmp(const void *s1, const void *s2, size_t n) 
{
    const unsigned char *p1 = s1, *p2 = s2;
    for (size_t i = 0; i < n; ++i, ++p1, ++p2) 
    {
        if (*p1 != *p2) 
        {
            return (*p1 < *p2) ? -1 : 1;
        }
    }
    return 0;
}

void *v_memchr(const void *s, int c, size_t n) 
{
    const unsigned char *p = s;
    for (size_t i = 0; i < n; ++i, ++p) 
    {
        if (*p == (unsigned char) c) 
        {
            return (void *) p;
        }
    }
    return NULL;
}

void *v_memrchr(const void *s, int c, size_t n) 
{
    const unsigned char *p = (const unsigned char *) s + n - 1;
    for (; p >= (const unsigned char *) s; --p) 
    {
        if (*p == (unsigned char) c) 
        {
            return (void *) p;
        }
    }
    return NULL;
}

void *v_memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen)
{
    const char *p1 = (const char *)haystack;
    const char *p2 = (const char *)needle;
    size_t i;
    if (needlelen == 0)
    {
        return (void *)p1;
    }
    if (haystacklen < needlelen)
    {
        return NULL;
    }
    for (i = 0; i <= haystacklen - needlelen; i++)
    {
        if (p1[i] == *p2 && v_memcmp(p1 + i, p2, needlelen) == 0)
        {
            return (void *)(p1 + i);
        }
    }
    return NULL;
}

void *v_memfrob(void *s, size_t n)
{
    unsigned char *p = (unsigned char *)s;
    for (size_t i = 0; i < n; i++)
    {
        p[i] ^= 42;
    }
    return s;
}

char *v_strcpy(char *dest, const char *src) 
{
    char *p = dest;
    while (*src) 
    {
        *p++ = *src++;
    }
    *p = '\0';
    return dest;
}

char *v_strncpy(char *dest, const char *src, size_t n) 
{
    char *p = dest;
    size_t i = 0;
    while (i < n && *src) 
    {
        *p++ = *src++;
        ++i;
    }
    while (i < n) 
    {
        *p++ = '\0';
        ++i;
    }
    return dest;
}

size_t v_strlen(const char *s) 
{
    const char *p = s;
    while (*p) 
    {
        ++p;
    }
    return p - s;
}

size_t v_strnlen(const char *s, size_t n)
{
    const char *p = s;
    size_t i;
    for (i = 0; i < n && *p != '\0'; i++, p++);
    return i;
}

char *v_strcat(char *dest, const char *src) 
{
    char *p = dest + v_strlen(dest);
    while (*src) 
    {
        *p++ = *src++;
    }
    *p = '\0';
    return dest;
}

int v_strcmp(const char *s1, const char *s2) 
{
    while (*s1 && *s1 == *s2) 
    {
        ++s1;
        ++s2;
    }
    return *s1 - *s2;
}

int v_strncmp(const char *s1, const char *s2, size_t n) 
{
    size_t i = 0;
    while (i < n && *s1 && *s1 == *s2) 
    {
        ++s1;
        ++s2;
        ++i;
    }
    if (i == n) 
    {
        return 0;
    } 
    else 
    {
        return *s1 - *s2;
    }
}

static int v_strcoll_helper(char c1, char c2) 
{
    if (c1 < c2) 
    {
        return -1;
    } 
    else if (c1 > c2) 
    {
        return 1;
    } 
    else 
    {
        return 0;
    }
}

int v_strcoll(const char *s1, const char *s2)
{
    while (*s1 && *s1 == *s2) 
    {
        ++s1;
        ++s2;
    }
    return v_strcoll_helper(*s1, *s2);
}

size_t v_strxfrm(char *dest, const char *src, size_t n) 
{
    size_t i = 0;
    while (*src && i < n) 
    {
        *dest = *src;
        ++dest;
        ++src;
        ++i;
    }
    if (i == n) 
    {
        *dest = '\0';
        return n;
    } 
    else 
    {
        *dest = '\0';
        return i;
    }
}

char *v_strdup(const char *s) 
{
    size_t len = v_strlen(s) + 1;
    char *p = v_malloc(len);
    if (p == NULL) 
    {
        return NULL;
    }
    return (char *) v_memcpy(p, s, len);
}

char *v_strndup(const char *str, size_t n) 
{
    size_t len = v_strlen(str);
    if (n < len) 
    {
        len = n;
    }
    char *new_str = (char *) v_malloc(len + 1);
    if (new_str == NULL) {
        return NULL;
    }
    v_memcpy(new_str, str, len);
    new_str[len] = '\0';
    return new_str;
}

char *v_strchr(const char *str, int ch) 
{
    while (*str != '\0' && *str != ch) 
    {
        str++;
    }
    if (*str == ch) 
    {
        return (char *) str;
    } 
    else 
    {
        return NULL;
    }
}

char *v_strrchr(const char *str, int ch) 
{
    const char *last = NULL;
    while (*str != '\0') 
    {
        if (*str == ch) 
        {
            last = str;
        }
        str++;
    }
    if (*str == ch) 
    {
        return (char *) str;
    } 
    else 
    {
        return (char *) last;
    }
}

char *v_strchrnul(const char *str, int ch) 
{
    while (*str != '\0' && *str != ch) 
    {
        str++;
    }
    return (char *) str;
}

size_t v_strcspn(const char *str, const char *charset) 
{
    size_t len = 0;
    while (*str != '\0') 
    {
        const char *p = charset;
        while (*p != '\0') 
        {
            if (*str == *p) 
            {
                return len;
            }
            p++;
        }
        str++;
        len++;
    }
    return len;
}

char *v_strpbrk(const char *str, const char *charset)
{
    while (*str != '\0') 
    {
        const char *p = charset;
        while (*p != '\0') 
        {
            if (*str == *p) 
            {
                return (char *) str;
            }
            p++;
        }
        str++;
    }
    return NULL;
}

char *v_strstr(const char *str, const char *substr) 
{
    if (*substr == '\0') 
    {
        return (char *) str;
    }
    while (*str != '\0') 
    {
        const char *p = str;
        const char *q = substr;
        while (*p != '\0' && *q != '\0' && *p == *q) 
        {
            p++;
            q++;
        }
        if (*q == '\0') 
        {
            return (char *) str;
        }
        str++;
    }
    return NULL;
}

char *v_strrstr(const char *str, const char *substr) 
{
    size_t len = v_strlen(substr);
    if (len == 0) {
        return (char *) (str + v_strlen(str));
    }
    const char *end = str + v_strlen(str) - len;
    for (const char *p = end; p >= str; p--) 
    {
        if (v_strncmp(p, substr, len) == 0) 
        {
            return (char *) p;
        }
    }
    return NULL;
}

char *v_strtok(char *str, const char *delim)
{
    static char *last_str = NULL;
    static const char *last_delim = NULL;
    char *ret = NULL;
    if (str != NULL) 
    {
        last_str = str;
    }
    else if (last_str == NULL)
    {
        return NULL;
    }
    if (delim != NULL)
    {
        last_delim = delim;
    }
    else if (last_delim == NULL)
    {
        return NULL;
    }
    ret = last_str;
    while (*last_str != '\0')
    {
        const char *p = last_delim;
        while (*p != '\0' && *p != *last_str)
        {
            p++;
        }
        if (*p != '\0')
        {
            *last_str = '\0';
            last_str++;
            break;
        }
        last_str++;
    }
    return ret;
}

char *v_strtok_r(char *str, const char *delim, char **saveptr)
{
    char *last_str = NULL;
    const char *last_delim = NULL;
    char *ret = NULL;
    if (str != NULL)
    {
        last_str = str;
    }
    else if (*saveptr == NULL)
    {
        return NULL;
    }
    else
    {
        last_str = *saveptr;
    }
    if (delim != NULL)
    {
        last_delim = delim;
    }
    else if (*saveptr == NULL)
    {
        return NULL;
    }
    else
    {
        last_delim = *saveptr;
    }
    ret = last_str;
    while (*last_str != '\0')
    {
        const char *p = last_delim;
        while (*p != '\0' && *p != *last_str)
        {
            p++;
        }
        if (*p != '\0')
        {
            *last_str = '\0';
            last_str++;
            break;
        }
        last_str++;
    }
    *saveptr = last_str;
    return ret;
}

char *v_strcasestr(const char *haystack, const char *needle)
{
    const char *p1 = haystack;
    while (*p1 != '\0')
    {
        const char *p2 = needle;
        const char *p3 = p1;
        while (*p2 != '\0' && v_tolower(*p2) == v_tolower(*p3))
        {
            p2++;
            p3++;
        }
        if (*p2 == '\0')
        {
            return (char *)p1;
        }
        p1++;
    }
    return NULL;
}

char *v_strsep(char **stringp, const char *delim)
{
    char *p, *start;
    start = *stringp;
    p = (start != NULL) ? v_strpbrk(start, delim) : NULL;
    if (p == NULL) 
    {
        *stringp = NULL;
    }
    else
    {
        *p = '\0';
        *stringp = p + 1;
    }
    return start;
}

char *v_stpcpy(char *dest, const char *src)
{
    while ((*dest++ = *src++));
    return dest - 1;
}

char *v_stpncpy(char *dest, const char *src, size_t n)
{
    char *p = dest;
    while (n-- && *src)
    {
        *p++ = *src++;
    }
    while (n--)
    {
        *p++ = '\0';
    }
    return p;
}

int v_strverscmp(const char *s1, const char *s2)
{
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;
    int num1 = 0, num2 = 0;
    while (*p1 || *p2) 
    {
        while (v_isdigit(*p1)) 
        {
            num1 = num1 * 10 + (*p1 - '0');
            p1++;
        }
        while (v_isdigit(*p2))
        {
            num2 = num2 * 10 + (*p2 - '0');
            p2++;
        }
        if (num1 != num2)
        {
            return num1 > num2 ? 1 : -1;
        }
        if (*p1 != *p2)
        {
            return *p1 > *p2 ? 1 : -1;
        }
        if (*p1)
        {
            p1++;
        }
        if (*p2)
        {
            p2++;
        }
        num1 = num2 = 0;
    }
    return 0;
}


