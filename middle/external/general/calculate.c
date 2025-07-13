/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  calculate.h
 *         \unit  calculate
 *        \brief  This is a simple math expression calculation module for C language
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "calculate.h"
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <string.h>

/* Constant and macro definitions */
#define PI                  3.141592653589793238462643383279502884197169399375105820974944592308
#define E                   2.718281828459045235360287471352662497757247093699959574966967627724
#define isint(n)            (fabs(floor(n) - n) <= DBL_EPSILON && fabs(n) < 1.0e60)

/* Minimum unit information of calculation expression */
typedef struct
{
    char type;                                                  /**< Type of information, '+', '-', '*' ... */
    double value;                                               /**< Unit value */
} cinfo_t;

/* Calculation function structure definition */
typedef struct 
{
    char *name;                                                 /**< Function name */
    int len;                                                    /**< Length of function name */
    double (*func)();                                           /**< C function that performs calculations */
    int argc;                                                   /**< Number of function arguments, Maximum support 8 */
} function_t;

/* The definition of the built-in function corresponding to the calculation function */
static double cot(double v1) { return 1 / tan(v1); }
static double acot(double v1) { return atan(1 / v1); }
static double min(double v1, double v2) { return (v1<v2)?v1:v2; }
static double max(double v1, double v2) { return (v1>v2)?v1:v2; }
static double log_r(double v1, double v2) { return log(v2) / log(v1); }

/* Built-in function table */
static function_t in_function_table[] = 
{ /* name        len    func        argc */
    {"abs",      3,     fabs,       1},
    {"sqrt",     4,     sqrt,       1},
    {"exp",      3,     exp,        1},
    {"ln",       2,     log,        1},
    {"log10",    5,     log10,      1},
    {"sin",      3,     sin,        1},
    {"cos",      3,     cos,        1},
    {"tan",      3,     tan,        1},
    {"cot",      3,     cot,        1},
    {"asin",     4,     asin,       1},
    {"acos",     4,     acos,       1},
    {"atan",     4,     atan,       1},
    {"acot",     4,     acot,       1},
    {"ceil",     4,     ceil,       1},
    {"floor",    5,     floor,      1},
    {"round",    5,     round,      1},
    {"min",      3,     min,        2},
    {"max",      3,     max,        2},
    {"pow",      3,     pow,        2},
    {"log",      3,     log_r,      2},
};

/* External function table */
static function_t ex_function_table[CALCULATE_EXFUNC_MAX];

/* The current number of external functions */
static int ex_function_num = 0;

/* Function declaration */
static char *parse_value(char *p, double *n);

static char* skip(char* in)
{
    while (*in && (unsigned char)*in <= ' ') in++;
    return in;
}

/**
 *  \brief convert numeric string to numeric value
 *  \param[in] *s: numeric string
 *  \param[in] len: the length of the string to be converted
 *  \return convert result or NAN fail
 */
static double v_atof(const char *s, int len)
{
    long double result = 0.0L;
    int sign = 1; /* +/- sign */
    int fraction = 0; /* decimal part flag, 1 has a decimal part, but 0 does not. */
    long double div = 1.0L; /* decimal division factor */
    
    /* Get the numerical prefix, positive or negative */
    if (*s == '-') { sign = -1; s++; }
    else if (*s == '+') { s++; }
    
    /* Within the valid length and legal characters, convert bit by bit */
    while (len-- > 0 && (isdigit(*s) || *s == '.'))
    {
        /* Parsed to the decimal point, the value has a decimal part */
        if (*s == '.')
        {
            /* There is already a decimal point, no more decimal points are allowed. */
            if (fraction == 1) return NAN;
            
            /* Mark the decimal point and skip the decimal point */
            fraction = 1;
            s++;
            continue;
        }
        
        /* Integer part */
        if (fraction == 0)
        {
            result = result * 10.0L + (long double) (*s - '0');
        }
        /* Decimal part */
        else 
        {
            div *= 10.0L;
            result = result + (long double) (*s - '0') / div;
        }
        
        s++;
    }
    
    /* Skip extra spaces */
    while (len-- > 0)
    {
        if (*s++ > ' ') return NAN;
    }
    
    return sign * result;
}

