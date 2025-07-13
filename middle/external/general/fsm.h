/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  fsm.h
 *        \brief  This is a C language universal finite state machine module.
 *       \author  Lamdonn
 *      \version  1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __fsm_H
#define __fsm_H

/* Version infomation */
#define FSM_V_MAJOR                 1
#define FSM_V_MINOR                 0
#define FSM_V_REVISE                0

/* State transition type definition */
typedef struct
{
    int from;                       /* from stste */
    int to;                         /* to state */
    int event;                      /* event id */
    void (*action)(int event);      /* transform action */
} StateTransform;

/* Finite State Machine type define */
typedef struct
{
    int state;                      /* current stste */
    StateTransform* trans;          /* transform table */
    int count;                      /* size of trans */
} FSM;

/**
 *  \brief initialize the state machine.
 *  \param[in] fsm: state machine structure address
 *  \param[in] trans: state transition table
 *  \param[in] count: count of state transition table
 *  \param[in] state: initial state
 *  \return 1 success or 0 fail
 */
int fsm_init(FSM* fsm, StateTransform* trans, int count, int state);

/**
 *  \brief execution state machine.
 *  \param[in] fsm: state machine structure address
 *  \param[in] event: state transition event
 *  \return 1 success or 0 fail
 */
int fsm_execute(FSM* fsm, int event);

#if 0 /* for example */
#include <stdio.h>
#include "fsm.h"

enum
{
    TestState_Init = 0,
    TestState_Stanby,
    TestState_Run,
    TestState_Error,
    TestState_Exit,
};

enum
{
    TestEvent_Init = 0,
    TestEvent_ToRun,
    TestEvent_ToStanby,
    TestEvent_Error,
    TestEvent_Exit,
};

/* State transition diagram
+-----------+          +-----------+              +-----------+
|           |          |           | -----------> |           |
|   init    | -------> |  standby  |              |    run    |
|           |          |           | <----------  |           |
+-----------+          +-----------+              +-----------+
                             |       \          /       |
                             |        \        /        |
                             |         \      /         |
                             |          \    /          |
                             |           \  /           |
                             |            \/            |
                             |            /\            |
                             |           /  \           |
                             |          /    \          |
                             |         /      \         |
                             |        /        \        |
                             |       /          \       |
                             v      v            v      v
                       +-----------+              +-----------+
                       |           |              |           |
                       |   error   | -----------> |   exit    |
                       |           |              |           |
                       +-----------+              +-----------+
*/
StateTransform TestFsmTable[] = 
{   /*  from,               to,                 event,              action */
    {TestState_Init,    TestState_Stanby,   TestEvent_Init,     NULL},
    {TestState_Stanby,  TestState_Run,      TestEvent_ToRun,    NULL},
    {TestState_Stanby,  TestState_Error,    TestEvent_Error,    NULL},
    {TestState_Stanby,  TestState_Exit,     TestEvent_Exit,     NULL},
    {TestState_Run,     TestState_Stanby,   TestEvent_ToStanby, NULL},
    {TestState_Run,     TestState_Error,    TestEvent_Error,    NULL},
    {TestState_Run,     TestState_Exit,     TestEvent_Exit,     NULL},
    {TestState_Error,   TestState_Exit,     TestEvent_Exit,     NULL},
};

int main(int argc, char* argv[])
{
    FSM TestFsm;
    int event[] = { TestState_Init, TestEvent_ToRun, TestState_Exit };

    fsm_init(&TestFsm, TestFsmTable, 8, TestState_Init);

    printf("%d\r\n", TestFsm.count);

    for (int i = 0; i < sizeof(event) / sizeof(event[0]); i++)
    {
        fsm_execute(&TestFsm, event[i]);
        printf("state %d\r\n", TestFsm.state);
    }

    printf("%c", getchar());
    return 0;
}
#endif 

#endif
