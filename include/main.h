/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:03:17 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/18 09:10:26 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/wait.h>
#include <sys/resource.h>

#include <signal.h>

#include <errno.h>

#include "../libft/libft.h"

#include <limits.h>

/** TOKNIZER **/
#define VALID_TOKENS "<>()\"'*|"

enum e_type
{
    SCAN,
    WORD,
    IOFILE,
    COMMAND,
    PIPE = '|',
    INRDR = '<',
    ANDOP = '&',
    INDQTS = '"',
    OUTRDR = '>',
    INSQTS = '\'',
    ASETRIC = '*',
    OPARENTHSIS = '(',
    CPARENTHSIS = ')',
};

typedef struct s_token
{
    char *value;
    enum e_type type;
} t_token;

t_token *tokenizer(char *line);

/** PARSER **/
#define MAX_ARGS 128

typedef struct s_node
{
    enum e_type type;
    union
    {
        char *command[MAX_ARGS];
        char *value;
    } u_data;
    struct s_node *left;
    struct s_node *right;
} t_node;

t_node *parse_command(char *line);

#endif