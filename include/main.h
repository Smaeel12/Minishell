/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:03:17 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/16 10:02:51 by iboubkri         ###   ########.fr       */
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

// typedef char *t_cmd;
// typedef char *t_path;

// #define CMD_NF ": command not found\n"
// #define PERM_IN ": Permission denied\n"

#define VALID_TOKENS "<>()\"'*|"

enum e_type
{
    SCAN,
    WORD,
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

#endif