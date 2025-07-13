/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  txls.c
 *         \unit  txls
 *        \brief  This is a C language version of text excel parser
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "txls.h"
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

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

/* column storage */
typedef struct COLUMN
{
    struct COLUMN *next;        /**< next column */
    CELL *cells;                /**< the cell base address of this column */
    ITERATOR iterator;          /**< cell list iterator */
    int align;                  /**< alignment */
    unsigned int width;         /**< the output width of this column when neatly outputting */
} COLUMN;

/* type of txls */
typedef struct TXLS
{
    COLUMN *columns;            /**< columns base */
    ITERATOR iterator;          /**< column iterator */
    unsigned int col;           /**< column count */
    unsigned int row;           /**< row count */
} TXLS;

/* parsing error message storage */
static int etype = 0;
static int eline = 0;

#define E(type) etype=(type)

static COLUMN *txls_column(txls_t txls, int index, int size)
{
    if (index >= size) return NULL;
    if (index < txls->iterator.i || !txls->iterator.p || index == 0)
    {
        txls->iterator.i = 0;
        txls->iterator.p = txls->columns;
    }
    while (txls->iterator.p && txls->iterator.i < index)
    {
        txls->iterator.p = ((COLUMN *)(txls->iterator.p))->next;
        txls->iterator.i++;
    }
    return txls->iterator.p;
}

static CELL *column_cell(COLUMN *column, int index, int size)
{
    if (index >= size) return NULL;
    if (index < column->iterator.i || !column->iterator.p || index == 0)
    {
        column->iterator.i = 0;
        column->iterator.p = column->cells;
    }
    while (column->iterator.p && column->iterator.i < index)
    {
        column->iterator.p = ((CELL *)(column->iterator.p))->next;
        column->iterator.i++;
    }
    return column->iterator.p;
}

/**
 *  \brief create a txls object
 *  \param[in] col: number of columns
 *  \param[in] row: number of rows
 *  \return txls handle or NULL FAIL
 */
txls_t txls_create(unsigned int col, unsigned int row)
{
    txls_t txls = NULL;
    unsigned int i;

    /* create null txls and initialize */
    txls = (txls_t)malloc(sizeof(TXLS));
    if (!txls) return NULL;
    txls->columns = NULL;
    txls->col = 0;
    txls->row = 0;

    /* Insert Column */
    for (i = 0; i < col; i++)
    {
        if (!txls_insert_column(txls, 1)) goto FAIL;
    }

    /* Insert Row */
    for (i = 0; i < row; i++)
    {
        if (!txls_insert_row(txls, 1)) goto FAIL;
    }

    txls->col = col;
    txls->row = row;

    return txls;

FAIL:
    txls_delete(txls);
    return NULL;
}

/**
 * \brief Create a new CELL structure and allocate memory for it
 *
 * \param[in] len Length of the address string to be allocated
 * \return A pointer to the newly created CELL structure, or NULL if the allocation fails
 */
static CELL* new_cell(int len)
{
    CELL *cell;
    
    /* If the length is negative, return NULL */
    if (len < 0) return NULL; 
    
    /* Allocate memory for the CELL structure */
    cell = (CELL *)malloc(sizeof(CELL));  
    if (!cell) return NULL;
    
    /* Allocate memory for the address string */
    cell->address = malloc(len + 1);
    if (!cell->address)
    {
        free(cell);
        return NULL;
    }
    
    /* Add a null terminator to the end of the address string */
    cell->address[len] = 0; 
    cell->next = NULL;
    
    return cell;
}

/**
 * \brief Free the memory allocated for a CELL structure
 *
 * \param[in] cell Pointer to the CELL structure to be freed
 * \return void
 */
static void free_cell(CELL* cell)
{
    if (!cell) return;
    
    /* If the address string is allocated, free the memory */
    if (cell->address) free(cell->address); 

    /* Free the memory allocated for the CELL structure */
    free(cell); 
}

/**
 * \brief Free the memory allocated for a COLUMN structure and its associated CELL structures
 *
 * \param[in] column Pointer to the COLUMN structure to be freed
 * \return void
 */
