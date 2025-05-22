/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:03:17 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/22 09:39:01 by iboubkri         ###   ########.fr       */
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
#define VALID_TOKENS "<>()\"'|"

enum e_TOKENType
{
    SCAN,
    WORD,
    PIPE = '|',
    INRDR = '<',
    ANDOP = '&',
    INDQTS = '"',
    OUTRDR = '>',
    INSQTS = '\'',
    OPARENTHSIS = '(',
    CPARENTHSIS = ')',
};

typedef struct s_token
{
    char *value;
    enum e_TOKENType type;
} t_token;

void tokenizer(char *line, t_list **lst);
static inline enum e_TOKENType advance(char c)
{
    if (ft_strchr(VALID_TOKENS, c))
        return (c);
    if (c && c != ' ')
        return (WORD);
    return (SCAN);
}

/** PARSER **/
#define MAX_ARGS 128
#define MAX_REDIRECTIONS 80

enum ASTNodeType
{
    NODE_COMMAND_LINE,
    NODE_COMMAND,
    NODE_OPERATOR,
    NODE_SIMPLE_COMMAND,
    NODE_REDIRECTION,
    NODE_WORD,
    NODE_IDENTIFIER,
    NODE_FILE
};

typedef struct s_ASTNode
{
    enum ASTNodeType type;
    union
    {
        struct
        {
            char *value;
            struct s_node *left;
            struct s_node *right;
        } operator;
        struct
        {
            char *args[MAX_ARGS];
            char *redirections[MAX_REDIRECTIONS];
        } command;
    };
} t_ASTNode;
t_ASTNode *parse_command(t_list **tokens);
t_ASTNode *parse_redirection(t_list **tokens);

#endif