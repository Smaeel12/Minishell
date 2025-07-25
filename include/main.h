/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:03:17 by iboubkri          #+#    #+#             */
/*   Updated: 2025/07/04 22:43:06 by iboubkri         ###   ########.fr       */
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
# include <sys/stat.h>
# include <sys/wait.h>

/** DEFINITIONS */
# define HEREDOC_FILE ".tmp_heredoc_file.txt"
# define HEREDOC_EOF "here-document delimited by end-of-file"

# define NO_FILE ": No such file or directiory"
# define NO_PERM ": Permission denied"
# define NO_CMD ": command not found"
# define DIR_CMD ": Is a directory"

# define MAX_HEREDOCS_ERROR "maximum here-document count exceeded"
# define MISSING_DELIM "Syntax error: Missing here_doc delimiter"
# define MISSING_FILENAME "Syntax error: Missing filename"
# define UNCLOS_QUOTES "Syntax error: Unclosed quotes"
# define RDR_INVALID "Syntax error: Invalid redirection"
# define PIPE_INVALID "Syntax error: Invalid pipe"
# define CREATE_PIPE_ERROR "Can't create a pipe"
# define MALLOC_FAILED "Memory allocation failed"
# define FORK_FAILED "Creating a child failed"

# define TOKEN_CHARSET " \t\"'|<>()"
# define MAX_REDIRECTIONS 512
# define MAX_HEREDOCS 17
# define MAX_ARGS 512

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
	ANDOP = '&',
	INRDR = '<',
	OUTRDR = '>',
	OPENED = '(',
	CLOSED = ')',
};

enum						e_streams
{
	IN,
	OUT,
	PIPEEND1,
	PIPEEND2,
};

enum
{
	EXPAND,
	SKIP,
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
			struct			s_heredoc
			{
				char		*delim;
				bool		mode;
			} heredocs[MAX_REDIRECTIONS];
			char			*rdrs[MAX_REDIRECTIONS];
			char			*args[MAX_ARGS];
			size_t			aidx;
			size_t			ridx;
			size_t			hidx;
		} s_cmd;
	};
}							t_tree;

struct						s_global_data
{
	t_tree					*tree;
	char					**paths;
	t_list					*tokens;
	char					**environs;
	int						st_exit;
	size_t					env_size;
	char					*ifs;
};
extern struct s_global_data	g_data;

/** PROTOTYPES */
int							expand_line(char **result, char *line, bool modes[],
								size_t max);

int							tokenize_cmdline(t_list **lst, char *line);
t_tree						*parse_pipeline(t_list *tokens, int *error);

int							execute_pipeline(t_tree *tree, int *streams,
								t_cmd *builtins);

int							set_env(char *new_var);
int							unset_env(char *key);
char						*get_env(char *key);
int							print_env(void);

void						clean_exit(int status);
void						clear_token(void *arg);
int							clear_tree(t_tree *tree);
int							clear_array(char **array);

int							add_token(t_list **lst, enum e_type state,
								char *line, size_t idx);

int							wait_processes(void);
int							find_command(t_cmd *cmd, t_cmd *builtins);
int							check_command(char *path);
char						*create_path_line(char *env_path, char *cmd,
								size_t cmd_len);

int							open_heredocs(struct s_heredoc *heredocs,
								size_t size);
int							open_redirections(char **rdrs, size_t size,
								int *fds);

int							ft_export(char **argv);
int							ft_unset(char **argv);
int							ft_exit(char **argv);
int							ft_echo(char **argv);
int							ft_env(char **argv);
int							ft_pwd(char **argv);
int							ft_cd(char **argv);

int							init_signals(void);
int							init_paths(char *path_line);
int							init_ifs(char *ifs);
int							init_env(char **environs, size_t size);
t_cmd						*init_shell(void);

void						print_err(char *cmd, char *arg, char *msg);

void						sigint_handler(int segnum);

static inline enum e_type	determine_token(char c)
{
	if (c == ' ' || c == '\t' || c == '\0')
		return (NONE);
	if (c == '<' || c == '>' || c == '|' || c == '\"' || c == '\'' || c == '&'
		|| c == '(' || c == ')')
		return (c);
	return (WORD);
}

#endif