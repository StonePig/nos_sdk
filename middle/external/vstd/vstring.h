/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  vstring.h
 *         \unit  vstring
 *        \brief  Similar to the C standard library string
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __vstring_H
#define __vstring_H

#include "vstddef.h"

extern void *v_memcpy(void *dest, const void *src, size_t n);
extern void *v_mempcpy(void *dest, const void *src, size_t n);
extern void *v_memmove(void *dest, const void *src, size_t n);
extern void *v_memccpy(void *dest, const void *src, int c, size_t n);
extern void *v_memset(void *s, int c, size_t n);
extern int v_memcmp(const void *s1, const void *s2, size_t n);
extern void *v_memchr(const void *s, int c, size_t n);
extern void *v_memrchr(const void *s, int c, size_t n);
extern void *v_memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);
extern void *v_memfrob(void *s, size_t n);
extern char *v_strcpy(char *dest, const char *src);
extern char *v_strncpy(char *dest, const char *src, size_t n);
extern char *v_strcat(char *dest, const char *src);
extern size_t v_strlen(const char *s);
extern size_t v_strnlen(const char *s, size_t n);
extern char *v_strcat(char *dest, const char *src);
extern int v_strcmp(const char *s1, const char *s2);
extern int v_strncmp(const char *s1, const char *s2, size_t n);
extern int v_strcoll(const char *s1, const char *s2);
extern size_t v_strxfrm(char *dest, const char *src, size_t n);
extern char *v_strdup(const char *s);
extern char *v_strndup(const char *str, size_t n);
extern char *v_strchr(const char *str, int ch);
extern char *v_strrchr(const char *str, int ch);
extern char *v_strchrnul(const char *str, int ch);
extern size_t v_strcspn(const char *str, const char *charset);
extern char *v_strpbrk(const char *str, const char *charset);
extern char *v_strstr(const char *str, const char *substr);
extern char *v_strrstr(const char *str, const char *substr);
extern char *v_strtok(char *str, const char *delim);
extern char *v_strtok_r(char *str, const char *delim, char **saveptr);
extern char *v_strcasestr(const char *haystack, const char *needle);
extern char *v_strsep(char **stringp, const char *delim);
extern char *v_stpcpy(char *dest, const char *src);
extern char *v_stpncpy(char *dest, const char *src, size_t n);
extern int v_strverscmp(const char *s1, const char *s2);

#endif
