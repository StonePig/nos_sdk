
/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  command.h
 *         \unit  command
 *        \brief  This is a simple string command parsing module for C language
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __command_H
#define __command_H

/* Version infomation */
#define COMAMND_V_MAJOR             1
#define COMAMND_V_MINOR             0
#define COMAMND_V_PATCH             0

/* Configuration information */
#define COMMAND_ARGC_MAX            16                      /**< The maximum number of parameters supported for parsing in the input command */
#define COMMAND_LINE_MAX            256                     /**< The maximum length supported for parsing in the input command */
#define COMMAND_COUNT_MAX           32                      /**< The maximum command count supported */

/* Return value of command module
 * Negative return values are used internally by the module
 * Positive return values are used by the command callback processing function.
 */
#define COMMAND_E_OK                0                       /**< Return correctly */
#define COMMAND_E_LINE              -1                      /**< Command line error, no valid characters */
#define COMMAND_E_ARGC              -2                      /**< Argument count exceeds maximum limit */
#define COMMAND_E_LENGTH            -3                      /**< Command length exceeds maximum limit */
#define COMMAND_E_COUNT             -4                      /**< Command count exceeds maximum limit */
#define COMMAND_E_MATCH             -5                      /**< No matching command in command list */
#define COMMAND_E_NULL              -6                      /**< NULL pointer */
#define COMMAND_E_REPEAT            -7                      /**< Repeated commands */

/**
 * Defines a function pointer type `command_handle_t`.
 *
 * The `command_handle_t` type represents a pointer to a function that takes two arguments:
 * - `int argc`: The number of command line arguments.
 * - `char *argv[]`: An array of command line arguments.
 *
 * The function pointed to by `command_handle_t` returns an `int` value.
 *
 * This function pointer type is typically used to define callbacks or handlers for command processing or execution.
 */
typedef int (*command_handle_t)(int argc, char *argv[]);

/**
 * Stores communication from `command_getopt` to the caller.
 *
 * In `command_getopt`, when an option that requires an argument is found, the argument value is returned through this variable.
 *
 * Additionally, when the `ordering` parameter is set to `RETURN_IN_ORDER`, each non-option ARGV-element is also returned through this variable.
 */
extern char *command_optarg;
 
/**
 * Stores the index in ARGV of the next element to be scanned.
 *
 * This variable is used for communication between the caller and `command_getopt`, as well as between successive calls to `command_getopt`.
 *
 * - When `command_getopt` is called for the first time, a value of zero indicates that it is the first call and initialization is required.
 * - When `command_getopt` returns -1, `command_optind` indicates the index of the first non-option element that the caller should scan.
 * - Otherwise, `command_optind` is used to communicate how much of ARGV has been scanned so far from one `command_getopt` call to the next.
 */
extern int command_optind;
 
/**
 * Stores the unrecognized option character that was encountered.
 */
extern int command_optopt;
                                    
/**
 *  \brief command line parsing function.
 *  \param[in] *line: command line
 *  \return COMMAND_E_XXX
 */
int command(const char *line);

/**
 *  \brief export command.
 *  \param[in] *name: command name
 *  \param[in] handle: command handle
 *  \return COMMAND_E_XXX
 */
int command_export(const char *name, command_handle_t handle);

/**
 *  \brief clear all exported commands.
 *  \return none
 */
void command_clear(void);

/**
 * \brief Parses the command line arguments for options using the short option format.
 *
 * \param[in] argc: The number of command line arguments.
 * \param[in] argv: The array of command line arguments.
 * \param[in] optstring: The string containing valid option characters.
 *
 * \return The next option character or a special value indicating the end or an error.
 */
int command_getopt(int argc, char *argv[], const char *optstring);

/**
 * \brief Initialize command options and execute this function before using `command_getopt` to traverse parameters.
 * \return none
 */
void command_opt_init(void);

#endif

