/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  fsm.c
 *        \brief  This is a C language universal finite state machine module.
 *       \author  Lamdonn
 *      \version  1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "fsm.h"
#include "stdlib.h"

int fsm_init(FSM* fsm, StateTransform* trans, int count, int state)
{
    if (!fsm) return 0;
    if (!trans) return 0;
    if (count < 1) return 0;

    fsm->trans = trans;
    fsm->count = count;
    fsm->state = state;

    return 1;
}

int fsm_execute(FSM* fsm, int event)
{
    StateTransform* trans = NULL;
    int i = 0;

    if (fsm == NULL) return 0;
    
    for (i = 0; i < fsm->count; i++)
    {
        if ((fsm->state == fsm->trans[i].from) && (event == fsm->trans[i].event))
        {
            trans = &fsm->trans[i];
            break;
        }
    }
    if (trans == NULL) return 0;

    fsm->state = trans->to;

    if (trans->action) trans->action(event);

    return 1;
}