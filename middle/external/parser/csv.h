/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  csv.h
 *         \unit  csv
 *        \brief  This is a C language version of csv excel parser
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __csv_H
#define __csv_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>

/* version infomation */

#define CSV_V_MAJOR                         1
#define CSV_V_MINOR                         0
#define CSV_V_PATCH                         0

/* csv type definition, hiding structural members, not for external use */

typedef struct CSV* csv_t;

/* error type */

#define CSV_E_OK                            (0) /* no error */
#define CSV_E_MEMORY                        (1) /* memory allocation failed */
#define CSV_E_OPEN                          (2) /* fail to open file */

/* find flag */

#define CSV_F_FLAG_MatchCase                (0x01) /* match case sensitive */
#define CSV_F_FLAG_MatchEntire              (0x02) /* match the entire cell content */
#define CSV_F_FLAG_MatchByCol               (0x04) /* match by column */
#define CSV_F_FLAG_MatchForward             (0x08) /* match from back to front */

/* load csv */

csv_t csv_loads(const char* text);
csv_t csv_file_load(const char* filename);

/* when loading fails, use this method to locate the error */

int csv_error_info(int* line, int* column);

/* dump csv */

char* csv_dumps(csv_t csv, int* len);
int csv_file_dump(csv_t csv, const char* filename);

/* create and delete csv */

csv_t csv_create(unsigned int row, unsigned int col, const void *array);
void csv_delete(csv_t csv);

/* conversion operation function */

csv_t csv_duplicate(csv_t csv);
int csv_to_array(csv_t csv, unsigned int o_row, unsigned int o_col, void *array, unsigned int row_size, unsigned int col_size);
void csv_minify(csv_t csv);

/* get the number of CSV rows, columns, cells */

unsigned int csv_row(csv_t csv);
unsigned int csv_col(csv_t csv);
unsigned int csv_cell(csv_t csv);

/* set and get CSV cell content */

int csv_set_text(csv_t csv, unsigned int row, unsigned int col, const char* text);
const char* csv_get_text(csv_t csv, unsigned int row, unsigned int col);
void csv_clean_text(csv_t csv, unsigned int row, unsigned int col);

/* row and column operations */

int csv_insert_row(csv_t csv, unsigned int pos, const char **array, unsigned int count);
int csv_insert_col(csv_t csv, unsigned int pos, const char **array, unsigned int count);
int csv_delete_row(csv_t csv, unsigned int pos);
int csv_delete_col(csv_t csv, unsigned int pos);
int csv_move_row_to(csv_t csv, unsigned int pos, unsigned int dest);
int csv_move_col_to(csv_t csv, unsigned int pos, unsigned int dest);
int csv_copy_row_to(csv_t csv, unsigned int pos, unsigned int dest);
int csv_copy_col_to(csv_t csv, unsigned int pos, unsigned int dest);

/* cell operations */

int csv_insert_cell(csv_t csv, unsigned int row, unsigned int col, int move_down);
int csv_delete_cell(csv_t csv, unsigned int row, unsigned int col, int move_up);
int csv_copy_cell_to(csv_t csv, unsigned int s_row, unsigned int s_col, unsigned int d_row, unsigned int d_col);
int csv_cut_cell_to(csv_t csv, unsigned int s_row, unsigned int s_col, unsigned int d_row, unsigned int d_col);

/* Find operation */

int csv_find(csv_t csv, const char* text, int flag, unsigned int* row, unsigned int* col);

/* Universal traversal method. */
#define csv_for_each(csv, row, col, text)                           \
for (                                                               \
    (row) = 1, (col) = 1;                                           \
    (csv) && (                                                      \
        (((text) = csv_get_text((csv), (row), (col))) == NULL) ?    \
            ((row) < csv_row(csv) ? 1 : 0) :                        \
            1                                                       \
    );                                                              \
    (text) ?                                                        \
        ((col)++) :                                                 \
        ((row)++, (col) = 1)                                        \
) if (text)                                                         \

#ifdef __cplusplus
}
#endif

#endif
