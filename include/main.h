/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:03:17 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/15 07:37:45 by iboubkri         ###   ########.fr       */
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
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

/** DEFINITIONS */
#define EXPECTED_VALID_FILENAME "Expected a valid filename for redirection"
#define INV_RDR_FILE "Invalid Redirection File the file should be a word"
#define MISSING_FILENAME "Missing filename for redirection"
#define MALLOC_FAILED "Memory allocation failed"
#define INV_PIPE "Incomplete/Invalid Pipe"
#define INV_RDR "Invalid Redirection"
#define QTS_ERR "Unclosed Quotes"
#define VALID_TOKENS "<>()\"'|"
#define MAX_REDIRECTIONS 80
#define MAX_ARGS 128

/** ENUMS **/
enum e_token_type
{
	SCAN,
	WORD,
	APPEND,
	HEREDOC,
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
	NONE,
	NODE_COMMAND,
	NODE_OPERATOR,
};

enum
{
	IN,
	OUT,
	UNUSED,
};

/** STRUCTS */
typedef struct s_cmd
{
	char *path;
	int (*func)(char **argv);
} t_cmd;

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
		struct s_command
		{
			char *arguments[MAX_ARGS];
			struct s_redirections
			{
				enum e_token_type type;
				char *filename;
			} redirections[MAX_REDIRECTIONS];
			size_t aidx;
			size_t ridx;
		} command;
	};
} t_tree;

struct s_global_data
{
	char **environs;
	int exit_status;
	size_t env_size;
};

/** VARIABLE */
struct s_global_data g_data;

/** PROTOTYPES */
t_tree *parse_line(void);

int execute_pipeline(t_tree *tree, char **paths, int *streams);

int add_token(t_list **lst, enum e_token_type state, enum e_token_type cstate, char *line, size_t len);
char *create_line(char **strs, size_t nstrs);
int is_valid_token(t_token *token);
char *expand_line(t_token *token);

void clear_token(void *arg);
int clear_tree(t_tree *tree);
int clear_paths(char **paths);
int clear_env(void);

int init_signals(void);

int export(char **argv);
int unset(char **argv);
int bexit(char **argv);
int echo(char **argv);
int env(char **argv);
int pwd(char **argv);
int cd(char **argv);

int set_env(char *key, char *value);
char *get_env(char *key);
int unset_env(char *key);
int print_env(void);
int init_env(void);

/// DEBUGG
void ft_puts(void *arg);
void print_tree(t_tree *tree);

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