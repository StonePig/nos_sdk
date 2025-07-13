/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  command.c
 *         \unit  command
 *        \brief  This is a simple string command parsing module for C language
 *       \author  Lamdonn
 *      \version  v1.4.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "command.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>                        

struct COMMAND
{
    char *name;                                             /**< The name of the command, the command name itself has no space, so the string pointer passed in needs to be a constant. */
    command_handle_t handle;                                /**< Command matching callback handler function */
};

int command_optind = 1;                                     /* index into parent argv vector */
int command_optopt = '?';                                   /* character checked for validity */
char *command_optarg = NULL;                                /* argument associated with option */

static int command_num = 1;                                 /* count of commands */
static int reset = 1;                                       /* reset command_getopt */
static char *place = "";                                    /* option letter processing */
static int nstart = -1;                                     /* first non option argument (for permute) */
static int nend = -1;                                       /* first option after non options (for permute) */

static int cmd(int argc, char *argv[]);

/* The basis of the command array */
static struct COMMAND base[COMMAND_COUNT_MAX] = 
{
/*  name        handle  */
    {"cmd",     cmd},
};

static void usage(void)
{
    printf(
"Usage:\n"
"Enter the command line to execute the corresponding command\n"
"\n"
"OPTIONS\n"
"[-l] : Print currently supported commands\n"
"[-n] : Print the number of currently supported commands\n"
"[-h] : Print help\n"
"[-v] : Print version\n"
"[-c] : Print the configuration information of the current command module\n"
"       `argc`  : The maximum number of parameters supported for parsing in the input command\n"
"       `line`  : The maximum length supported for parsing in the input command\n"
"       `count` : The maximum command count supported\n"
    );
}

/**
 *  \brief callback handler function of `cmd` command, this command is a built-in function of the command module
 *  \param[in] argc: count of arguments
 *  \param[in] argv: value of arguments
 *  \return result
 */
static int cmd(int argc, char *argv[])
{
    int opt;
    int flag = 0;
    int i;

    /* reset getopt */
    command_opt_init();
    while (1)
    {
        opt = command_getopt(argc, argv, "lnhvc:");
        if (opt == -1) break;

        switch (opt) 
        {
        case 'l' :
            printf("command list: \r\n");
            for (i = 0; i < command_num; i++)
            {
                printf("@ %s\r\n", base[i].name);
            }
            break;
        case 'n':
            printf("%d\r\n", command_num);
            break;
        case 'h' :
            usage();
            break;
        case 'v' :
            printf("command version %d.%d.%d\r\n", COMAMND_V_MAJOR, COMAMND_V_MINOR, COMAMND_V_PATCH);
            break;
        case 'c' :
            if (!strcmp(command_optarg, "argc"))
            {
                printf("COMMAND_ARGC_MAX: %d\r\n", COMMAND_ARGC_MAX);
            }
            else if (!strcmp(command_optarg, "line"))
            {
                printf("COMMAND_LINE_MAX: %d\r\n", COMMAND_LINE_MAX);
            }
            else if (!strcmp(command_optarg, "count"))
            {
                printf("COMMAND_COUNT_MAX: %d\r\n", COMMAND_COUNT_MAX);
            }
            else 
            {
                printf("no such optarg: %s\r\n", command_optarg);
            }
            break;
        case '?':
            printf("Unknown option `%c`\r\n", command_optopt);
            return -1;
        default:
            usage();
            break;
        }
    }
    
    return 0;
}

/**
 * \brief Skips leading whitespace characters in a string and returns a pointer to the first non-whitespace character.
 *
 * \param[in] The input string.
 * \return A pointer to the first non-whitespace character in the string.
 */
static char* skip(char* in)
{
    while (*in && (unsigned char)*in <= ' ') in++;
    return in;
}

