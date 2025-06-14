/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:02:57 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/14 21:51:59 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int init_child(int *streams)
{
	dup2(streams[IN], IN);
	dup2(streams[OUT], OUT);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (streams[UNUSED] != -1)
		close(streams[UNUSED]);
	close(streams[OUT]);
	close(streams[IN]);
	return (0);
}

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
		cmd_path = create_line((char *[]){ft_strdup(paths[i]), ft_strdup("/"),
										  ft_strdup(cmd->path)},
							   3);
		if (!access(cmd_path, X_OK | F_OK))
			return (cmd->path = cmd_path, cmd->func = NULL, 0);
		i++;
	}
	return (free(cmd_path), 1);
}

int open_streams(struct s_redirections *redirections, int *streams)
{
	char *line;
	size_t i;

	i = 0;
	while (redirections[i].filename)
	{
		if (redirections[i].type == HEREDOC)
		{
			streams[IN] = open("temp_pipe_file", O_CREAT | O_WRONLY | O_TRUNC,
							   0644);
			while (true)
			{
				line = readline("heredoc> ");
				if (!line)
					break;
				ft_putendl_fd(line, streams[IN]);
			}
		}
		if (redirections[i].type == INRDR)
		{
			close(streams[IN]);
			streams[IN] = open(redirections[i].filename, O_RDONLY);
		}
		if (redirections[i].type == OUTRDR)
		{
			close(streams[OUT]);
			streams[OUT] = open(redirections[i].filename,
								O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}
		if (redirections[i].type == APPEND)
		{
			close(streams[OUT]);
			streams[OUT] = open(redirections[i].filename,
								O_CREAT | O_WRONLY | O_APPEND, 0644);
		}
		i++;
	}
	return (0);
}

int execute_command(struct s_command command, char **paths, int *streams)
{
	t_cmd cmd;
	pid_t pid;

	cmd = (t_cmd){command.arguments[0], NULL};
	open_streams(command.redirections, streams);
	find_command(&cmd, paths, (t_cmd[]){{"echo", echo}, {"cd", cd}, {"pwd", pwd}, {"export", export}, {"unset", unset}, {"env", env}, {"exit", bexit}, {NULL, NULL}});
	if (cmd.func == cd || cmd.func == export || cmd.func == unset || cmd.func == bexit)
		return (cmd.func(command.arguments), free(cmd.path), 0);
	pid = fork();
	if (pid == -1)
		return (ft_putendl_fd("Can't create a Child", 2), 1);
	if (pid == 0)
	{
		init_child(streams);
		if (cmd.func)
			return (cmd.func(command.arguments), free(cmd.path), exit(0), 0);
		execve(cmd.path, command.arguments, g_data.environs);
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
		execute_pipeline(tree->operator.left, paths, (int[]){streams[IN], pipefds[OUT], pipefds[IN]});
		execute_pipeline(tree->operator.right, paths, (int[]){pipefds[IN], streams[OUT], pipefds[OUT]});
		close(streams[OUT]);
		close(streams[IN]);
		close(pipefds[OUT]);
		close(pipefds[IN]);
		wait(NULL);
		return (0);
	}
	return (execute_command(tree->command, paths, streams), 0);
}