static double get_number(char *p, int size)
{
    cinfo_t adata[size];
    cinfo_t *cal = NULL, *pre = NULL;
    cinfo_t info;
    int i = 0;
    double n = 0;

    /* Skip invalid characters */
    p = skip(p);
    
    /* Get sign for unit information */
    info.type = '+';
    if (*p == '-')
    {
        info.type = '-';
        p = skip(p + 1);
    }
    
    /* Parse out the first value and store it in the `adada` table */
    p = skip(parse_value(p, &info.value));
    adata[i++] = info;

    /* Consecutively parse each remaining subexpression */
    while (*p >= ' ' && *p != ')' && *p != ',') /* Encountering ')' or ',', ends the scope of the subexpression */
    {
        info.type = *p;
        p = skip(parse_value(skip(p + 1), &info.value));
        adata[i++] = info;
    }
    
    /* Stepwise symbolic operations according to the precedence of the operation symbols
     * 1. '^'               right union
     * 2. '*', '/', '%'     left union
     * 3. '+', '-'          left union
     */

    /* '^' */
    for (i = size - 1; i > 0; i--)
    {
        cal = adata + i;
        pre = adata + i - 1;
        if (cal->type == '^')
        {
            cal->type = '=';
            cal->value = pow(pre->value, cal->value);
            pre->value = cal->value;
        }
    }

    /* '*', '/', '%' */
    for (i = 1; i < size; i++)
    {
        cal = adata + i;
        pre = adata + i - 1;
        
        if      (cal->type == '=') { cal->value = pre->value; }
        else if (cal->type == '*') { cal->type = '='; cal->value = pre->value * cal->value; }
        else if (cal->type == '/') { cal->type = '='; cal->value = pre->value / cal->value; }
        else if (cal->type == '%') { cal->type = '='; cal->value = fmod(pre->value, cal->value); }
    }

    for (i = size - 1; i > 0; i--)
    {
        cal = adata + i;
        pre = adata + i - 1;
        
        if (cal->type == '=') { pre->value = cal->value; }
    }

    /* '+', '-' */
    for (i = 0; i < size; i++)
    {
        cal = adata + i;
        
        if      (cal->type == '+') n += cal->value;
        else if (cal->type == '-') n -= cal->value;
    }

    return n;
}

static char* evaluate_expression(char *p, double *n)
{
    double t = NAN;
    char *s = p;
    int size = 0;

    *n = NAN;

    /* Skip invalid characters */
    p = skip(p);
    
    /* Skip sign for unit information */
    if (*p == '-') p = skip(p + 1);
    
    /* Preliminarily parse the calculation expression, that is, check the expression syntax */
    p = skip(parse_value(p, &t));
    if (isnan(t)) return p;
    
    size++;
    
    /* Divide the calculation expression into independent minimum operation units */
    while (*p >= ' ' && *p != ')' && *p != ',')
    {
        if (*p != '+' && *p != '-' && *p != '*' && *p != '/' && *p != '%' && *p != '^') return p;
        p = skip(parse_value(skip(p + 1), &t));
        if (isnan(t)) return p;
        size++;
    }

    /* Get expression evaluation result */
    *n = get_number(s, size);

    return p;
}

