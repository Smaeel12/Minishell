/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:03:17 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/01 20:08:25 by iboubkri         ###   ########.fr       */
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

/** STRUCTS */
typedef struct s_cmd
{
	char *name;
	void (*func)(char **argv, char **envp);
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
		struct
		{
			char *arguments[MAX_ARGS];
			char *redirections[MAX_REDIRECTIONS];
			size_t aidx;
			size_t ridx;
		} command;
	};
} t_tree;

/** PROTOTYPES */

int tokenize_cmdline(t_list **lst, char *line);
t_tree *parse_pipeline(t_list *tokens, size_t last_status);

int execute_pipeline(t_tree *tree, char **paths, int *streams, int unused);
int open_streams(int *streams, char **redirections, int unused);
int execute_command(char **command, char **paths);
char *find_command(char *line, char **paths);

t_token *create_token(enum e_token_type type, char *line, size_t len);
char *expand_line(t_token *token, size_t last_status);
char *create_line(char **strs, size_t nstrs);

void clear_token(void *arg);
void clear_tree(t_tree *tree);
int clear_paths(char **paths);

void init_signals(void);

void cd(char **argv, char **envp);
void env(char **argv, char **envp);
void pwd(char **argv, char **envp);
void echo(char **argv, char **envp);
void unset(char **argv, char **envp);
void bexit(char **argv, char **envp);
void export(char **argv, char **envp);

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