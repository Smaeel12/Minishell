/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:03:17 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/27 14:23:56 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
#define MAIN_H

/** INCLUDES */

#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"
#include <sys/resource.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

/** DEFINITIONS */

#define INV_RDR_FILE "Invalid Redirection File the file should be a word"
#define INV_PIPE "Incomplete/Invalid Pipe"
#define INV_RDR "Invalid Redirection"
#define VALID_TOKENS "<>()\"'|"
#define MAX_REDIRECTIONS 80
#define MAX_ARGS 128

/** ENUMS **/

enum e_token_type
{
    SCAN,
    WORD,
    DQTS = '"',
    PIPE = '|',
    SQTS = '\'',
    INRDR = '<',
    ANDOP = '&',
    OUTRDR = '>',
    OPARENTHSIS = '(',
    CPARENTHSIS = ')',
};
enum e_node_type
{
    NODE_COMMAND,
    NODE_OPERATOR,
};
enum e_errors
{
    OK,
    QTS_ERR,
};

/** STRUCTS */

typedef struct s_token
{
    char *value;
    enum e_token_type type;
} t_token;
typedef struct s_tree
{
    enum e_node_type type;
    union
    {
        struct
        {
            char *value;
            struct s_tree *left;
            struct s_tree *right;
        } operator;
        struct
        {
            char *arguments[MAX_ARGS];
            char *redirections[MAX_REDIRECTIONS];
        } command;
    };
} t_tree;

/** PROTOTYPES */

enum e_errors tokenize_cmdline(t_list **lst, char *line);
t_tree *parse_pipeline(t_list *tokens, size_t last_status);
t_tree *parse_command(t_list **tokens, size_t last_status);

t_token *create_token(enum e_token_type type, char *line, size_t len);
char *expand_line(char *line, size_t last_status);
void clear_tree(t_tree *tree);
void clear_token(void *arg);

char *create_line(char **strs, size_t nstrs);

int execute_commands(t_tree *tree, char **paths, int *streams, int unused);

/** INLINE FUNCS */

static inline enum e_token_type advance(char c)
{
    if (ft_strchr(VALID_TOKENS, c))
        return (c);
    if (c && c != ' ')
        return (WORD);
    return (SCAN);
}

#endif