static char *parse_value(char *p, double *n)
{
    double value, v[8];
    char sign = '+', *s;
    int i, count, argc;
    function_t *function = NULL;

    *n = NAN;

    /* Get sign for unit information */
    p = skip(p);
    if (*p == '-' || *p == '+') sign = *p++;
    
    /* Skip invalid characters and start parsing from valid characters */
    p = skip(p);
    s = p;
    
    count = sizeof(in_function_table) / sizeof(in_function_table[0]);
    
    while (*p && *p != ')' && *p != ',' && *p != '+' && *p != '-' && *p != '*' && *p != '/' && *p != '%' && *p != '^')
    {
        /* When brackets '"()" are encountered, the expression inside the brackets is evaluated first.
         * In the brackets, there may be a unit, or an expression, or several parameters of the function.
         */
        if (*p == '(')
        {
            p = skip(p + 1);
            
            /* The brackets are immediately adjacent to the previous character. */
            /* Calculate the result and return it directly */
            if (p == skip(s + 1)) 
            {
                p = evaluate_expression(p, &value);
                if (*p == ')') *n = value;
                return p + 1;
            }

            /* built-in function */
            for (i = 0; i < count; i++)
            {
                if (!strncmp(s, in_function_table[i].name, in_function_table[i].len))
                {
                    /* Check each parameter, which is an expression */
                    for (argc = 0; argc < in_function_table[i].argc; argc++)
                    {
                        p = evaluate_expression(p, &v[argc]);
                        if (isnan(v[argc])) return p;
                        if (argc == in_function_table[i].argc - 1) { if (*p != ')') return p; }
                        else { if (*p != ',') return p; }
                        p++;
                    }
                    
                    argc = in_function_table[i].argc;
                    function = &in_function_table[i];
                    break;
                    
                }
            }
            
            /* extern function */
            if (!function)
            {
                for (i = 0; i < ex_function_num; i++)
                {
                    if (!strncmp(s, ex_function_table[i].name, ex_function_table[i].len))
                    {
                        /* Check each parameter, which is an expression */
                        for (argc = 0; argc < ex_function_table[i].argc; argc++)
                        {
                            p = evaluate_expression(p, &v[argc]);
                            if (isnan(v[argc])) return p;
                            if (argc == ex_function_table[i].argc - 1) { if (*p != ')') return p; }
                            else { if (*p != ',') return p; }
                            p++;
                        }

                        argc = ex_function_table[i].argc;
                        function = &ex_function_table[i];
                        break;

                    }
                }
            }
            
            /* Call the function based on the number of function arguments */
            if (function)
            {
                switch (argc) 
                {
                case 1: *n = function->func(v[0]); break;
                case 2: *n = function->func(v[0], v[1]); break;
                case 3: *n = function->func(v[0], v[1], v[2]); break;
                case 4: *n = function->func(v[0], v[1], v[2], v[3]); break;
                case 5: *n = function->func(v[0], v[1], v[2], v[3], v[4]); break;
                case 6: *n = function->func(v[0], v[1], v[2], v[3], v[4], v[5]); break;
                case 7: *n = function->func(v[0], v[1], v[2], v[3], v[4], v[5], v[6]); break;
                case 8: *n = function->func(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]); break;
                }
            }
            
            return p;
        }
        
        p = skip(p + 1);
    }

    /* constant */
    if ((s[0] == 'p' || s[0] == 'P') && (s[1] == 'i' || s[1] == 'I')) value = PI;
    else if (s[0] == 'e' || s[0] == 'E') value = E;
    else if (s[0] >= '0' && s[0] <= '9') value = v_atof(s, p - s);
    else return p;

    if (isnan(value)) return p;

    *n = (sign == '+') ? value : -value;

    return p;
}

int calculate_export(const char *name, double (*func)(), int argc)
{
    int i = 0, count, len;
    
    if (ex_function_num >= CALCULATE_EXFUNC_MAX) return 0;
    
    /* check validity */
    if (!name || !func) return 0;
    
    if (argc <= 0) return 0;
    
    len = strlen(name);
    if (len <= 0) return 0;

    /* Traverse the function list and check if there are duplicate functions */
    count = sizeof(in_function_table) / sizeof(in_function_table[0]);
    for (i = 0; i < count; i++)
    {
        if (!strncmp(name, in_function_table[i].name, in_function_table[i].len))
        {
            return 0;
        }
    }
    
    for (i = 0; i < ex_function_num; i++)
    {
        if (!strncmp(name, ex_function_table[i].name, ex_function_table[i].len))
        {
            return 0;
        }
    }
    
    /* Add a function to the extern function list */
    ex_function_table[ex_function_num].name = (char *)name;
    ex_function_table[ex_function_num].len = len;
    ex_function_table[ex_function_num].func = func;
    ex_function_table[ex_function_num].argc = argc;
    ex_function_num++;
    
    return 1;
}

double calculate(const char *expression)
{
    double n;
    char *p;
    
    /* Check the validity of a calculated expression */
    if (!expression) return NAN;
    
    /* Start evaluating a calculation expression */
    p = evaluate_expression((char *)expression, &n);
    if (isnan(n) || *p)
    {
        printf("Calculate fail at column %d\r\n", (int)(p - expression));
        return NAN;
    }
    
    return n;
}

