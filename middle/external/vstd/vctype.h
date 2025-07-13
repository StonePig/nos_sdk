/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  vctype.h
 *         \unit  vctype
 *        \brief  Similar to the C standard library ctype
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __vctype_H
#define __vctype_H

#if 0 /* BIG_ENDIAN */
#define _V_ISbit(bit)    (1 << (bit))
#else /* LITTLE_ENDIAN */
#define _V_ISbit(bit)    ((bit) < 8 ? ((1 << (bit)) << 8) : ((1 << (bit)) >> 8))
#endif

enum
{
    _V_ISupper = _V_ISbit (0),    /* UPPERCASE.  */
    _V_ISlower = _V_ISbit (1),    /* lowercase.  */
    _V_ISalpha = _V_ISbit (2),    /* Alphabetic.  */
    _V_ISdigit = _V_ISbit (3),    /* Numeric.  */
    _V_ISxdigit = _V_ISbit (4),    /* Hexadecimal numeric.  */
    _V_ISspace = _V_ISbit (5),    /* Whitespace.  */
    _V_ISprint = _V_ISbit (6),    /* Printing.  */
    _V_ISgraph = _V_ISbit (7),    /* Graphical.  */
    _V_ISblank = _V_ISbit (8),    /* Blank (usually SPC and TAB).  */
    _V_IScntrl = _V_ISbit (9),    /* Control character.  */
    _V_ISpunct = _V_ISbit (10),    /* Punctuation.  */
    _V_ISalnum = _V_ISbit (11)    /* Alphanumeric.  */
};

extern int v_tolower(int c);
extern int v_toupper(int c);
extern int v_isascii(int c);
extern int v_toascii(int c);
extern int v_isalpha(int c);
extern int v_isdigit(int c);
extern int v_isalnum(int c);
extern int v_iscntrl(int c);
extern int v_islower(int c);
extern int v_isupper(int c);
extern int v_isgraph(int c);
extern int v_isprint(int c);
extern int v_ispunct(int c);
extern int v_isspace(int c);
extern int v_isxdigit(int c);
extern int v_isctype(int c, int mask);

#endif 
