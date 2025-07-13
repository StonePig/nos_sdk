/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  txls.h
 *         \unit  txls
 *        \brief  This is a C language version of text excel parser
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __txls_H
#define __txls_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>

/* version infomation */

#define TXLS_V_MAJOR                        1
#define TXLS_V_MINOR                        0
#define TXLS_V_PATCH                        0

/* txls type definition, hiding structural members, not for external use */

typedef struct TXLS *txls_t;

/* error type define */

#define TXLS_E_OK                           (0) /* no error */
#define TXLS_E_HEAD                         (1) /* irregular header format */
#define TXLS_E_ALLOC                        (2) /* failed to allocate space */
#define TXLS_E_BEGIN                        (3) /* missing "|" separator at the begin */
#define TXLS_E_END                          (4) /* missing "|" separator at the end */
#define TXLS_E_IDENT                        (5) /* missing "-" separator at split row */
#define TXLS_E_BRANK                        (6) /* there are extra blank lines */
#define TXLS_E_MEMORY                       (7) /* memory allocation failed */
#define TXLS_E_OPEN                         (8) /* fail to open file */

/* cell alignment */

#define TXLS_ALIGN_UNKNOW                   (0)
#define TXLS_ALIGN_LEFT                     (1)
#define TXLS_ALIGN_RIGHT                    (2)
#define TXLS_ALIGN_CENTER                   (3)

/* Load txls */

txls_t txls_loads(const char* text);
txls_t txls_file_load(const char* filename);

/* When loading fails, use this method to locate the error */

int txls_error_info(int* line);

/* Dump txls */

char* txls_dumps(txls_t txls, int neat, int* len);
int txls_file_dump(txls_t txls, const char* filename);

/* Create and delete txls */

txls_t txls_create(unsigned int col, unsigned int row);
void txls_delete(txls_t txls);

/* Get the number of txls rows and columns*/

unsigned int txls_row(txls_t txls);
unsigned int txls_col(txls_t txls);

/* set and get txls cell text */

const char* txls_get_text(txls_t txls, unsigned int col, unsigned int row);
int txls_set_text(txls_t txls, unsigned int col, unsigned int row, const char* text);

/* row and column operations */

int txls_insert_column(txls_t txls, unsigned int col);
int txls_delete_column(txls_t txls, unsigned int col);
int txls_insert_row(txls_t txls, unsigned int row);
int txls_delete_row(txls_t txls, unsigned int row);

/* set column alignment */

int txls_set_align(txls_t txls, unsigned int col, int align);

/* Set and get header content */

#define txls_set_head(txls, col, head)      txls_set_text((txls), (col), 0, (head))
#define txls_get_head(txls, col)            txls_get_text((txls), (col), 0)

#ifdef __cplusplus
}
#endif

#endif
