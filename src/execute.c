/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:02:57 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/03 20:45:33 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

char	*find_command(char *line, char **paths)
{
	char	*command;
	size_t	i;

	i = 0;
	command = ft_strdup(line);
	if (command[0] == '/' || !ft_strncmp(command, "./", 2))
		return (command);
	while (paths[i])
	{
		command = create_line((char *[]){ft_strdup(paths[i]), ft_strdup("/"),
				ft_strdup(line)}, 3);
		if (!access(command, X_OK | F_OK))
			return (command);
		free(command);
		i++;
	}
	ft_putstr_fd(line, 2);
	ft_putendl_fd(": Command not found", 2);
	exit(127);
}

int	open_streams(int *streams, char **redirections, int unused)
{
	size_t	i;

	i = 0;
	if (unused != -1)
		close(unused);
	dup2(streams[0], 0);
	dup2(streams[1], 1);
	while (redirections[i])
	{
		close(streams[0]);
		close(streams[1]);
		if (redirections[i][0] == '<' && redirections[i][1] == '\0')
			streams[0] = open(redirections[++i], O_RDONLY);
		if (redirections[i][0] == '>' && redirections[i][1] == '\0')
			streams[1] = open(redirections[++i], O_CREAT | O_WRONLY | O_TRUNC,
					0644);
		if (redirections[i][0] == '>' && redirections[i][1] == '>')
			streams[1] = open(redirections[++i], O_CREAT | O_WRONLY | O_APPEND,
					0644);
		dup2(streams[0], 0);
		dup2(streams[1], 1);
		i++;
	}
	return (close(streams[0]), close(streams[1]), 0);
}

int	execute_command(char **command, char **paths)
{
	extern char	**environ;
	t_cmd		*builtins;
	char		*cmd_path;
	size_t		i;

	i = 0;
	cmd_path = NULL;
	builtins = (t_cmd[]){{"echo", echo}, {"cd", cd}, {"pwd", pwd}, {"export",
		export}, {"unset", unset}, {"env", env}, {"exit", bexit}, {NULL, NULL}};
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	while (builtins[i].name && builtins[i].func)
	{
		if (!(ft_strncmp(command[0], builtins[i].name, ft_strlen(command[0]))))
			builtins[i].func(command, environ);
		i++;
	}
	cmd_path = find_command(command[0], paths);
	execve(cmd_path, command, environ);
	perror(command[0]);
	free(cmd_path);
	if (errno == ENOENT)
		exit(127);
	exit(126);
}

int	execute_pipeline(t_tree *tree, char **paths, int *streams, int unused)
{
	int		pipefds[2];
	pid_t	pid;

	if (!tree || !tree->type)
		return (0);
	if (tree->type == NODE_OPERATOR)
	{
		if (pipe(pipefds) == -1)
			return (1);
		execute_pipeline(tree->operator.left, paths, (int[]){streams[0],
			pipefds[1]}, pipefds[0]);
		execute_pipeline(tree->operator.right, paths, (int[]){pipefds[0],
			streams[1]}, pipefds[1]);
		return (close(streams[0]), close(streams[1]), close(pipefds[0]),
			close(pipefds[1]), wait(NULL), wait(NULL), 0);
	}
	pid = fork();
	if (pid == -1)
		return (ft_putendl_fd("Can't create a Child", 2), 1);
	if (pid == 0)
	{
		open_streams(streams, tree->command.redirections, unused);
		execute_command(tree->command.arguments, paths);
	}
	return (0);
}
