/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:02:57 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/28 18:28:05 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

char *find_command(char *line, char **paths)
{
	char *command;
	size_t i;

	i = 0;
	command = ft_strdup(line);
	while (paths[i])
	{
		if (!access(command, X_OK) && command[0] == '/')
			return (command);
		free(command);
		command = create_line((char *[]){paths[i], "/", line}, 3);
		i++;
	}
	return (free(command), NULL);
}

int open_streams(int *streams, char **redirections)
{
	size_t i;

	i = 0;
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
	close(streams[0]);
	close(streams[1]);
	return (0);
}

int execute_commands(t_tree *tree, char **paths, int *streams, int unused)
{
	extern char **environ;
	int pipefds[2];
	char *command;
	pid_t pid;

	if (!tree || !tree->type)
		return (0);
	if (tree->type == NODE_OPERATOR)
	{
		if (pipe(pipefds) == -1)
			return (1);
		execute_commands(tree->operator.left, paths, (int[]){streams[0], pipefds[1]}, pipefds[0]);
		execute_commands(tree->operator.right, paths, (int[]){pipefds[0], streams[1]}, pipefds[1]);
		close(streams[0]);
		close(streams[1]);
		close(pipefds[0]);
		close(pipefds[1]);
		wait(NULL);
		wait(NULL);
		return (0);
	}
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		command = find_command(tree->command.arguments[0], paths);
		open_streams(streams, tree->command.redirections);
		if (unused != -1)
			close(unused);
		if (!command)
		{
			ft_putstr_fd(tree->command.arguments[0], 2);
			ft_putendl_fd(": Command not found", 2);
			exit(127);
		}
		execve(command, tree->command.arguments, environ);
		perror(tree->command.arguments[0]);
		free(command);
		exit(1);
	}
	return (0);
}