static void free_column(COLUMN *column)
{
    CELL *cell = NULL;

    /* Check if the column pointer is not NULL */
    if (column)  
    {
        /* Loop through the cells in the column */
        while (column->cells)  
        {
            /* Store the next cell pointer */
            cell = column->cells->next; 

            /* Free the memory allocated for the current cell */
            free_cell(column->cells);  

            /* Move to the next cell */
            column->cells = cell;  
        }

        /* Free the memory allocated for the COLUMN structure */
        free(column);  
    }
}

/**
 *  \brief delete txls object
 *  \param[in] txls: txls handle
 *  \return none
 */
void txls_delete(txls_t txls)
{
    if (!txls) return;

    /* Loop through all columns in the txls_t structure */
    while (txls->col) 
    {
        /* Delete the first column of the txls_t structure */
        txls_delete_column(txls, 1); 
    }

    /* Free the memory allocated for the txls_t structure */
    free(txls); 
}

/**
 *  \brief get the number of txls columns
 *  \param[in] txls: txls handle
 *  \return number of txls columns
 */ 
unsigned int txls_col(txls_t txls)
{
    if (!txls) return 0;
    return txls->col;
}

/**
 *  \brief get the number of txls rows
 *  \param[in] txls: txls handle
 *  \return number of txls rows
 */
unsigned int txls_row(txls_t txls)
{
    if (!txls) return 0;
    return txls->row;
}

/**
 *  \brief Duplicate a given string.
 * 
 *  \param[in] str String to be duplicated.
 *  \param[in] len Length of the string.
 *  \return Pointer to the duplicated string if successful, NULL otherwise.
 */
static char* txls_strdup(const char* str, int len)
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

/**
 * \brief Calculate the size of a string considering special characters
 *
 * \param[in] s Pointer to the input string
 * \return The calculated size of the string taking special characters into account
 */
static unsigned int tsize(char *s)
{
    unsigned int size = 0; /* Initialize the size counter */

    /* Loop through the string until the end */
    while (s && *s++) 
    {
        /* If the current character is a newline, increase the size by 3 */
        if (*s == '\n') size += 3; 
        /* If the current character is a '|', increase the size by 1 */
        else if (*s == '|') size++; 
        
        /* Increase the size for every character */
        size++; 
    }

    return size;
}

/**
 * \brief Update the width of a column based on the content of its cells
 *
 * \param[in] column Pointer to the COLUMN structure to be updated
 * \param[in] cell Pointer to the CELL structure to be excluded from width calculation
 * \param[in] row The row index for which the width is being updated
 * \return void
 */
static void update_width(COLUMN *column, CELL *cell, unsigned int row)
{
    CELL *c = NULL;
    unsigned int i;
    unsigned int size = 0;

    /* Reset the column width to 0 */
    column->width = 0;

    /* Iterate through the rows up to the specified row index */
    for (i = 0; i <= row; i++) 
    {
        /* Get the cell at the specified row index */
        c = column_cell(column, i, row + 1);

        /* Calculate the size of the cell's address */
        size = tsize(c->address);

        /* Update the column width if the size is greater */
        if (c != cell && size >= column->width) column->width = size; 
    }
}

/**
 *  \brief set the alignment of the column
 *  \param[in] txls: txls handle
 *  \param[in] col: the column, starting from 1
 *  \param[in] align: alignment, TXLS_ALIGN_LEFT, TXLS_ALIGN_RIGHT, TXLS_ALIGN_CENTER
 *  \return 1 success or 0 fail
 */
int txls_set_align(txls_t txls, unsigned int col, int align)
{
    COLUMN *column;
    
    if (!txls) return 0;
    
    /* Get the specified column from the txls_t structure */
    column = txls_column(txls, col - 1, txls->col); 
    if (!column) return 0;
    
    /* Check the alignment value */
    switch (align) 
    {
    case TXLS_ALIGN_RIGHT:
    case TXLS_ALIGN_CENTER:
    case TXLS_ALIGN_LEFT:
        column->align = align; /* Set the column's alignment */
        break;
    default:
        column->align = TXLS_ALIGN_UNKNOW; /* Set the column's alignment to unknown if the value is not recognized */
        break;
    }
    
    return 1;
}

