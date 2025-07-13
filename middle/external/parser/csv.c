/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  csv.c
 *         \unit  csv
 *        \brief  This is a C language version of csv excel parser
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "csv.h"
#include <string.h>
#include <stdio.h>

/* dump buffer define */
typedef struct
{
    char* address;              /**< buffer base address */
    unsigned int size;          /**< size of buffer */
    unsigned int end;           /**< end of buffer used */
} BUFFER;

/* iterator define */
typedef struct
{
    void *p;                    /**< iteration pointer */
    unsigned int i;             /**< iteration index */
} ITERATOR;

/* smallest memory cell */
typedef struct CELL
{
    struct CELL *next;          /**< next cell */
    char* address;              /**< address of cell content */
} CELL;

/* row storage */
typedef struct ROW
{
    struct ROW *next;           /**< next column */
    CELL *cells;                /**< the cell base address of this column */
    unsigned int size;          /**< columns of current row */
    ITERATOR iterator;          /**< cell list iterator */
} ROW;

/* type of csv */
typedef struct CSV
{
    ROW *rows;                  /**< rows base */
    unsigned int size;          /**< rows of csv */
    ITERATOR iterator;          /**< rows list iterator */
} CSV;

/* parsing error message storage */
static int etype = 0;
static int eline = 0;
static int ecolumn = 0;
static const char* lbegin = NULL;

#define Tarray(a, r, c, i, j)   (*(char**)(((char *)a)+((i)*(c)+(j))*sizeof(void*)))
#define E(type) etype=(type)

/**
 *  \brief Duplicate a given string.
 * 
 *  \param[in] str String to be duplicated.
 *  \param[in] len Length of the string.
 *  \return Pointer to the duplicated string if successful, NULL otherwise.
 */
static char* csv_strdup(const char* str, int len)
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

static ROW *it_row(csv_t csv, int index, int size)
{
    if (index >= size) return NULL;
    if (index < csv->iterator.i || !csv->iterator.p || index == 0)
    {
        csv->iterator.i = 0;
        csv->iterator.p = csv->rows;
    }
    while (csv->iterator.p && csv->iterator.i < index)
    {
        csv->iterator.p = (void *)(((ROW *)(csv->iterator.p))->next);
        csv->iterator.i++;
    }
    return csv->iterator.p;
}

static CELL *it_cell(ROW *row, int index, int size)
{
    if (index >= size) return NULL;
    if (index < row->iterator.i || !row->iterator.p || index == 0)
    {
        row->iterator.i = 0;
        row->iterator.p = row->cells;
    }
    while (row->iterator.p && row->iterator.i < index)
    {
        row->iterator.p = (void *)(((CELL *)(row->iterator.p))->next);
        row->iterator.i++;
    }
    return row->iterator.p;
}

/**
 *  \brief Free the memory occupied by a linked list of CELL structures.
 * 
 *  \param[in] cell Pointer to the first CELL in the linked list.
 * 
 * This function frees the memory occupied by a linked list of CELL structures.
 * It iterates through the linked list, freeing each CELL structure and its associated memory.
 * 
 *  \note This function does not return a value.
 */
static void cell_free(CELL* cell)
{
    CELL* t;

    /* Iterate through the linked list, freeing each CELL structure */
    while (cell)
    {
        t = cell->next;

        /* Free the address string if it exists */
        if (cell->address) 
        {
            free(cell->address);
        }

        /* Free the CELL structure */
        free(cell);

        cell = t;
    }
}

/**
 *  \brief Create a linked list of CELL structures with the specified number of elements.
 * 
 *  \param[in] number Number of CELL structures to create in the linked list.
 *  \return Pointer to the first CELL in the linked list if successful, NULL otherwise.
 * 
 * This function creates a linked list of CELL structures with the specified number of elements.
 * Each CELL structure is allocated dynamically, with an empty address string and a NULL next pointer.
 * The function returns a pointer to the first CELL in the linked list if successful, and NULL otherwise.
 * 
 *  \note The caller is responsible for freeing the allocated memory.
 */
static CELL* cell_new(unsigned int number)
{
    CELL* cell = NULL, *t, *p = NULL;
    unsigned int i = 0;

    /* Create the specified number of CELL structures in the linked list */
    for (i = 0; i < number; i++)
    {
        t = (CELL*)malloc(sizeof(CELL));
        if (!t)
        {
            cell_free(cell);
            return NULL;
        }

        t->address = malloc(sizeof(char));
        if (!t->address)
        {
            cell_free(cell);
            return NULL;
        }

        t->next = NULL;
        t->address[0] = '\0';

        if (p)
        {
            p->next = t;
        }
        p = t;

        if (cell == NULL)
        {
            cell = t;
        }
    }

    return cell;
}

/**
 *  \brief Free the memory occupied by a linked list of ROW structures.
 * 
 *  \param[in] row Pointer to the first ROW in the linked list.
 * 
 * This function frees the memory occupied by a linked list of ROW structures.
 * It iterates through the linked list, freeing each ROW structure and its associated memory.
 * Additionally, it calls the `cell_free` function to free the memory occupied by the cells in each ROW structure.
 * 
 *  \note This function does not return a value.
 */
static void row_free(ROW *row)
{
    ROW* t;

    /* Iterate through the linked list, freeing each ROW structure */
    while (row)
    {
        t = row->next;

        /* Free the memory occupied by the cells in the ROW structure */
        cell_free(row->cells);

        /* Free the ROW structure */
        free(row);

        row = t;
    }
}

/**
 *  \brief Create a linked list of ROW structures with the specified number of elements.
 * 
 *  \param[in] number Number of ROW structures to create in the linked list.
 *  \return Pointer to the first ROW in the linked list if successful, NULL otherwise.
 * 
 * This function creates a linked list of ROW structures with the specified number of elements.
 * Each ROW structure is allocated dynamically, with the cells set to NULL, size set to 0, next pointer set to NULL, and iterator fields set to initial values.
 * The function returns a pointer to the first ROW in the linked list if successful, and NULL otherwise.
 * 
 *  \note The caller is responsible for freeing the allocated memory.
 */
static ROW *row_new(unsigned int number)
{
    ROW *row = NULL, *t, *p = NULL;
    unsigned int i = 0;

    /* Create the specified number of ROW structures in the linked list */
    for (i = 0; i < number; i++)
    {
        t = (ROW *)malloc(sizeof(ROW));
        if (!t)
        {
            row_free(row);
            return NULL;
        }

        t->cells = NULL;
        t->size = 0;
        t->next = NULL;
        t->iterator.p = NULL;
        t->iterator.i = 0;

        if (p)
        {
            p->next = t;
        }
        p = t;

        if (row == NULL)
        {
            row = t;
        }
    }

    return row;
}

/**
 *  \brief Minify the given row by removing empty cells at the end.
 *
 *  \param[in] row The row object to be minified
 */
static void row_minify(ROW *row)
{
    CELL *cell, *t = NULL;
    unsigned int i = 0;

    if (!row) return;
    if (!row->cells) return;

    cell = row->cells;
    while (cell)
    {
        /* Check if the cell contains a null string */
        if (!cell->address || cell->address[0] == '\0')
        {
            i++;  /* Count the number of consecutive null cells */
        }
        else  /* Not a null string */
        {
            t = cell;  /* Keep track of the last non-null cell */
            i = 0;  /* Reset the consecutive null cell count */
        }

        cell = cell->next;  /* Move to the next cell */
    }

    /* If there are consecutive null cells at the end, remove them */
    if (i > 0 && t)
    {
        cell_free(t->next);  /* Free the memory of the consecutive null cells */
        t->next = NULL;  /* Set the next pointer of the last non-null cell to NULL */
        row->size -= i;  /* Subtract the number of consecutive null cells from the row size */
    }
}

