/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:02:57 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/12 14:27:59 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int find_command(t_cmd *cmd, char **paths, t_cmd builtins[])
{
	char *cmd_path;
	size_t i;

	i = 0;
	cmd_path = ft_strdup(cmd->path);
	while (builtins[i].path && builtins[i].func)
	{
		if (!(ft_strncmp(cmd_path, builtins[i].path, ft_strlen(cmd_path))))
			return (cmd->path = cmd_path, cmd->func = builtins[i].func, 0);
		i++;
	}
	i = 0;
	if (cmd_path[0] == '/' || !ft_strncmp(cmd_path, "./", 2))
		return (cmd->path = cmd_path, cmd->func = NULL, 0);
	while (paths[i])
	{
		free(cmd_path);
		cmd_path = create_line((char *[]){ft_strdup(paths[i]), ft_strdup("/"), ft_strdup(cmd->path)}, 3);
		if (!access(cmd_path, X_OK | F_OK))
			return (cmd->path = cmd_path, cmd->func = NULL, 0);
		i++;
	}
	return (free(cmd_path), 1);
}

int open_streams(struct s_redirections *redirections, int *streams)
{
	size_t i;

	i = 0;
	while (redirections[i].filename)
	{
		close(streams[0]);
		close(streams[1]);
		if (redirections[i].type == INRDR)
			streams[0] = open(redirections[i].filename, O_RDONLY);
		if (redirections[i].type == OUTRDR)
			streams[1] = open(redirections[i].filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (redirections[i].type == APPEND)
			streams[1] = open(redirections[i].filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		i++;
	}
	return (0);
}

int execute_command(struct s_command command, char **paths, int *streams)
{
	extern char **environ;
	t_cmd cmd;
	pid_t pid;

	cmd = (t_cmd){command.arguments[0], NULL};
	open_streams(command.redirections, streams);
	find_command(&cmd, paths, (t_cmd[]){{"echo", echo}, {"cd", cd}, {"pwd", pwd}, {"export", export}, {"unset", unset}, {"env", env}, {"exit", bexit}, {NULL, NULL}});
	printf("%s\n", cmd.path);
	if (cmd.func)
		return (cmd.func(command.arguments, environ), 0);
	pid = fork();
	if (pid == -1)
		return (ft_putendl_fd("Can't create a Child", 2), 1);
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		dup2(streams[OUT], OUT);
		dup2(streams[IN], IN);
		close(streams[OUT]);
		close(streams[IN]);
		if (streams[UNUSED])
			close(streams[UNUSED]);
		execve(cmd.path, command.arguments, environ);
		return (perror(command.arguments[0]), free(cmd.path), exit(126), 0);
	}
	return (free(cmd.path), 0);
}

int execute_pipeline(t_tree *tree, char **paths, int *streams)
{
	int pipefds[2];

	if (!tree || !tree->type)
		return (0);
	if (tree->type == NODE_OPERATOR)
	{
		if (pipe(pipefds) == -1)
			return (1);
		execute_pipeline(tree->operator.left, paths,
						 (int[]){streams[0], pipefds[1], pipefds[0]});
		execute_pipeline(tree->operator.right, paths,
						 (int[]){pipefds[0], streams[1], pipefds[1]});
		close(pipefds[0]);
		close(pipefds[1]);
		wait(NULL);
		return (0);
	}
	execute_command(tree->command, paths, streams);
	return (0);
}
