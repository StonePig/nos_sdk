/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  cPatten.h
 *         \unit  cPatten
 *        \brief  This is a C language artistic character patterns
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __cPatten_H
#define __cPatten_H

#include <stdlib.h>

/* Version infomation */

#define CPATTEN_V_MAJOR                     1
#define CPATTEN_V_MINOR                     0
#define CPATTEN_V_PATCH                     0

/**
 *  \brief Set masking character.
 *  \param[in] c: Mask characters, which are artistic characters composed of these characters
 *  \return 1 success or 0 fail
 */
int cPatten_setMask(char c);

/**
 *  \brief Show artistic character.
 *  \param[in] c: char
 *  \return none
 */
void cPatten_showChar(char c);

/**
 *  \brief Show artistic string.
 *  \param[in] s: string
 *  \return none
 */
void cPatten_showString(char *s);


#endif
