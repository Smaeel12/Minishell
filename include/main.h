/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:03:17 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/26 09:41:28 by iboubkri         ###   ########.fr       */
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
typedef struct s_token
{
    char *value;
    enum e_token_type type;
} t_token;

/** PARSER **/
#define MAX_ARGS 128
#define MAX_REDIRECTIONS 80
enum e_node_type
{
    NODE_COMMAND,
    NODE_OPERATOR,
};
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
            size_t ridx;
            size_t aidx;
        } command;
    };
} t_tree;

/** TOKENIZATION FUNCTIONS */
enum e_errors tokenizer(char *line, t_list **lst);
static inline enum e_token_type advance(char c)
{
    if (ft_strchr(VALID_TOKENS, c))
        return (c);
    if (c && c != ' ')
        return (WORD);
    return (SCAN);
}

int last_status;

/** PARSING FUNCTIONS */
t_tree *parse_command(t_list **tokens);
t_tree *parse_redirection(t_list **tokens);
t_tree *parse_pipeline(t_list **tokens);

/** PARSER_HELPER_FUNCTIONS */
void clear_tree(t_tree *tree);
int is_word(t_token *token, size_t last_status);
char *expand_line(char *line, size_t last_status);
t_token *create_token(enum e_token_type type, char *line, size_t len);

enum e_errors
{
    OK,
    QTS_ERR,
};

#define INV_RDR_FILE "Invalid Redirection File the file should be a word"
#define INV_RDR "Invalid Redirection"
#define INV_PIPE "Incomplete/Invalid Pipe"

#endif