int command(const char *line)
{
    static char s[COMMAND_LINE_MAX];        /* Ensure that the input command is not modified and the parsed arguments will exist here. */
    static char *argv[COMMAND_ARGC_MAX];    /* The parsed arguments pointer will exist here */
    
    int argc = 0;
    char *in = (char *)line;
    char *out = s;
    int i = 0;
    
    if (strlen(line) >= COMMAND_LINE_MAX) return COMMAND_E_LENGTH;

    /* Check whether the entered command line is empty command
     * When there are valid characters, whether the command line conforms to the specification on the input
     */
    in = skip(in); /* skip spaces */
    if (!*in) return COMMAND_E_LINE; /* no valid characters */
    
    *out = 0;
    argv[argc++] = out;
    if (argc > COMMAND_ARGC_MAX) return COMMAND_E_ARGC;
    
    while (*in)
    {
        /* Space separator
         * When a space character is encountered, it indicates that the parsing of the previous parameter has ended.
         * Stop parsing arguments and record the arguments.
         */
        if (*in == ' ')
        {
            *out++ = 0; /* Add a terminator to the previous argument */
            
            in = skip(in); /* skip spaces */
            if (!*in) break;
            
            argv[argc++] = out; /* Start new argument parsing */
            if (argc > COMMAND_ARGC_MAX) return COMMAND_E_ARGC;
        }
        /* Escape character
         * The escape character is `\` + specific character
         */
        else if (*in == '\\')
        {
            in++; /* skip '\' */
            if      (*in == ' ')  { *out++ = ' '; }
            else if (*in == '\\') { *out++ = '\\'; }
            else if (*in == '\"') { *out++ = '\"'; }
            else                  { *out++ = '\\', *out++ = *in; } /* Copy `\` with normal character */
            in++;
        }
        /* Double quotes
         * Characters enclosed in double quotes will not be parsed, except for escaped double quotes
         */
        else if (*in == '\"')
        {
            in++; /* skip '"' */
            while (*in)
            {
                if (*in == '\"') break; /* Parsed to `"` on the right, double quotes form a closed range */
                
                /* Within a double quote range, if an escaped double quote is encountered, 
                 * it will not be treated as a right double quote, but as an ordinary character. 
                 */
                if (*in == '\\' && *(in + 1) == '\"') in++; 
                
                *out++ = *in++;
            }
            i++;
        }
        /* General character
         */
        else
        {
            *out++ = *in++;
        }
    }
    *out = 0;

    /* Match command 
     * Match the commands in the command list one by one
     */
    for (i = 0; i < command_num; i++)
    {
        /* If successfully matched, execute the callback processing function directly and return */
        if (!strcmp(argv[0], base[i].name))
        {
            return (base[i].handle)(argc, argv);
        }
    }
    
    printf("No '%s' such command!\r\n", argv[0]);

    return COMMAND_E_MATCH;
}

int command_export(const char *name, command_handle_t handle)
{
    int i = 0;
    
    if (command_num >= COMMAND_COUNT_MAX) return COMMAND_E_COUNT;
    
    /* check validity */
    if (!name || !handle) return COMMAND_E_NULL;

    /* Traverse the command list and check if there are duplicate commands */
    for (i = 0; i < command_num; i++)
    {
        if (!strcmp(name, base[i].name))
        {
            return COMMAND_E_REPEAT;
        }
    }
    
    /* Add a command to the command list */
    base[command_num].name = (char *)name;
    base[command_num].handle = handle;
    command_num++;

    return COMMAND_E_OK;
}

void command_clear(void)
{
    command_num = 1;
}
 
/**
 * \brief Calculates the greatest common divisor (GCD) of two integers.
 *
 * \param[in] a: The first integer.
 * \param[in] b: The second integer.
 * \return The GCD of the two integers.
 */
static int gcd(int a, int b)
{
    int c = a % b;

    while (c != 0) 
    {
        a = b;
        b = c;
        c = a % b;
    }
 
    return b;
}

/**
 * \brief Rearranges a subset of the command-line arguments based on given indices.
 *
 * Args within the specified indices are treated as non-optional (panonopt),
 * from panonopt_start (inclusive) to panonopt_end (exclusive),
 * followed by optional arguments from panonopt_end (inclusive) to opt_end (exclusive).
 *
 * \param[in] panonopt_start: The start index for non-optional arguments (inclusive).
 * \param[in] panonopt_end: The end index for non-optional arguments (exclusive).
 * \param[in] opt_end: The end index for all optional arguments (exclusive).
 * \param[in,out] argv: The command-line arguments array that needs to be rearranged.
 * \return none.
 */
static void permute_args(int panonopt_start, int panonopt_end, int opt_end, char *argv[]) 
{
    int i, j;
    int cstart, pos;
    char *swap;
    
    /* calculate the lengths of non-optional and optional argument blocks. */
    int nnonopts = panonopt_end - panonopt_start;
    int nopts = opt_end - panonopt_end;
    
    /* determine the number of cycles needed and the length of each cycle. */
    int ncycle = gcd(nnonopts, nopts); /* custom or library function to find greatest common divisor. */
    int cyclelen = (opt_end - panonopt_start) / ncycle;

    /* iterate over each cycle. */
    for (i = 0; i < ncycle; i++) 
    {
        cstart = panonopt_end + i;  /* cycle start index. */
        pos = cstart;  /* current position within the cycle. */
        
        /* permute the cycle. */
        for (j = 0; j < cyclelen; j++) 
        {
            /* determine the new position within the cycle, wrapping around if necessary. */
            if (pos >= panonopt_end) 
            {
                pos -= nnonopts;
            } 
            else 
            {
                pos += nopts;
            }
            
            /* swap the arguments at pos and cstart. */
            swap = argv[pos];
            argv[pos] = argv[cstart];
            argv[cstart] = swap;
        }
    }
}