/**
 *  \brief get the content of the specified cell
 *  \param[in] txls: txls handle
 *  \param[in] col: the column, starting from 1
 *  \param[in] row: the row, starting from 1
 *  \return conten address or NULL fail
 */
const char* txls_get_text(txls_t txls, unsigned int col, unsigned int row)
{
    COLUMN *column;
    CELL *cell;
    
    if (!txls) return NULL;
    
    /**< If the column and row index is out of range, return NULL */
    if (col < 1 || col > txls->col) return NULL; 
    if (row > txls->row) return NULL;
    
    /* Get the specified column from the txls_t structure */
    column = txls_column(txls, col - 1, txls->col); 

    /* Get the cell at the specified row index */
    cell = column_cell(column, row, txls->row + 1); 
    
    return cell ? cell->address : NULL;
}

/**
 *  \brief set the content of the specified cell
            passing in other invisible characters will cause the setting to fail
 *  \param[in] txls: txls handle
 *  \param[in] col: the column, starting from 1
 *  \param[in] row: the row, starting from 1
 *  \param[in] *text: specified content
 *  \return 1 success or 0 fail
 */
int txls_set_text(txls_t txls, unsigned int col, unsigned int row, const char* text)
{
    COLUMN *column;
    CELL *cell;
    char* s;
    int len = 0;
    int i;
    
    if (!txls) return 0;
    if (!text) return 0;

    /**< If the column and row index is out of range, return 0 */
    if (col < 1 || col > txls->col) return 0;
    if (row > txls->row) return 0;

    /* Get the specified column from the txls_t structure */
    column = txls_column(txls, col - 1, txls->col);

    /* Get the cell at the specified row index */
    cell = column_cell(column, row, txls->row + 1);

    /* Loop through the text to calculate its length */
    for (i = 0;;i++)
    {
        if (text[i] == 0) break;
        // if (!(text[i] >= ' ' && text[i] <= '~') && text[i] != '\n') return 0; /* Not a printable character */
        len++;
    }

    /* Copy the text content to a new memory location */
    s = txls_strdup(text, len);
    if (!s) return 0;

    /* Free the memory occupied by the original cell content */
    if (cell->address) free(cell->address);

    /* Set the cell's address to the copied text */
    cell->address = s;

    return 1;
}

/**
 *  \brief insert a column into txls
 *  \param[in] txls: txls handle
 *  \param[in] col: the column, starting from 1
 *  \param[in] *head: header
 *  \return 1 success or 0 fail
 */
int txls_insert_column(txls_t txls, unsigned int col)
{
    COLUMN *column = NULL, *pcolumn = NULL;
    CELL *cell = NULL, *pcell = NULL;
    unsigned int i;

    if (!txls) return 0;

    /* If the column index is out of range, return 0 */
    if (col > txls->col + 1 || col < 1) return 0; 

    /* Allocate memory for the new column */
    column = (COLUMN *)malloc(sizeof(COLUMN)); 
    if (!column) goto FAIL;

    /**< Initialize the new column */
    column->align = TXLS_ALIGN_UNKNOW;
    column->cells = NULL; 

    /* Loop through the rows to create new cells for the new column */
    for (i = 0; i <= txls->row; i++) 
    {
        /* Create a new cell */
        cell = new_cell(0); 
        if (!cell) goto FAIL;

        /* Set the first cell of the new column */
        if (!column->cells) column->cells = cell; 

        /* Set the next pointer of the previous cell to the new cell */
        if (pcell) pcell->next = cell;

        /* Update the previous cell pointer */
        pcell = cell; 
    }

    /* If the new column is to be inserted at the beginning */
    if (col == 1)
    {
        /* Set the next pointer of the new column to the current first column */
        column->next = txls->columns;
        /* Update the txls_t structure to point to the new first column */
        txls->columns = column; 
    }
    /* If the new column is to be inserted in the middle or at the end */
    else
    {
        /* Get the column before the specified position */
        pcolumn = txls_column(txls, col - 2, txls->col);

        /* Set the next pointer of the new column to the next column of the previous column */
        column->next = pcolumn->next; 
        /* Update the next pointer of the previous column to the new column */
        pcolumn->next = column; 
    }

    /* Increment the total column count in the txls_t structure */
    txls->col++; 

    return 1;

FAIL:
    /* Free the memory allocated for the new column in case of failure */
    if (column) free_column(column);
    return 0;
}