/**
 *  \brief Minify the given CSV table by removing empty rows at the end.
 *
 *  \param[in] csv handle
 */
void csv_minify(csv_t csv)
{
    ROW *row, *t = NULL;
    unsigned int i = 0;

    if (!csv) return;
    if (!csv->rows) return;

    row = csv->rows;
    while (row)
    {
        /* Minify the current row by removing empty cells */
        row_minify(row);

        /* Check if the row is empty (null row) */
        if (!row->cells || row->size == 0)
        {
            i++;  /* Count the number of consecutive null rows */
        }
        else  /* Not a null row */
        {
            t = row;  /* Keep track of the last non-null row */
            i = 0;   /* Reset the consecutive null row count */
        }

        row = row->next;  /* Move to the next row */
    }

    /* If there are consecutive null rows at the end, remove them */
    if (i > 0 && t)
    {
        row_free(t->next);  /* Free the memory of the consecutive null rows */
        t->next = NULL;  /* Set the next pointer of the last non-null row to NULL */
        csv->size -= i;  /* Subtract the number of consecutive null rows from the total size */
    }
}

/**
 *  \brief create a csv object
 *  \param[in] col: number of columns
 *  \param[in] row: number of rows
 *  \param[in] array: initialize CSV array, defined as `char *array[row][col] = {...}`, set null when passed in `NULL`
 *  \return csv handle or NULL FAIL
 */
csv_t csv_create(unsigned int row, unsigned int col, const void *array)
{
    csv_t csv = NULL;
    unsigned int i, j;
    char *text = NULL;

    /* create null csv */
    csv = (csv_t)malloc(sizeof(CSV));
    if (!csv) return NULL;
    csv->rows = NULL;
    csv->size = 0;
    csv->iterator.p = NULL;
    csv->iterator.i = 0;

    for (i = 0; i < row; i++)
    {
        if (array)
        {
            for (j = 0; j < col; j++)
            {
                text = Tarray(array, row, col, i, j);
                if (!csv_set_text(csv, i + 1, j + 1, text ? text : "")) goto FAIL;
            }
        }
        else  
        {
            if (!csv_set_text(csv, i + 1, col, "")) goto FAIL;
        }
    }

    return csv;
FAIL:
    csv_delete(csv);
    return NULL;
}

/**
 *  \brief delete the csv entity and its sub-entities.
 *  \param[in] csv: csv handle
 *  \return none
 */
void csv_delete(csv_t csv)
{
    if (csv)
    {
        row_free(csv->rows);
        free(csv);
    }
}

/**
 *  \brief Attach a row to a CSV table at the specified position.
 *
 *  \param[in] csv csv handle
 *  \param[in] pos The position where the row should be attached
 *  \param[in] row The row to be attached
 *
 *  \return Returns the number of rows attached to the table
 */
static int attach_row(csv_t csv, unsigned int pos, ROW *row)
{
    ROW *prev = NULL, *next = NULL;
    int count = 1;

    if (pos == 1)
    {
        next = csv->rows;
        csv->rows = row; 
    }
    else
    {
        prev = it_row(csv, pos - 2, csv->size);
        next = prev->next;
        prev->next = row;
    }

    while (row->next)
    {
        row = row->next;
        count++;
    }
    row->next = next;

    return count;
}

/**
 *  \brief Detach a row from a CSV table at the specified position.
 *
 *  \param[in] csv csv handle
 *  \param[in] pos The position of the row to be detached
 *
 *  \return Returns the detached row
 */
static ROW *detach_row(csv_t csv, unsigned int pos)
{
    ROW *row = NULL, *prev = NULL;

    /* Delete the row at the specified position */
    if (pos == 1)
    {
        /* Delete the first row */
        row = it_row(csv, 0, csv->size);
        csv->rows = row->next;
    }
    else
    {
        /* Delete a row other than the first row */
        prev = it_row(csv, pos - 2, csv->size);
        row = prev->next;
        prev->next = row->next;
    }

    /* Clear the next pointer of the detached row */
    row->next = NULL;

    return row;
}

/**
 *  \brief Insert a new row at the specified position in the CSV data.
 * 
 *  \param[in] csv csv handle
 *  \param[in] pos Position to insert the new row (counting from 1)
 *  \param[in] array initialize row array, defined as `char *array[size] = {...}`, set null when passed in `NULL`
 *  \param[in] count count of array
 *  \return 1 if the row is successfully inserted, 0 otherwise
 */
int csv_insert_row(csv_t csv, unsigned int pos, const char **array, unsigned int count)
{
    ROW *row = NULL;
    unsigned int i = 1, ins;
    unsigned int old = 0;

    /* Check if input parameter is valid */
    if (!csv) return 0;

    /* Adjust the position if it is 0 (insert at the end) */
    if (pos == 0)
    {
        pos = csv->size + 1;
    }
    
    old = csv->size;
    ins = pos;

    if (pos > csv->size + 1)
    {
        i = pos - csv->size;
        ins = csv->size + 1;
    }

    row = row_new(i);
    if (!row) return 0;

    csv->size += attach_row(csv, ins, row);

    /* Initialize insertion value */
    if (array && count)
    {
        for (i = 0; i < count; i++)
        {
            if (!csv_set_text(csv, pos, i + 1, array[i]))
            {
                csv_delete_row(csv, pos);
                for (; csv->size > old; )
                {
                    csv_delete_row(csv, old + 1);
                }
                return 0;
            }
        }
    }

    return 1;
}

/**
 *  \brief Delete the row at the specified position in the CSV data.
 * 
 *  \param[in] csv csv handle.
 *  \param[in] pos Position of the row to delete (counting from 1). If pos is 0, the last row will be deleted.
 *  \return 1 if the row is successfully deleted, 0 otherwise.
 */
int csv_delete_row(csv_t csv, unsigned int pos)
{
    ROW *row = NULL;

    /* Check if input parameter is valid */
    if (!csv) return 0;

    if (csv->size == 0) return 0;

    /* Adjust the position if it is 0 (delete the last row) */
    if (pos == 0) pos = csv->size;

    if (pos > csv->size) return 0;

    row = detach_row(csv, pos);

    row_free(row);

    csv->size--;

    return 1;
}

/**
 *  \brief Attach a cell to a row at the specified position.
 *
 *  \param[in] row The row object to attach the cell to
 *  \param[in] pos The position where the cell should be attached
 *  \param[in] cell The cell to be attached
 *
 *  \return Returns the number of cells attached to the row
 */
static int attach_cell(ROW *row, unsigned int pos, CELL *cell)
{
    CELL *prev = NULL, *next = NULL;
    int count = 1;

    if (!row) return 0;
    if (!cell) return 0;

    if (pos == 1)
    {
        next = row->cells;
        row->cells = cell; 
    }
    else
    {
        prev = it_cell(row, pos - 2, row->size);
        next = prev->next;
        prev->next = cell;
    }

    while (cell->next)
    {
        cell = cell->next;
        count++;
    }
    cell->next = next;

    return count;
}