int command_getopt(int argc, char *argv[], const char *optstring)
{
    char *index;                        /* option letter list index */
    int optc;                           /* option char */
    char premute = 1;                   /* permute non-options to the end of argv */
    char allargs = 0;                   /* treat non-options as args to option "-1" */
 
    if (!optstring) return -1;
 
    if (*optstring == '+')
    {
        premute = 0;
        optstring++;
    }
    else if (*optstring == '-')
    {
        allargs = 1;
        optstring++;
    }

    command_optarg = NULL;
 
    /* reset getopt function */
    if (reset)
    {
        command_optind = 1;
        command_optopt = '?';
        nstart = -1;
        nend = -1;
        place = "";
    }

    while (1) 
    {
        if (reset || !*place) /* update scanning pointer */
        {
            reset = 0;

            /* end of argument vector */
            if (command_optind >= argc)
            {
                place = "";
                if (nend != -1)
                {
                    /* do permutation, if have to */
                    permute_args(nstart, nend, command_optind, argv);
                    command_optind -= (nend - nstart);
                }
                else if (nstart != -1)
                {
                    /* if skipped non-options, set command_optind to the first of them. */
                    command_optind = nstart;
                }

                nstart = -1;
                nend = -1;

                return -1;
            }

            place = argv[command_optind];

            if (place[0] != '-' || (place[1] == '\0' && !strchr(optstring, '-')))
            {
                place = "";        /* found non-option */
                if (allargs)
                {
                    /* return non-option as argument to option 1 */
                    command_optarg = argv[command_optind++];
                    return 1;
                }

                /* if no permutation wanted, stop parsing at first non-option. */
                if (!premute) return -1;

                /* do permutation */
                if (nstart == -1)
                {
                    nstart = command_optind;
                }
                else if (nend != -1) 
                {
                    permute_args(nstart, nend, command_optind, argv);
                    nstart = command_optind - (nend - nstart);
                    nend = -1;
                }
                command_optind++;
                /* process next argument */
                continue;
            }
            if (nstart != -1 && nend == -1)
            {
                nend = command_optind;
            }
      
            /* if have "-" do nothing */
            if (place[1] != '\0' && *++place == '-' && place[1] == '\0') 
            {
                command_optind++;
                place = "";

                /* if skipped non-options, have to permute. */
                if (nend != -1) 
                {
                    permute_args(nstart, nend, command_optind, argv);
                    command_optind -= (nend - nstart);
                }

                nstart = -1;
                nend = -1;
                return -1;
            }
        }
      
        optc = (int)*place++;
        if ((optc == (int)':') ||
            (optc == (int)'-' && *place != '\0') ||
            (index = strchr(optstring, optc)) == NULL) 
        {
            /*
             * if the user specified "-" and  '-' isn't listed in options, return -1 (non-option) as per POSIX
             * otherwise, it is an unknown option character (or ':').
             */
            if (optc == (int)'-' && *place == '\0') return -1;
            
            if (!*place) ++command_optind;
            
            command_optopt = optc;
            
            return '?';
        }
        if (*++index != ':') /* doesn't take argument */
        {
            if (!*place) ++command_optind;
        } 
        else /* takes (optional) argument */
        {
            command_optarg = NULL;
            if (*place) /* no white space */
            {
                command_optarg = place;
            }
            else if (index[1] != ':') /* arg not optional */
            {
                if (++command_optind >= argc) /* no arg */
                {
                    place = "";
                    command_optopt = optc;
                    return (*optstring == ':') ? ':' : '?';
                } 
                else
                {
                    command_optarg = argv[command_optind];
                }
            } 
            else if (!premute) 
            {
                /* if permutation is disabled, can accept an optional arg separated by whitespace so long as it does not start with a dash (-). */
                if (command_optind + 1 < argc && *argv[command_optind + 1] != '-')
                {
                    command_optarg = argv[++command_optind];
                }
            }
            
            place = "";
            ++command_optind;
        }
        /* dump back option letter */
        return optc;
    }
}

void command_opt_init(void)
{
    reset = 1;
}
