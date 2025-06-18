/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:03:17 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/18 11:21:16 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

/** INCLUDES */
# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <sys/wait.h>

/** DEFINITIONS */
# define HEREDOC_FILE ".tmp_heredoc_file.txt"
# define CREATE_PIPE_ERROR "Can't create a pipe"
# define MALLOC_FAILED "Memory allocation failed"
# define HEREDOC_ERROR "here-document delimited by end-of-file"
# define MISSING_FILE_ERROR "Syntax error: Missing filename"
# define RDR_ERROR "Syntax error: invalid redirection"
# define QUOTES_ERR "Syntax error: unclosed quotes"
# define PIPE_ERROR "Syntax error: invalid pipe"
# define FORK_FAILED "Creating a child failed"
# define COMMAND_NOT_FOUND ": command not found"
# define NO_FILE ": no such file or directiory"
# define TOKEN_CHARSET " \t\"'|<>"
# define MAX_REDIRECTIONS 128
# define MAX_ARGS 128

/** ENUMS **/
enum						e_node
{
	COMMAND_NODE,
	OPERATOR_NODE,
};

enum						e_type
{
	NONE,
	WORD,
	APPEND,
	HEREDOC,
	PIPE = '|',
	DQTS = '"',
	SQTS = '\'',
	INRDR = '<',
	OUTRDR = '>',
};

enum						e_streams
{
	IN,
	OUT,
	UNUSED,
};

/** STRUCTS */
typedef struct s_cmd
{
	char					*path;
	int						(*func)(char **argv);
}							t_cmd;

typedef struct s_token
{
	char					*value;
	enum e_type				type;
}							t_token;

typedef struct s_tree
{
	enum e_node				type;
	union
	{
		struct
		{
			char			*value;
			struct s_tree	*left;
			struct s_tree	*right;
		} s_operator;
		struct				s_command
		{
			struct			s_redirections
			{
				char		*file;
				enum e_type	type;
			} redirections[MAX_REDIRECTIONS];
			char			*arguments[MAX_ARGS];
			size_t			aidx;
			size_t			ridx;
		} s_command;
	};
}							t_tree;

struct
{
	char					**paths;
	char					**environs;
	int						exit_status;
	size_t					env_size;
}							s_data;

/** PROTOTYPES */
int							tokenize_cmdline(t_list **lst, char *line);
t_tree						*parse_pipeline(t_list *tokens);

int							execute_pipeline(t_tree *tree, int *streams);

int							set_env(char *key, char *value);
int							unset_env(char *key);
char						*get_env(char *key);
int							print_env(void);
int							init_env(void);

void						clear_token(void *arg);
int							clear_tree(t_tree *tree);
int							clear_array(char **array);

int							add_token(t_list **lst, enum e_type state,
								char *line, size_t idx);
char						*create_line(char **strs, size_t nstrs);
char						*concatenate_string(char *line);
int							expand_line(char **result, char *line);

int							wait_processes(void);
int							find_command(t_cmd *cmd, t_cmd *builtins);
int							open_redirections(struct s_redirections *rdrs,
								int *streams);

int							ft_export(char **argv);
int							ft_unset(char **argv);
int							ft_exit(char **argv);
int							ft_echo(char **argv);
int							ft_env(char **argv);
int							ft_pwd(char **argv);
int							ft_cd(char **argv);

int							init_signals(void);

static inline enum e_type	determine_token(char c)
{
	if (c == ' ' || c == '\t' || c == '\0')
		return (NONE);
	if (c == '<' || c == '>' || c == '|' || c == '\"' || c == '\'')
		return (c);
	return (WORD);
}

#endif