/**
 *  \brief Detach a cell from a row at the specified position.
 *
 *  \param[in] row The row object to detach the cell from
 *  \param[in] pos The position of the cell to be detached
 *
 *  \return Returns the detached cell
 */
static CELL *detach_cell(ROW *row, unsigned int pos)
{
    CELL *cell = NULL, *prev = NULL;

    if (!row) return NULL;

    if (pos == 1)
    {
        cell = it_cell(row, 0, row->size);
        row->cells = cell->next;
    }
    else
    {
        prev = it_cell(row, pos - 2, row->size);
        cell = prev->next;
        prev->next = cell->next;
    }

    cell->next = NULL;

    return cell;
}

/**
 *  \brief Insert a new cell at the specified position in a row.
 * 
 *  \param[in] row Pointer to the row where the cell will be inserted.
 *  \param[in] pos Position to insert the new cell (counting from 1). If pos is 0, the new cell will be inserted at the end of the row.
 *  \return 1 if the cell is successfully inserted, 0 otherwise.
 * 
 * This function inserts a new cell at the specified position in a row. If pos is 0, the new cell will be inserted at the end of the row.
 * The function returns 1 if the cell is successfully inserted at the specified position, and 0 otherwise.
 * 
 *  \note The position number is counted from 1.
 *  \note It is the responsibility of the caller to ensure the validity of the row structure.
 *  \note The memory for the new cell is allocated dynamically using the `cell_new` function.
 */
static int row_insert_cell(ROW *row, unsigned int pos)
{
    CELL* cell = NULL;
    unsigned int i = 1, ins;

    /* Check if input parameter is valid */
    if (!row) return 0;

    /* Adjust the position if it is 0 (insert at the end) */
    if (pos == 0)
    {
        pos = row->size + 1;
    }

    ins = pos;

    if (pos > row->size + 1)
    {
        i = pos - row->size;
        ins = row->size + 1;
    }

    cell = cell_new(i);
    if (!cell) return 0;

    row->size += attach_cell(row, ins, cell);

    return 1;
}

/**
 *  \brief Delete the cell at the specified position in a row.
 * 
 *  \param[in] row Pointer to the row from which the cell will be deleted.
 *  \param[in] pos Position of the cell to delete (counting from 1). If pos is 0, the last cell will be deleted.
 *  \return 1 if the cell is successfully deleted, 0 otherwise.
 * 
 * This function deletes the cell at the specified position in a row. If pos is 0, the last cell in the row will be deleted.
 * The function returns 1 if the cell is successfully deleted, and 0 otherwise.
 * 
 *  \note The position number is counted from 1.
 *  \note It is the responsibility of the caller to ensure the validity of the row structure.
 *  \note The memory occupied by the deleted cell will be freed using the `cell_free` function.
 */
static int row_delete_cell(ROW* row, unsigned int pos)
{
    CELL* cell = NULL;

    /* Check if input parameter is valid */
    if (!row) return 0;

    /* Adjust the position if it is 0 (delete the last cell) */
    if (pos == 0) pos = row->size;

    if (pos > row->size) return 0;

    cell = detach_cell(row, pos);

    /* Free the memory occupied by the deleted cell */
    cell_free(cell);

    row->size--;

    return 1;
}

/**
 *  \brief Insert a new column at the specified position in the CSV data.
 * 
 *  \param[in] csv csv handle.
 *  \param[in] pos Position to insert the new column (counting from 1). If pos is 0, the new column will be inserted at the end of the CSV data.
 *  \param[in] array initialize column array, defined as `char *array[size] = {...}`, set null when passed in `NULL`
 *  \param[in] count count of array
 *  \return 1 if the column is successfully inserted, 0 otherwise.
 */
int csv_insert_col(csv_t csv, unsigned int pos, const char **array, unsigned int count)
{
    ROW *row = NULL;
    unsigned int i = 0, j = 0;
    unsigned int old;               /* original number of rows */

    /* Check if input parameter is valid */
    if (!csv) return 0;

    /* Adjust the position if it is 0 (insert at the end) */
    if (pos == 0)
    {
        pos = csv_col(csv) + 1;
    }

    old = csv->size;

    /* Insert the new column in each row of the CSV data */
    for (i = 1; i <= csv->size; i++)
    {
        row = it_row(csv, i - 1, csv->size);
        if (!row_insert_cell(row, pos))
        {
            goto FAIL;
        }
    }

    /* Initialize insertion value */
    if (array && count)
    {
        for (j = 0; j < count; j++)
        {
            if (!csv_set_text(csv, j + 1, pos, array[j]))
            {
                goto FAIL;
            }
        }
    }

    return 1;

FAIL:
    /* Rollback the column insertion if it fails */
    for (i--; i >= 1; i--)
    {
        row = it_row(csv, i - 1, csv->size);
        row_delete_cell(row, pos);
    }
    for (; csv->size > old; )
    {
        csv_delete_row(csv, old + 1);
    }
    return 0;
}

/**
 *  \brief Delete the column at the specified position in the CSV data.
 * 
 *  \param[in] csv csv handle.
 *  \param[in] pos Position of the column to delete (counting from 1). If pos is 0, the last column will be deleted.
 *  \return 1 if the column is successfully deleted, 0 otherwise.
 */
int csv_delete_col(csv_t csv, unsigned int pos)
{
    ROW *row = NULL;
    unsigned int i = 0;

    /* Check if input parameter is valid */
    if (!csv) return 0;

    /* Adjust the position if it is 0 (delete the last column) */
    if (pos == 0)
    {
        pos = csv_col(csv);
    }

    /* Delete the column from each row of the CSV data */
    for (i = 1; i <= csv->size; i++)
    {
        row = it_row(csv, i - 1, csv->size);
        row_delete_cell(row, pos);
    }

    return 1;
}

/**
 *  \brief Move a row to a new position within a CSV data structure.
 *
 *  \param[in] csv Pointer to the CSV data structure.
 *  \param[in] pos Current position of the row to be moved (counting from 1).
 *  \param[in] dest New position to move the row to (counting from 1).
 *  \return 1 if the row is successfully moved, 0 otherwise.
 *
 * This function moves a row within a CSV data structure from its current position to a new position.
 * If the current position (pos) is greater than the current size of the CSV data structure, the function checks if the new position (dest) is also greater than the current size. If so, it returns 1 indicating that nothing needs to be done.
 * If the new position is greater than the current size, an empty row is inserted at the new position using the `csv_insert_row` function.
 * The function then detaches the row from its current position using the `detach_row` function, updates the size of the CSV data structure, and attaches the row at the new position using the `attach_row` function.
 * The function returns 1 if the row is successfully moved, and 0 otherwise.
 *
 *  \note The caller is responsible for ensuring the validity of the CSV data structure and the indices of the current and new positions.
 */
int csv_move_row_to(csv_t csv, unsigned int pos, unsigned int dest)
{
    ROW *row = NULL;

    /* Check if the CSV data structure exists and is not empty */
    if (!csv) return 0;
    if (csv->size == 0) return 0;

    /* If the current position is greater than the size, check if the new position is also greater than the size */
    if (pos > csv->size)
    {
        if (dest > csv->size) return 1;
        else return csv_insert_row(csv, dest, NULL, 0);
    }

    /* If the new position is greater than the size, insert an empty row at the new position */
    if (dest > csv->size)
    {
        if (!csv_set_text(csv, dest, 0, "")) return 0;
    }

    /* Detach the row from the current position */
    row = detach_row(csv, pos);
    csv->size--;

    /* Attach the row at the new position */
    attach_row(csv, dest, row);
    csv->size++;

    return 1;
}