/**
 *  \brief delete a column from txls
 *  \param[in] txls: txls handle
 *  \param[in] col: the column, starting from 1
 *  \return 1 success or 0 fail
 */
int txls_delete_column(txls_t txls, unsigned int col)
{
    COLUMN *column = NULL, *pcolumn = NULL;

    if (!txls) return 0;

    /* If the column index is out of range, return 0 */
    if (col > txls->col || col < 1) return 0;

    /* If the first column is to be deleted */
    if (col == 1) 
    {
        /* Get the first column from the txls_t structure */
        column = txls_column(txls, 0, txls->col);

        /* Update the txls_t structure to point to the next column as the new first column */
        txls->columns = column->next; 
    }
    /* If a column in the middle or at the end is to be deleted */
    else 
    {
        /* Get the column before the specified position */
        pcolumn = txls_column(txls, col - 2, txls->col);

        /* Get the column to be deleted */
        column = pcolumn->next;
        /* Update the next pointer of the previous column to skip the column to be deleted */ 
        pcolumn->next = column->next; 
    }
    
    /* Free the memory allocated for the deleted column */
    free_column(column); 

    /* Decrement the total column count in the txls_t structure */
    txls->col--; 

    return 1;
}

/**
 *  \brief insert a row to txls
 *  \param[in] txls: txls handle
 *  \param[in] row: the row, starting from 1
 *  \return 1 success or 0 fail
 */
int txls_insert_row(txls_t txls, unsigned int row)
{
    COLUMN *column = NULL;
    CELL *cell = NULL, *pcell = NULL;
    unsigned int i;

    if (!txls) return 0;

    /* If the row index is out of range, return 0 */
    if (row > txls->row + 1 || row < 1) return 0;

    /* Loop through the columns to create new cells for the new row */
    for (i = 0; i < txls->col; i++)
    {
        /* Create a new cell */
        cell = new_cell(0); 
        if (!cell) goto FAIL;
        
        /* Get the current column */
        column = txls_column(txls, i, txls->col);

        /* Get the cell before the specified position */
        pcell = column_cell(column, row - 1, txls->row + 1); 

        /* Set the next pointer of the new cell to the cell at the specified position */
        cell->next = pcell->next; 
        /* Update the next pointer of the previous cell to the new cell */
        pcell->next = cell; 
    }

    /* Increment the total row count in the txls_t structure */
    txls->row++; 

    return 1;

FAIL:
    /* In case of failure, revert the changes and free the memory */
    while (i--) 
    {
        /* Get the current column */
        column = txls_column(txls, i, txls->col); 

        /* Get the cell before the specified position */
        pcell = column_cell(column, row - 1, txls->row + 1); 

        /* Get the cell to be deleted */
        cell = pcell->next; 
        /* Update the next pointer of the previous cell to skip the cell to be deleted */
        pcell->next = cell->next; 

        /* Free the memory occupied by the deleted cell */
        free(cell); 
    }

    return 0;
}

/**
 *  \brief delete a row from txls
 *  \param[in] txls: txls handle
 *  \param[in] row: the row, starting from 1
 *  \return 1 success or 0 fail
 */
