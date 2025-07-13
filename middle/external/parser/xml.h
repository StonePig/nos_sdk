/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  xml.h
 *         \unit  xml
 *        \brief  This is a C language version of xml parser
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __xml_H
#define __xml_H

#ifdef __cplusplus
extern "C"
{
#endif

/* version infomation */

#define XML_V_MAJOR                         1
#define XML_V_MINOR                         0
#define XML_V_PATCH                         0

/* xml type definition, hiding structural members, not for external use */

typedef struct XML *xml_t;

/* error type define */

#define XML_E_OK                            (0) /* ok */
#define XML_E_TEXT                          (1) /* empty text */
#define XML_E_MEMORY                        (2) /* memory */
#define XML_E_LABEL                         (3) /* label */
#define XML_E_VERSION                       (4) /* version */
#define XML_E_ENCODING                      (5) /* encoding */
#define XML_E_ILLEGAL                       (6) /* illegal character */
#define XML_E_END                           (7) /* end */
#define XML_E_VALUE                         (8) /* missing value */
#define XML_E_QUOTE                         (9) /* missing quete */
#define XML_E_COMMENT                       (10) /* missing comment tail --> */
#define XML_E_NOTES                         (11) /* head notes error */
#define XML_E_CDATA                         (12) /* missing comment tail ]]> */

/* Load xml */

xml_t xml_loads(const char* text);
xml_t xml_file_load(const char* filename);

/* When loading fails, use this method to locate the error */

int xml_error_info(int* line, int* column);

/* Dump xml */

char* xml_dumps(xml_t xml, int preset, int unformat, int* len);
int xml_file_dump(xml_t xml, char* filename);

/* Create and delete xml */

xml_t xml_create(const char* name);
void xml_delete(xml_t xml);

/* Add and remove XML attribute */

int xml_add_attribute(xml_t xml, const char *name, const char *value);
int xml_remove_attribute(xml_t xml, const char *name, int index);

/* Get XML attribute */

const char* xml_get_attribute(xml_t xml, const char *name, int index);

/* Insert and remove XML object */

int xml_insert(xml_t xml, int index, xml_t ins);
int xml_remove(xml_t xml, const char *name, int index);

/* Get child xml object */

xml_t xml_to(xml_t xml, const char *name, int index);

/* Set and get xml value text */

int xml_set_text(xml_t xml, const char *text);
const char* xml_get_text(xml_t xml);

#ifdef __cplusplus
}
#endif

#endif