/**
 *  \brief Move a column to a new position within a CSV data structure.
 *
 *  \param[in] csv Pointer to the CSV data structure.
 *  \param[in] pos Current position of the column to be moved (counting from 1).
 *  \param[in] dest New position to move the column to (counting from 1).
 *  \return 1 if the column is successfully moved, 0 otherwise.
 *
 * This function moves a column within a CSV data structure from its current position to a new position.
 * If the current position (pos) is greater than the maximum column index in the CSV data structure, the function checks if the new position (dest) is also greater than the maximum column index. If so, it returns 1 indicating that nothing needs to be done.
 * If the new position is greater than the maximum column index, an empty column is inserted at the new position using the `csv_insert_col` function.
 * The function then iterates through each row in the CSV data structure, detaches the cell from the current position using the `detach_cell` function, updates the size of the row, and attaches the cell at the new position using the `attach_cell` function.
 * Finally, the function returns 1 if the column is successfully moved, and 0 otherwise.
 *
 *  \note The caller is responsible for ensuring the validity of the CSV data structure and the indices of the current and new positions.
 */
int csv_move_col_to(csv_t csv, unsigned int pos, unsigned int dest)
{
    ROW *row;
    CELL *cell;
    unsigned int max_col = 0;
    unsigned int i;

    /* Check if the CSV data structure exists and is not empty */
    if (!csv) return 0;
    if (csv->size == 0) return 0;

    /* Determine the maximum column index in the CSV data structure */
    max_col = csv_col(csv);

    /* If the current position is greater than the maximum column index, check if the new position is also greater than the maximum column index */
    if (pos > max_col)
    {
        if (dest > max_col) return 1;
        else return csv_insert_col(csv, dest, NULL, 0);
    }

    /* If the new position is greater than the maximum column index, insert an empty column at the new position */
    if (dest > max_col)
    {
        for (i = 1; i <= csv->size; i++)
        {
            if (!csv_set_text(csv, i, dest, "")) goto FAIL;
        }
    }

    /* Move the column to the new position */
    for (i = 1; i <= csv->size; i++)
    {
        row = it_row(csv, i - 1, csv->size);
        cell = detach_cell(row, pos);
        row->size--;
        attach_cell(row, dest, cell);
        row->size++;
    }

    return 1;

FAIL:
    /* Rollback the column insertion if an error occurs */
    if (i > 1)
    {
        for (; dest > max_col; dest--)
        {
            csv_delete_col(csv, max_col + 1);
        }
    }
    return 0;
}


/**
 *  \brief Copy a row from a CSV table to another position in the table.
 *
 *  \param[in] csv handle
 *  \param[in] pos The index of the row to be copied
 *  \param[in] dest The index where the row should be copied to
 *
 *  \return Returns 1 if the row is successfully copied, 0 otherwise
 */
int csv_copy_row_to(csv_t csv, unsigned int pos, unsigned int dest)
{
    unsigned int i;
    unsigned int size;

    if (!csv) return 0;
    if (csv->size == 0) return 0;

    if (pos == 0)
    {
        pos = csv->size;
    }

    /* Insert a new row at the destination position */
    csv_insert_row(csv, dest, NULL, 0);

    /* If the destination is before the source position, increment the source position by 1 */
    if (dest < pos) pos += 1;

    /* Get the size of the source row */
    size = it_row(csv, pos - 1, csv->size)->size;

    /* Copy each cell from the source row to the destination row */
    for (i = 1; i <= size; i++)
    {
        /* Set the text of the destination cell to the text of the source cell */
        if (!csv_set_text(csv, dest, i, csv_get_text(csv, pos, i)))
        {
            /* If setting text fails, delete the destination row and return 0 */
            csv_delete_row(csv, dest);
            return 0;
        }
    }

    return 1;
}

/**
 *  \brief Copy a column from a CSV table to another position in the table.
 *
 *  \param[in] csv handle
 *  \param[in] pos The index of the column to be copied
 *  \param[in] dest The index where the column should be copied to
 *
 *  \return Returns 1 if the column is successfully copied, 0 otherwise
 */
int csv_copy_col_to(csv_t csv, unsigned int pos, unsigned int dest)
{
    unsigned int i;
    unsigned int max_col = 0;
    const char *text;

    if (!csv) return 0;
    if (csv->size == 0) return 0;

    max_col = csv_col(csv);
    if (pos == 0)
    {
        pos = max_col;
    }

    /* Insert a new column at the destination position */
    csv_insert_col(csv, dest, NULL, 0);

    /* If the destination is before the source position, increment the source position by 1 */
    if (dest < pos) pos += 1;

    /* Copy each cell from the source column to the destination column */
    for (i = 1; i <= max_col; i++)
    {
        /* Get the text of the source cell */
        text = csv_get_text(csv, i, pos);

        /* If the text is available, set it to the destination cell */
        if (text && !csv_set_text(csv, i, dest, text))
        {
            /* If setting text fails, delete the destination column and return 0 */
            csv_delete_col(csv, dest);
            return 0;
        }
    }

    return 1;
}

/**
 *  \brief get the number of CSV rows
 *  \param[in] csv: csv handle
 *  \return number of rows
 */
unsigned int csv_row(csv_t csv)
{
    if (!csv) return 0;

    return csv->size;
}

/**
 *  \brief get the number of CSV columns
 *  \param[in] csv: csv handle
 *  \return number of columns
 */
unsigned int csv_col(csv_t csv)
{
    unsigned int col = 0;
    ROW *row;

    if (!csv) return 0;
    row = csv->rows;
    
    while (row)
    {
        if (row->size > col) col = row->size;
        
        row = row->next;
    }

    return col;
}

/**
 *  \brief Get the total number of non-empty cells in a CSV table.
 *
 *  \param[in] csv csv handle
 *
 *  \return Returns the number of non-empty cells in the table
 */
unsigned int csv_cell(csv_t csv)
{
    unsigned int count = 0;
    ROW *row;
    CELL *cell;

    if (!csv) return 0;

    row = csv->rows;
    while (row)
    {
        cell = row->cells;
        while (cell)
        {
            if (cell->address && cell->address[0])
            {
                count++;  /* Increment the count for each non-empty cell */
            }

            cell = cell->next;
        }

        row = row->next;
    }

    return count;
}

/**
 *  \brief get the number of CSV columns
 *  \param[in] csv: csv handle
 *  \param[in] row: row index, counting from 1
 *  \param[in] col: column index, counting from 1
 *  \param[in] text: cell text
 *  \return 1 success or 0 fail
 */