int txls_delete_row(txls_t txls, unsigned int row)
{
    COLUMN *column = NULL;
    CELL *cell = NULL, *pcell = NULL;
    unsigned int i;

    if (!txls) return 0;

    /* If the row index is out of range, return 0 */
    if (row > txls->row || row < 1) return 0; 

    /* Loop through the columns to delete the cells of the specified row */
    for (i = 0; i < txls->col; i++) 
    {
        /* Get the current column */
        column = txls_column(txls, i, txls->col); 

        /* Get the cell before the specified position */
        pcell = column_cell(column, row - 1, txls->row + 1);

        /* Get the cell to be deleted */
        cell = pcell->next; 
        /* Update the next pointer of the previous cell to skip the cell to be deleted */
        pcell->next = cell->next; 

        /* Free the memory occupied by the deleted cell */
        free_cell(cell);
    }

    /* Decrement the total row count in the txls_t structure */
    txls->row--; 

    return 1;
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
 * \brief Print the content of a cell with specified width and alignment into a buffer
 *
 * \param[in] cell Pointer to the CELL structure containing the cell content
 * \param[in] width Width of the cell to be printed
 * \param[in] align Alignment of the cell content (TXLS_ALIGN_CENTER, TXLS_ALIGN_RIGHT, or TXLS_ALIGN_LEFT)
 * \param[in] buf Pointer to the BUFFER structure for storing the printed content
 * \return 1 if the cell content is successfully printed into the buffer, 0 if the operation fails or the cell pointer is NULL
 */
static int print_cel(CELL *cell, unsigned int width, int align, BUFFER* buf)
{
    int i = 0;
    char* addr = NULL;
    unsigned int size = 0;

    if (!cell) return 0;

    /* Calculate the size of the cell's address */
    size = tsize(cell->address); 

    /* Calculate the width to be filled with spaces */
    if (width < 1) width = 1;
    if (width <= size) width = 0;
    else width = width - size;

    /* Append the required space to the buffer */
    if (!buf_append(width + size + 3)) return 0; 
    buf_putc('|');
    buf_putc(' ');

    /* Calculate the padding for center and right alignment */
    if (align == TXLS_ALIGN_CENTER) i = width / 2; 
    else if (align == TXLS_ALIGN_RIGHT) i = width;
    /* Fill the buffer with padding spaces */
    while (--i >= 0) buf_putc(' '); 

    /* Set the address pointer to the cell's address */
    addr = cell->address; 
    /* Loop through the cell's address and process each character */
    while (addr && *addr) 
    {
        /* If the character is a newline */
        if (*addr == '\n') 
        {
            buf_putc('<'); /* Add '<' to the buffer */
            buf_putc('b'); /* Add 'b' to the buffer */
            buf_putc('r'); /* Add 'r' to the buffer */
            buf_putc('>'); /* Add '>' to the buffer */
        }
        /* If the character is a '|' */
        else if (*addr == '|') 
        {
            buf_putc('\\'); /* Add '\' to the buffer */
            buf_putc('|');  /* Add '|' to the buffer */
        }
        /* For other characters */
        else 
        {
            buf_putc(*addr);
        }

        /* Move to the next character */
        addr++; 
    }

    /* Calculate the padding for center and left alignment */
    if (align == TXLS_ALIGN_CENTER) i = width - width / 2; 
    else if (align == TXLS_ALIGN_LEFT || align == TXLS_ALIGN_UNKNOW) i = width;
    /* Fill the buffer with padding spaces */
    while (--i >= 0) buf_putc(' '); 

    /* Add a space to the buffer */
    buf_putc(' '); 

    return 1;
}

/**
 * \brief Prints a dividing line in the buffer with a specific width and alignment
 *
 * \param[in] width The width of the dividing line
 * \param[in] align The alignment of the dividing line (TXLS_ALIGN_LEFT, TXLS_ALIGN_CENTER, or TXLS_ALIGN_RIGHT)
 * \param[in] buf The pointer to the BUFFER structure where the dividing line will be printed
 * \return Returns 1 if the dividing line is successfully printed, 0 otherwise
 */
static int print_div(int width, int align, BUFFER* buf)
{
    /* Ensure the width is at least 1 */
    if (width < 1) width = 1;

    /* Attempt to append enough space in the buffer for the dividing line */
    if (!buf_append(width + 3)) return 0;

    /* Print the left edge of the dividing line */
    buf_putc('|');

    /* Print the appropriate alignment character (':' for left or center alignment, '-' for right alignment) */
    buf_putc((align == TXLS_ALIGN_LEFT || align == TXLS_ALIGN_CENTER) ? ':' : '-');

    /* Print the actual dividing line based on the specified width */
    while (width--) buf_putc('-');

    /* Print the appropriate alignment character (':' for right or center alignment, '-' for left alignment) */
    buf_putc((align == TXLS_ALIGN_RIGHT || align == TXLS_ALIGN_CENTER) ? ':' : '-');

    return 1;
}

/**
 * \brief Prints the contents of a txls_t structure in a formatted manner to the buffer
 *
 * \param[in] txls The txls_t structure containing the data to be printed
 * \param[in] buf The pointer to the BUFFER structure where the formatted contents will be printed
 * \param[in] neat Flag indicating whether to print the contents in neat format
 * \return Returns 1 if the contents are successfully printed, 0 otherwise
 */
static int print_txls(txls_t txls, BUFFER* buf, int neat)
{
    /* Pointer to a COLUMN structure */
    COLUMN *column;
    /* Pointers to CELL structures */
    CELL *cell;
    /* Loop counters */
    unsigned int i, j;

    if (!txls) return 0;

    if (txls->col == 0)
    {
        /* Set the end of the buffer to 0 */
        buf_end() = 0;
        
        return 1;
    }

    /* Loop through each row */
    for (i = 0; i <= txls->row; i++)
    {
        /* Loop through each column */
        for (j = 0; j < txls->col; j++)
        {
            /* Get the column at index j */
            column = txls_column(txls, j, txls->col);

            /* Get the cell at (i, j) */
            cell = column_cell(column, i, txls->row + 1);

            /* Print the cell contents */
            if (!print_cel(cell, neat ? column->width : 0, column->align, buf)) return 0;
        }

        /* Append 2 characters to the buffer */
        if (!buf_append(2)) return 0;
        buf_putc('|');
        buf_putc('\n');

        /* If it's the first row */
        if (i == 0)
        {
            /* Loop through each column */
            for (j = 0; j < txls->col; j++)
            {
                /* Get the column at index j */
                column = txls_column(txls, j, txls->col);

                /* Print the dividing line */
                if (!print_div(neat ? column->width : 0, column->align, buf)) return 0;
            }

            /* Append (txls->col + 2) characters to the buffer */
            if (!buf_append(txls->col + 2)) return 0;
            buf_putc('|');
            buf_putc('\n');
        }
    }

    /* Append 1 character to the buffer */
    if (!buf_append(1)) return 0;
    buf_end() = 0;

    return 1;
}

/**
 *  \brief dump the txls object into a string.
 *  \param[in] txls: txls handle
 *  \param[in] neat: output neatly, with each column aligned
 *  \param[out] *len: the length of the string actually dumped
 *  \return dumped string, please release this string space after use
 */
/**
 * \brief Dumps the contents of a txls_t structure into a character array and returns the pointer to the array
 *
 * \param[in] txls The txls_t structure containing the data to be dumped
 * \param[in] neat Flag indicating whether to dump the contents in neat format
 * \param[out] len Pointer to an integer where the length of the dumped content will be stored
 * \return Returns a pointer to the character array containing the dumped contents, or NULL if an error occurs
 */
char* txls_dumps(txls_t txls, int neat, int* len)
{
    BUFFER p;
    COLUMN *column;
    unsigned int i;
    int preset = 1;

    if (!txls) return NULL;

    /* Calculate the initial preset value */
    preset = (4 * txls->col + 2) * (txls->row + 2) + 1;

    /* If neat format is requested, calculate the additional space required */
    if (neat)
    {
        for (i = 0; i < txls->col; i++)
        {
            column = txls_column(txls, i, txls->col);
            update_width(column, NULL, txls->row);
            preset += (column->width - 1) * (txls->row + 2);
        }
    }

    /* Allocate memory for the buffer */
    p.address = (char*)malloc(preset);
    if (!p.address) return NULL;

    /* Set the buffer size */
    p.size = preset;
    /* Set the end of the buffer to 0 */
    p.end = 0;

    /* Print the contents of txls into the buffer */
    if (!print_txls(txls, &p, neat))
    {
        /* Free the allocated memory */
        free(p.address);

        return NULL;
    }

    /* Set the length of the dumped content if len pointer is provided */
    if (len) *len = p.end;

    /* Return the pointer to the character array containing the dumped contents */
    return p.address;
}

/**
 *  \brief according to the txls, generate a file.
 *  \param[in] txls: txls handle
 *  \param[in] *filename: file name
 *  \return length
 */
int txls_file_dump(txls_t txls, const char* filename)
{
    FILE* f;
    char* out;
    int len;

    if (!txls) return -1;
    if (!filename) return -1;

    out = txls_dumps(txls, 1, &len);
    if (!out) return -1;

    f = fopen(filename, "w");
    if (!f)
    {
        free(out);
        return -1;
    }

    fwrite(out, 1, len, f);
    fclose(f);

    free(out);

    return len;
}

/**
* \brief Skip leading whitespace characters in a text.
*
* \param[in] in The text to process.
* \return A pointer to the first non-whitespace character in the text.
*/
static const char* skip(const char* in)
{
    while (*in && (unsigned char)(*in) <= ' ')
    {
        /* when a newline character is encountered */
        if (*in == '\n') break;

        in++;
    }
    return in;
}

/**
 * \brief Parses a string in the given text and assigns it to the CELL structure.
 *
 * \param[in] text The input text to parse.
 * \param[out] cell Pointer to the CELL structure to store the parsed string.
 * \return A pointer to the remaining text after parsing the string.
 */
static const char* parse_string(const char* text, CELL *cell)
{
    const char *s, *e, *h;
    int len = 0, i= 0;
    char *address = NULL;

    text = skip(text);
    if (*text == '|' || *text == '\n') /* null */
    {
        return text;
    }

    s = text;
    h = s;
    while (*s)
    {
        /* To the end of the line, but there is no '|' separator at the end */
        if (*s == '\n')
        {
            E(TXLS_E_END);
            return s;
        }

        /* The '|' delimiter is positioned to complete the range of this cell */
        if (*s == '|')
        {
            e = s - 1;
            while (len > 0 && (unsigned char)(*e) <= ' ') e--, len--;
            break;
        }

        if (*s == '\\') /* escape character '|' delimiter */
        {
            if (*(s+1) == '|') s++;
        }
        else if (*s == '<') /* escape character newline \n */
        {
            if (strncmp(s, "<br>", 4) == 0) s += 3;
        }
        len++;
        s++;
    }
    if (*s == 0) /* At the end of the text, the '|' separator is not positioned */
    {
        E(TXLS_E_END);
        return s;
    }

    /* Allocate space and assign cell contents */
    if (len)
    {
        address = (char*)malloc(len + 1);
        if (!address)
        {
            E(TXLS_E_MEMORY);
            return s;
        }

        i = 0;
        while (h <= e)
        {
            if (*h == '\\')
            {
                if (*(h+1) == '|')
                {
                    address[i++] = '|';
                    h++;
                }
                else  
                {
                    address[i++] = '\\';
                }
            }
            else if (*h == '<')
            {
                if (strncmp(h, "<br>", 4) == 0)
                {
                    address[i++] = '\n';
                    h += 3;
                }
                else  
                {
                    address[i++] = '<';
                }
            }
            else  
            {
                address[i++] = *h;
            }
            h++;
        }
        address[i] = 0;
        if (cell->address) free(cell->address);
        cell->address = address;
    }

    return s;
}

/**
 * \brief Parses the header of a table in the given text and updates the txls_t structure.
 *
 * \param[in] text The input text to parse.
 * \param[in,out] txls The txls_t structure to update with the parsed header information.
 * \return A pointer to the remaining text after parsing the header.
 */
static const char* parse_head(const char* text, txls_t txls)
{
    const char* s = text;
    COLUMN *column;
    CELL *cell;
    int i = 0;
    int align = 0;

    // s = skip(s);
    if (*s != '|')
    {
        E(TXLS_E_BEGIN);
        return s;
    }

    /* parse and get header */
    while (*s)
    {
        if (*s == '|')
        {
            s = skip(s + 1);
            if (*s == '\n') continue;
            if (!txls_insert_column(txls, txls->col + 1))
            {
                E(TXLS_E_MEMORY);
                return s;
            }
            column = txls_column(txls, txls->col - 1, txls->col);
            cell = column_cell(column, 0, txls->row + 1);
            s = parse_string(s, cell);
            if (etype) return s;
        }
        else if (*s == '\n')
        {
            s++;
            eline++;
            break;
        }
    }

    // s = skip(s);
    if (*s != '|')
    {
        E(TXLS_E_BEGIN);
        return s;
    }

    i = txls->col;
    while (*s)
    {
        if (*s == '|')
        {
            column = txls_column(txls, txls->col - i, txls->col);
            s = skip(s + 1);
            if (*s == '\n') continue;
            if (i < 0)
            {
                E(TXLS_E_HEAD);
                return s;
            }
            align = 0;
            if (*s == ':')
            {
                align |= 1;
                s++;
            }
            if (*s == '-') while (*s == '-') s++;
            else
            {
                E(TXLS_E_IDENT);
                return s;
            }
            if (*s == ':')
            {
                align |= 2;
                s++;
            }
            s = skip(s);
            if (*s != '|')
            {
                E(TXLS_E_END);
                return s;
            }
            i--;
            column->align = align;
        }
        else if (*s == '\n')
        {
            if (i != 0)
            {
                E(TXLS_E_HEAD);
                return s;
            }
            eline++;
            s++;
            break;
        }
    }

    return s;
}

/**
 * \brief Parses a line of a table in the given text and updates the txls_t structure.
 *
 * \param[in] text The input text to parse.
 * \param[in,out] txls The txls_t structure to update with the parsed line information.
 * \return A pointer to the remaining text after parsing the line.
 */
static const char* parse_line(const char* text, txls_t txls)
{
    const char* s = text;
    COLUMN *column;
    CELL *cell;
    int i = 0;

    s = skip(s);
    if (!*s) return s;
    if (*s == '\n')
    {
        E(TXLS_E_BRANK);
        return s;
    }
    if (*s != '|')
    {
        E(TXLS_E_BEGIN);
        return s;
    }
    if (!txls_insert_row(txls, txls->row + 1))
    {
        E(TXLS_E_MEMORY);
        return s;
    }

    while (*s)
    {
        if (*s == '|')
        {
            if (i >= txls->col)
            {
                while (*s && *s != '\n') s++;
                continue;
            }
            column = txls_column(txls, i++, txls->col);
            cell = column_cell(column, txls->row, txls->row + 1);
            s = parse_string(s + 1, cell);
            if (etype) return s;
        }
        else if (*s == '\n')
        {
            s++;
            eline++;
            break;
        }
    }

    return s;
}

/**
 *  \brief load txls text and generate txls.
 *  \param[in] *text: address of text
 *  \return txls handler
 */
txls_t txls_loads(const char* text)
{
    txls_t txls;
    CELL *ll = NULL, *base = NULL, *up;
    int count = 0;
    const char* s;

    if (!text) return NULL;
    txls = txls_create(0, 0);
    if (!txls)
    {
        E(TXLS_E_MEMORY);
        return NULL;
    }

    etype = TXLS_E_OK;
    eline = 1;

    s = parse_head(text, txls);
    if (etype) goto FAIL;
    while (1)
    {
        s = parse_line(s, txls);
        if (etype) goto FAIL;
        if (!*s) break;
    }
    return txls;

FAIL:
    txls_delete(txls);
    return NULL;
}

/**
 *  \brief load txls file and generate txls.
 *  \param[in] *filename: filename
 *  \return txls handler
 */
txls_t txls_file_load(const char* filename)
{
    txls_t txls;
    FILE* f;
    long len;
    char* text;

    if (!filename) return NULL;

    eline = 0;

    /* open file and get the length of file */
    f = fopen(filename, "rb");
    if (!f)
    {
        E(TXLS_E_OPEN);
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
        E(TXLS_E_MEMORY);
        goto FAIL;
    }
    text[len] = 0;
    
    txls = txls_loads(text); /* load text */
    if (!txls)
    {
        goto FAIL;
    }
    
    free(text);

    return txls;

FAIL:
    // printf("error line %d code %d\r\n", eline, etype); /* output error information */
    if (f) fclose(f);
    if (text) free(text);
    return NULL;
}

/**
 *  \brief obtain parsing error information.
 *  \param[out] *line: line number where the error occurred
 *  \param[out] *type: error type, ref TXLS_E_XXX
 *  \return 1 has an error or 0 does not exist
 */
int txls_error_info(int* line)
{
    if (etype == TXLS_E_OK) return TXLS_E_OK;
    if (line) *line = eline;
    return etype;
}
