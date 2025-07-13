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
#ifndef __ini_H
#define __ini_H

#ifdef __cplusplus
extern "C"
{
#endif

/* version infomation */

#define INI_V_MAJOR                         1
#define INI_V_MINOR                         0
#define INI_V_PATCH                         0

/* ini type definition, hiding structural members, not for external use */

typedef struct INI* ini_t;

/* error type define */

#define INI_E_OK                            (0) /* ok */
#define INI_E_BRACKETS                      (1) /* missing brackets ']' */
#define INI_E_DELIM                         (2) /* missing delimiter */
#define INI_E_KEY                           (3) /* missing key */
#define INI_E_SECTION                       (4) /* missing section */
#define INI_E_REKEY                         (5) /* key repeat */
#define INI_E_RESECTION                     (6) /* section repeat */
#define INI_E_MEMORY                        (7) /* memory allocation failed */
#define INI_E_OPEN                          (8) /* fail to open file */
#define INI_E_MAX                           (9) /*  */

/* load ini */

ini_t ini_loads(const char* text);
ini_t ini_file_load(const char* filename);

/* when loading fails, use this method to locate the error */

int ini_error_info(int* line, int* type);

/* dump ini */

char* ini_dumps(ini_t ini, int preset, int *len);
int ini_file_dump(ini_t ini, char* filename);

/* create and delete ini */

ini_t ini_create(void);
void ini_delete(ini_t ini);

/* section name and index are mutually located */

int ini_section_index(ini_t ini, const char* section);
const char* ini_section_name(ini_t ini, int index);

/* add and remove section */

int ini_add_section(ini_t ini, const char* section);
int ini_remove_section(ini_t ini, const char* section);

/* key name and index are mutually located */

int ini_key_index(ini_t ini, const char* section, const char* key);
const char* ini_key_name(ini_t ini, const char* section, int index);

/* set and get the value of the corresponding key in the specified section */

int ini_set_value(ini_t ini, const char* section, const char* key, const char* value);
const char* ini_get_value(ini_t ini, const char* section, const char* key);

/* remove key value pairs, there is no specific method to add key value pairs, but instead `ini_set_value()` */

int ini_remove_key(ini_t ini, const char* section, const char* key);

/* statistics section count and key value pair count under specified section */

int ini_section_count(ini_t ini);
int ini_pair_count(ini_t ini, const char* section);

#ifdef __cplusplus
}
#endif

#endif