int csv_set_text(csv_t csv, unsigned int row, unsigned int col, const char* text)
{
    ROW *srow;      /* Pointer to the row */
    CELL *cell;     /* Pointer to the cell */
    char* s;        /* Buffer for copied text content */
    int len = 0;    /* Counter for the length of the text content */
    int i;

    /* Check if input parameters are valid */
    if (!csv) return 0;
    if (!text) return 0;
    if (col < 1) return 0;
    if (row < 1) return 0;

    /* If the specified row exceeds the size of the CSV data, insert a new row */
    if (row > csv->size)
    {
        if (!csv_insert_row(csv, row, NULL, 0)) return 0;
    }

    /* Get the specified row */
    srow = it_row(csv, row - 1, csv->size);
    if (!srow) return 0;

    /* If the specified column exceeds the size of the row, insert a new cell */
    if (col > srow->size)
    {
        if (!row_insert_cell(srow, col)) return 0;
    }

    /* Get the specified cell */
    cell = it_cell(srow, col - 1, srow->size);
    if (!cell) return 0;

    /* Calculate the length of the text content and check if it contains printable characters */
    for (i = 0;;i++)
    {
        if (text[i] == 0) break;
#if 0
        /* Not a printable character */
        if (!(text[i] >= ' ' && text[i] <= '~') && text[i] != '\n') return 0;
#endif 
        len++;
    }

    /* Copy the text content to a newly allocated memory space */
    s = csv_strdup(text, len);
    if (!s) return 0;

    /* Free the old address pointer and update it with the new address */
    if (cell->address) free(cell->address);
    cell->address = s;

    return 1;
}

/**
 *  \brief Get the text content at the specified row and column position in the CSV data.
 * 
 *  \param[in] csv csv handle.
 *  \param[in] row Row index, counting from 1.
 *  \param[in] col Column index, counting from 1.
 *  \return Pointer to the text content if found, NULL otherwise.
 */
const char* csv_get_text(csv_t csv, unsigned int row, unsigned int col)
{
    ROW* srow;      /* Pointer to the row */
    CELL* cell;     /* Pointer to the cell */

    /* Check if input parameter is valid */
    if (!csv) return NULL;

    /* Get the specified row */
    srow = it_row(csv, row - 1, csv->size);
    if (!srow) return NULL;

    /* Get the specified cell */
    cell = it_cell(srow, col - 1, srow->size);
    if (!cell) return NULL;

    return cell->address;
}

/**
 *  \brief Clean the text content at the specified position in a CSV data structure by setting it to an empty string.
 * 
 *  \param[in] csv Pointer to the CSV data structure.
 *  \param[in] row Row index (counting from 1).
 *  \param[in] col Column index (counting from 1).
 * 
 * This function cleans the text content at the specified position in a CSV data structure by setting it to an empty string.
 * If the specified position is invalid or if the text content is already empty, the function does nothing.
 * 
 *  \note The caller is responsible for ensuring the validity of the CSV data structure and the indices of the row and column.
 */
void csv_clean_text(csv_t csv, unsigned int row, unsigned int col)
{
    if (!csv) return;
    if (col < 1) return;
    if (row < 1) return;

    /* Check if the text content at the specified position is not empty */
    if (!csv_get_text(csv, row, col)) return;

    /* Set the text content at the specified position to an empty string */
    csv_set_text(csv, row, col, "");
}

/**
 *  \brief Calculate the smallest power of 2 that is greater than or equal to a given number.
 * 
 *  \param[in] x The given number.
 *  \return The smallest power of 2 greater than or equal to x.
 */
static unsigned int pow2gt(unsigned int x)
{
    int b = sizeof(int) * 8;
    int i = 1;

    --x;
    while (i < b)
    {
        x |= (x >> i);
        i <<= 1;
    }

    return x + 1;
}

/**
 *  \brief confirm whether buf still has the required capacity, otherwise add capacity.
 *  \param[in] buf: buf handle
 *  \param[in] needed: required capacity
 *  \return 1 success or 0 fail
 */
static int expansion(BUFFER* buf, unsigned int needed)
{
    char* address;
    unsigned int size;
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

/**
 *  \brief Print the content of a CELL structure to a BUFFER structure.
 * 
 *  \param[in] cell Pointer to the CELL structure containing the content to be printed.
 *  \param[in] buf Pointer to the BUFFER structure where the content will be stored.
 *  \return 1 if the content is successfully printed, 0 otherwise.
 * 
 * This function prints the content of a CELL structure to a BUFFER structure.
 * It iterates through the characters in the address string of the CELL structure and handles special characters, such as double quotes and newlines, by properly escaping them.
 * The function returns 1 if the content is successfully printed, and 0 otherwise.
 * 
 *  \note The caller is responsible for managing the BUFFER structure and ensuring its validity.
 */
static int print_cel(CELL* cell, BUFFER* buf)
{
    char* addr;
    char escape = 0;
    unsigned int size = 0;

    /* Check if the address string in the CELL structure is valid */
    if (!cell->address) return 1;

    /* Calculate the size of the content and determine if any special characters need to be escaped */
    for (addr = cell->address; *addr; addr++)
    {
        switch (*addr)
        {
        case '"':
            size++;
        case '\n':
        case ',':
            escape = 1;
            break;
        default:
            break;
        }
        
        size++;
    }
    if (escape) size += 2;

    /* Ensure that the BUFFER structure has enough space to store the content */
    if (!buf_append(size)) return 0;

    /* Print the content to the BUFFER structure, properly escaping special characters */
    if (escape) buf_putc('"');
    for (addr = cell->address; *addr; addr++)
    {
        switch (*addr)
        {
        case '"':
            buf_putc('"');
            break;
        default:
            break;
        }
        buf_putc(*addr);
    }
    if (escape) buf_putc('"');

    return 1;
}

/**
 *  \brief Print the contents of a CSV data structure to a BUFFER structure.
 * 
 *  \param[in] csv csv handle.
 *  \param[in] buf Pointer to the BUFFER structure where the contents will be stored.
 *  \return 1 if the contents are successfully printed, 0 otherwise.
 * 
 * This function prints the contents of a CSV data structure to a BUFFER structure.
 * It iterates through the rows and cells in the CSV data structure and uses the `print_cel` function to print each cell's content to the BUFFER structure.
 * The function returns 1 if the contents are successfully printed, and 0 otherwise.
 * 
 *  \note The caller is responsible for managing the CSV data structure and ensuring its validity.
 *  \note The caller is also responsible for managing the BUFFER structure and ensuring its validity.
 */
static int print_csv(csv_t csv, BUFFER* buf)
{
    ROW* row, *rnext;
    CELL* cell, *cnext;

    /* Check if the CSV data structure is valid */
    if (csv)
    {
        row = csv->rows;

        /* Iterate through the rows in the CSV data structure */
        while (row)
        {
            rnext = row->next;

            cell = row->cells;

            /* Iterate through the cells in each row */
            if (cell)
            {
                while (cell)
                {
                    cnext = cell->next;

                    /* Print the cell's content to the BUFFER structure */
                    print_cel(cell, buf);

                    cell = cnext;

                    /* Append a comma if there are more cells in the row */
                    if (cell)
                    {
                        if (!buf_append(1)) return 0;
                        buf_putc(',');
                    }
                }
            }

            row = rnext;

            /* Append a newline if there are more rows in the CSV data structure */
            if (row)
            {
                if (!buf_append(1)) return 0;
                buf_putc('\n');
            }
        }

        /* Null-terminate the BUFFER structure */
        buf_end() = '\0';
    }

    return 1;
}

/**
 *  \brief convert csv to text, using a buffered strategy.
 *  \param[in] csv: csv handle
 *  \param[out] *len: address that receives the length of printed characters
 *  \return address of converted text, free the char* when finished
 * 
 * This function generates a CSV string representation of a CSV data structure.
 * It uses the `print_csv` function to print the contents of the CSV data structure to a BUFFER structure, and then allocates memory to store the generated CSV string.
 * The function returns a pointer to the generated CSV string if successful, and NULL otherwise.
 * The length of the CSV string is stored in the `len` parameter if it is not NULL.
 * 
 *  \note The caller is responsible for freeing the allocated memory.
 */
char* csv_dumps(csv_t csv, int* len)
{
    BUFFER p;
    int preset = 1;

    /* Check if the CSV data structure is valid and not empty */
    if (!csv) return NULL;
    if (csv->size == 0) return NULL;

    /* Allocate memory for the initial BUFFER structure */
    p.address = (char*)malloc(preset);
    if (!p.address) return NULL;
    p.size = preset;
    p.end = 0;

    /* Print the CSV data to the BUFFER structure */
    if (!print_csv(csv, &p))
    {
        free(p.address);
        return NULL;
    }

    /* Store the length of the generated CSV string if the len parameter is provided */
    if (len) *len = p.end;

    return p.address;
}

/**
 *  \brief according to the csv, generate a file.
 *  \param[in] csv: csv handle
 *  \param[in] *filename: file name
 *  \return The length of the dumped CSV string if successful, or an error code if an error occurs.
 *          -1: csv handle null
 *          -2: filename null
 *          -3: dump string fail
 *          -4: create file fail 
 * 
 * This function dumps the contents of a CSV data structure to a file.
 * It calls the `csv_dumps` function to generate the CSV string representation of the CSV data structure.
 * The generated CSV string is then written to the specified file.
 * The function returns the length of the dumped CSV string if successful, or an error code if an error occurs.
 * 
 *  \note The caller is responsible for managing the CSV data structure and ensuring its validity.
 *  \note If an error occurs during file operations, the function may leave the file partially written.
 */
int csv_file_dump(csv_t csv, const char* filename)
{
    FILE* f;
    char* out;
    int len;

    /* Check if the CSV data structure and filename are valid */
    if (!csv) return -1;
    if (!filename) return -2;

    /* Generate the CSV string representation of the CSV data structure */
    out = csv_dumps(csv, &len);
    if (!out) return -3;

    /* Open the file for writing */
    f = fopen(filename, "w");
    if (!f)
    {
        free(out);
        return -4;
    }

    /* Write the CSV string to the file */
    fwrite(out, 1, len, f);
    fclose(f);

    /* Free the memory allocated for the CSV string */
    free(out);

    return len;
}

/**
 *  \brief load csv text and generate csv.
 *  \param[in] *text: address of text
 *  \return csv handler
 */
csv_t csv_loads(const char* text)
{
    csv_t csv;
    ROW* row = NULL;
    CELL* cell = NULL;
    const char* s = text;
    const char* start = s;
    const char* end = NULL;                 /* [start, end) is a cell text interval */
    const char* quotation = NULL;           /* (start, quotation) is a cell text escape interval */
    const char* t;
    int len = 0;
    
    if (!text) return NULL;

    /* Initialize static parameters for parsing */
    etype = CSV_E_OK;
    eline = 1;
    ecolumn = 0;
    lbegin = s;

    /* create a null csv */
    csv = csv_create(0, 0, NULL);
    if (!csv)
    {
        E(CSV_E_MEMORY);
        return NULL;
    }

    while (1)
    {
        switch (*s)
        {
        case '\0':
            end = s;
            if (*start == '"') quotation = s; /* At the end of the text, use the end as an escape closed interval */
        case '\n':
            eline++;
            lbegin = s + 1;
        case ',':
            if (*start == '"')
            {
                if (quotation) end = s; /* Leaving the escape interval, it can be concluded now */
            }
            else
            {
                end = s; /* Not within the escape interval, ending directly */
            }

            /* Parsing string has reached the end */
            if (end)
            {
#if defined(_WIN32) || defined(_WIN64)
                if (*end == '\n') end--; /* skip '\r' */
#endif 
                /* If there is currently no selected row, create a new row */
                if (!row)
                {
                    if (!csv_insert_row(csv, 0, NULL, 0))
                    {
                        E(CSV_E_MEMORY);
                        goto FAIL;
                    }
                    row = it_row(csv, csv->size - 1, csv->size);
                }
                
                /* Add a cell to store parsed text */
                if (!row_insert_cell(row, 0))
                {
                    E(CSV_E_MEMORY);
                    goto FAIL;
                }
                cell = it_cell(row, row->size - 1, row->size);

                /* The length of the string is end minus start, and then subtract the excess in the escape `"` */
                len = end - start - len;
                
                cell->address = malloc(len + 1);
                if (!cell->address)
                {
                    E(CSV_E_MEMORY);
                    goto FAIL;
                }
                
                /* Convert text to cells */
                len = 0;
                for (t = start; t < end; t++)
                {
                    /* Double quotation marks including escape */
                    if (*start == '"')
                    {
                        if (*t == '"')
                        {
                            if (t == start || t == quotation) continue; /* Skip both ends of quotation marks directly */
                            if (t < quotation) t++; /* Escaped quotation marks skip the first one */
                        }
                    }
                    cell->address[len++] = *t;
                }
                cell->address[len] = '\0';

                /* Reset to resolve the next one */
                if (*s == '\n') row = NULL;
                cell = NULL;
                start = s + 1;
                end = NULL;
                quotation = NULL;
                len = 0;
            }
            break;
        case '"':
            /* Starting with quotation marks, there is an escaped interval */
            if (*start == '"')
            {
                /* Currently at the starting position */
                if (s == start)
                {
                    len++;
                }
                else
                {
                    /* The quotation marks have not yet formed a closed interval, they are still within the scope of escape */
                    if (!quotation)
                    {
                        if (s[1] == '"') s++; /* it is an escaped quotation mark */
                        else quotation = s; /* A single quotation mark indicates the formation of a closed interval */
                        len++;
                    }
                }
            }
            break;
        default:
            break;
        }

        /* Exit of loop parsing */
        if (!*s) break;

        s++;
    }

    return csv;

FAIL:
    ecolumn = s - lbegin + 1;
    csv_delete(csv);
    return NULL;
}

/**
 *  \brief load csv file and generate csv.
 *  \param[in] *filename: filename
 *  \return csv handler
 */
csv_t csv_file_load(const char* filename)
{
    csv_t csv;
    FILE* f;
    long len;
    char* text = NULL;

    if (!filename) return NULL;

    eline = 0;

    /* open file and get the length of file */
    f = fopen(filename, "rb");
    if (!f)
    {
        E(CSV_E_OPEN);
        goto FAIL;
    }

    /* get file length */
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    /* read file */
    text = (char*)malloc(len + 1);
    if (text)
    {
        fread(text, 1, len, f);
        fclose(f);
        f = NULL;
    }
    else
    {
        E(CSV_E_MEMORY);
        goto FAIL;
    }
    text[len] = 0;

    csv = csv_loads(text); /* load text */
    if (!csv)
    {
        goto FAIL;
    }

    free(text);

    return csv;

FAIL:
    if (f) fclose(f);
    if (text) free(text);
    return NULL;
}

/**
 *  \brief obtain parsing error information.
 *  \param[out] *line: line number where the error occurred
 *  \param[out] *column: column number where the error occurred
 *  \return error type
 */
int csv_error_info(int* line, int* column)
{
    if (etype == CSV_E_OK) return 0;
    if (line) *line = eline;
    if (column) *column = ecolumn;
    return etype;
}

/**
 *  \brief Create a duplicate of a given csv handle.
 * 
 *  \param[in] csv csv handle.
 *  \return Pointer to the duplicated CSV data structure if successful, NULL otherwise.
 * 
 * This function creates a duplicate of a given CSV data structure. It creates a new CSV data structure, iterates through each row and cell in the original CSV data structure, and sets the corresponding text content in the duplicated CSV data structure using the `csv_set_text` and `csv_get_text` functions.
 * The function returns a pointer to the duplicated CSV data structure if successful, and NULL otherwise.
 * 
 *  \note The caller is responsible for freeing the allocated memory by calling the `csv_delete` function for the duplicated CSV data structure.
 *  \note If an error occurs during the duplication process, the function may leave the duplicated CSV data structure in an inconsistent state.
 */
csv_t csv_duplicate(csv_t csv)
{
    csv_t dp = NULL;
    ROW *row = NULL;
    unsigned int i, j;

    /* Check if the original CSV data structure is valid */
    if (!csv) return NULL;

    /* Create a new CSV data structure for the duplicate */
    dp = csv_create(0, 0, NULL);
    if (!dp) return NULL;

    /* Iterate through each row and cell in the original CSV data structure */
    for (i = 1; i <= csv->size; i++)
    {
        row = it_row(csv, i - 1, csv->size);

        for (j = 1; j <= row->size; j++)
        {
            /* Set the text content in the duplicated CSV data structure using the original CSV data structure */
            if (!csv_set_text(dp, i, j, csv_get_text(csv, i, j)))
            {
                csv_delete(dp);
                return NULL;
            }
        }
    }

    return dp;
}

/**
 *  \brief Copy the text content from a source cell to a destination cell in a CSV data structure.
 * 
 *  \param[in] csv csv handle.
 *  \param[in] s_row Source row index (counting from 1).
 *  \param[in] s_col Source column index (counting from 1).
 *  \param[in] d_row Destination row index (counting from 1).
 *  \param[in] d_col Destination column index (counting from 1).
 *  \return 1 if the text content is successfully copied, 0 otherwise.
 * 
 * This function copies the text content from a source cell in a CSV data structure to a destination cell.
 * It retrieves the text content from the source cell using the `csv_get_text` function and sets the same text content in the destination cell using the `csv_set_text` function.
 * The function returns 1 if the text content is successfully copied, and 0 otherwise.
 * 
 *  \note The caller is responsible for ensuring the validity of the CSV data structure and the indices of the source and destination cells.
 */
int csv_copy_cell_to(csv_t csv, unsigned int s_row, unsigned int s_col, unsigned int d_row, unsigned int d_col)
{
    const char* text = NULL;

    if (!csv) return 0;
    if (!s_row || !s_col || !d_row || !d_col) return 0;

    text = csv_get_text(csv, s_row, s_col);

    return csv_set_text(csv, d_row, d_col, text ? text : "");
}

/**
 *  \brief Cut the text content from a source cell and paste it into a destination cell in a CSV data structure.
 * 
 *  \param[in] csv csv handle.
 *  \param[in] s_row Source row index (counting from 1).
 *  \param[in] s_col Source column index (counting from 1).
 *  \param[in] d_row Destination row index (counting from 1).
 *  \param[in] d_col Destination column index (counting from 1).
 *  \return 1 if the text content is successfully cut and pasted, 0 otherwise.
 * 
 * This function cuts the text content from a source cell in a CSV data structure and pastes it into a destination cell.
 * It retrieves the text content from the source cell and sets it in the destination cell. Additionally, it swaps the addresses of the source and destination cells to complete the cut and paste operation.
 * The function returns 1 if the text content is successfully cut and pasted, and 0 otherwise.
 */
int csv_cut_cell_to(csv_t csv, unsigned int s_row, unsigned int s_col, unsigned int d_row, unsigned int d_col)
{
    ROW *row = NULL;
    CELL *cell = NULL, *d_cell = NULL;
    char *swap;

    if (!csv) return 0;
    if (!s_row || !s_col || !d_row || !d_col) return 0;

    if (!csv_set_text(csv, d_row, d_col, "")) return 0;
    row = it_row(csv, d_row - 1, csv->size);
    d_cell = it_cell(row, d_col - 1, row->size);

    row = it_row(csv, s_row - 1, csv->size);
    if (!row) return 1;

    cell = it_cell(row, s_col - 1, row->size);
    if (!cell) return 1;

    swap = cell->address;
    cell->address = d_cell->address;
    d_cell->address = swap;

    return 1;
}

/**
 *  \brief Insert a cell in the given CSV table.
 *
 *  \param[in] csv handle
 *  \param[in] row The index of the row where the cell is inserted
 *  \param[in] col The index of the column where the cell is inserted
 *  \param[in] move_down Whether to move other cells down
 *                      - 0: Do not move other cells down
 *                      - Non-zero: Move other cells right
 *
 *  \return Returns 1 if the cell is successfully inserted, 0 otherwise
 */
int csv_insert_cell(csv_t csv, unsigned int row, unsigned int col, int move_down)
{
    CELL *c1, *c2;
    ROW *r1, *r2;
    unsigned int i, size;
    char *text;

    if (!csv) return 0;
    if (csv->size == 0) return 0;
    if (!row || !col) return 0;
    if (row > csv->size) return 0;
    if (col > csv_col(csv)) return 0;

    if (move_down)
    {
        size = csv->size;
        /* Move other cells down from the given row to the end of the table */
        for (i = row; i <= size + 1; i++)
        {
            if (!csv_get_text(csv, i, col))
            {
                /* If the current cell is empty, set it to an empty string */
                if (!csv_set_text(csv, i, col, "")) return 0;
            }
        }
        
        /* Starting from the end, swap cell contents row by row and column by column to complete the move down operation */
        for (i = size; i >= row; i--)
        {
            r1 = it_row(csv, i - 1, csv->size);
            c1 = it_cell(r1, col - 1, r1->size);
            r2 = it_row(csv, i, csv->size);
            c2 = it_cell(r2, col - 1, r2->size);

            text = c1->address;
            c1->address = c2->address;
            c2->address = text;
        }
    }
    else 
    {
        /* If not moving other cells down, simply insert a new cell at the specified column in the given row */
        r1 = it_row(csv, row - 1, csv->size);
        if (col > r1->size) return 1;

        c1 = cell_new(1);
        if (!c1) return 0;
        attach_cell(r1, col, c1);
    }

    return 1;
}

/**
 *  \brief Delete a cell in the given CSV table.
 *
 *  \param[in] csv handle
 *  \param[in] row The index of the row where the cell is deleted
 *  \param[in] col The index of the column where the cell is deleted
 *  \param[in] move_up Whether to move other cells up
 *                    - 0: Do not move other cells up
 *                    - Non-zero: Move other cells left
 *
 *  \return Returns 1 if the cell is successfully deleted, 0 otherwise
 */
int csv_delete_cell(csv_t csv, unsigned int row, unsigned int col, int move_up)
{
    CELL *c1, *c2;
    ROW *r1, *r2;
    unsigned int i;
    char *text, *old;

    if (!csv) return 0;
    if (csv->size == 0) return 0;
    if (!row || !col) return 0;
    if (row > csv->size) return 0;
    if (col > csv_col(csv)) return 0;

    if (move_up)
    {
        /* Delete the cell and move cells up from the given row to the end of the table */
        for (i = row; i <= csv->size; i++)
        {
            if (!csv_get_text(csv, i, col))
            {
                /* If the current cell is empty, set it to an empty string */
                if (!csv_set_text(csv, i, col, "")) return 0;
            }
        }

        /* Set the cell at the specified row and column to an empty string */
        if (!csv_set_text(csv, row, col, "")) return 0;

        /* Starting from the given row, swap cell contents row by row and column by column to complete the move up operation */
        for (i = row; i < csv->size; i++)
        {
            r1 = it_row(csv, i - 1, csv->size);
            c1 = it_cell(r1, col - 1, r1->size);
            r2 = it_row(csv, i, csv->size);
            c2 = it_cell(r2, col - 1, r2->size);

            text = c1->address;
            c1->address = c2->address;
            c2->address = text;
        }
    }
    else  
    {
        /* If not moving other cells up, simply detach and free the cell at the specified column in the given row */
        r1 = it_row(csv, row - 1, csv->size);
        if (col > r1->size) return 1;

        c1 = detach_cell(r1, col);
        cell_free(c1);
    }

    return 1;
}

/**
 *  \brief Copy the text content from a CSV data structure to a 2D array.
 * 
 *  \param[in] csv Pointer to the CSV data structure.
 *  \param[in] o_row Starting row index in the CSV data structure (counting from 1).
 *  \param[in] o_col Starting column index in the CSV data structure (counting from 1).
 *  \param[out] array Pointer to the 2D array where the text content will be copied, defined as `char *array[row][col]`
 *  \param[in] row_size Number of rows in the 2D array.
 *  \param[in] col_size Number of columns in the 2D array.
 *  \return 1 if the text content is successfully copied, 0 otherwise.
 * 
 * This function copies the text content from a CSV data structure to a 2D array. It iterates through the specified range of rows and columns in the CSV data structure, retrieves the text content from each cell using the `csv_get_text` function, and copies it to the corresponding location in the 2D array.
 * The function returns 1 if the text content is successfully copied, and 0 otherwise.
 * 
 *  \note The caller is responsible for ensuring the validity of the CSV data structure and the indices of the starting row and column.
 *  \note The caller is also responsible for managing the memory of the 2D array.
 */
int csv_to_array(csv_t csv, unsigned int o_row, unsigned int o_col, void *array, unsigned int row_size, unsigned int col_size)
{
    const char *text = NULL;
    unsigned int i, j;

    /* Check if the CSV data structure exists */
    if (!csv) return 0;
    if (!array) return 0;
    if (!row_size || !col_size) return 0;

    /* Iterate through the specified range of rows and columns */
    for (i = 0; i < row_size; i++)
    {
        for (j = 0; j < col_size; j++)
        {
            /* Retrieve the text content from the CSV data structure */
            text = csv_get_text(csv, o_row + i, o_col + j);

            /* Copy the text content to the corresponding location in the 2D array */
            Tarray(array, row_size, col_size, i, j) = (char *)text;
        }
    }

    return 1;
}

/**
 *  \brief Compare two strings case-insensitively.
 *
 *  \param[in] s1 The first string to compare
 *  \param[in] s2 The second string to compare
 *
 *  \return Returns an integer less than, equal to, or greater than zero if s1 is found, respectively,
 * to be less than, to match, or be greater than s2.
 */
static int strccmp(const char* s1, const char* s2)
{
    if (!s1) return (s1 == s2) ? 0 : 1;
    if (!s2) return 1;

    for (; tolower(*s1) == tolower(*s2); ++s1, ++s2)
    {
        if (*s1 == 0) return 0;
    }

    return tolower(*(const unsigned char*)s1) - tolower(*(const unsigned char*)s2);
}

/**
 *  \brief Find the first occurrence of a substring within a string, case-insensitively.
 *
 *  \param[in] str The string to search within
 *  \param[in] substr The substring to search for
 *
 *  \return Returns a pointer to the first occurrence of the substring within the string,
 *         or NULL if the substring is not found.
 */
static char *strcstr(const char *str, const char *substr) 
{
    const char *p;
    const char *q;

    if (*substr == '\0') 
    {
        return (char *) str;
    }

    while (*str != '\0') 
    {
        p = str;
        q = substr;

        while (*p != '\0' && *q != '\0' && tolower(*p) == tolower(*q)) 
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

/**
 *  \brief Find a substring within a string, with matching options.
 *
 *  \param[in] s The string to search within
 *  \param[in] d The substring to search for
 *  \param[in] flag The matching options
 *
 *  \return Returns 1 if the substring is found, 0 otherwise
 */
static int strfind(const char *s, const char *d, int flag)
{
    if (flag & CSV_F_FLAG_MatchEntire)
    {
        if (flag & CSV_F_FLAG_MatchCase)
        {
            /* Compare the strings case-sensitively */
            if (!strcmp(s, d))
            {
                return 1;  /* Substring found */
            }
        }
        else 
        {
            /* Compare the strings case-insensitively */
            if (!strccmp(s, d))
            {
                return 1;  /* Substring found */
            }
        }
    }
    else  
    {
        if (flag & CSV_F_FLAG_MatchCase)
        {
            /* Check if the substring is present in the string case-sensitively */
            if (strstr(s, d))
            {
                return 1;  /* Substring found */
            }
        }
        else 
        {
            /* Check if the substring is present in the string case-insensitively */
            if (strcstr(s, d))
            {
                return 1;  /* Substring found */
            }
        }
    }
    return 0;  /* Substring not found */
}

/**
 *  \brief Find the first occurrence of a text within a CSV table.
 *
 *  \param[in] csv csv handle
 *  \param[in] text The text to search for
 *  \param[in] flag The matching options
 *  \param[out] row The index of the row where the text is found
 *  \param[out] col The index of the column where the text is found
 *
 *  \return Returns 1 if the text is found, 0 if the text is not found, or -1 if the search ends
 */
int csv_find(csv_t csv, const char* text, int flag, unsigned int* row, unsigned int* col)
{
    static csv_t s = NULL;
    static char *cmp = NULL;
    static unsigned int match_row = 0, match_col = 0, *x, *y;
    static unsigned int size_row = 0, size_col = 0, *size_x, *size_y;
    static int move_x, move_y;
    char *t;

    /* Check if the CSV data structure exists */
    if (!csv) return 0;
    if (!text) return 0;
    if (text[0] == '\0') return 0;
    if (!row || !col) return 0;

    /* Update lookup items */
    if (s != csv || cmp != text) 
    {
        s = csv;
        cmp = text;
        
        size_row = csv_row(csv);
        size_col = csv_col(csv);

        move_x = 1;
        move_y = 1;

        if (flag & CSV_F_FLAG_MatchByCol)
        {
            x = &match_col;
            y = &match_row;
            size_x = &size_col;
            size_y = &size_row;
        }
        else  
        {
            x = &match_row;
            y = &match_col;
            size_x = &size_row;
            size_y = &size_col;
        }

        if (flag & CSV_F_FLAG_MatchForward)
        {
            move_x = -move_x;
            move_y = -move_y;
            *x = *size_x;
            *y = *size_y;
        }
        else  
        {
            *x = 1;
            *y = 1;
        }
    }
    
    for (; 1 <= *x && *x <= *size_x; *x += move_x)
    {
        for (; 1 <= *y && *y <= *size_y; *y += move_y)
        {
            t = csv_get_text(csv, match_row, match_col);
            if (t)
            {
                if (strfind(t, cmp, flag))
                {
                    *row = match_row;
                    *col = match_col;

                    *y += move_y;

                    return 1;  /* Text found */
                }
            }
        }

        /* Update iteration position */
        *y = (flag & CSV_F_FLAG_MatchForward) ? *size_y : 1;
    }

    return -1;  /* Found end */
}

