/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:02:57 by iboubkri          #+#    #+#             */
/*   Updated: 2025/07/04 22:49:26 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	safe_close(int stream)
{
	if (stream == -1)
		return (-1);
	close(stream);
	return (0);
}

int	execute_builtin_command(t_cmd *cmd, char **args, int *streams)
{
	int	fd;

	free(cmd->path);
	if (streams[IN] == -1 || streams[OUT] == -1)
		return (g_data.st_exit = 1, 0);
	fd = open("/dev/stdout", O_TRUNC | O_WRONLY);
	dup2(streams[OUT], OUT);
	g_data.st_exit = cmd->func(args);
	dup2(fd, OUT);
	return (0);
}

int	execute_command(t_cmd *cmd, char **args, int *streams)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (streams[IN] == -1 || streams[OUT] == -1)
			return (free(cmd->path), clean_exit(1), 1);
		dup2(streams[OUT], OUT);
		dup2(streams[IN], IN);
		safe_close(streams[PIPEEND2]);
		safe_close(streams[PIPEEND1]);
		safe_close(streams[OUT]);
		safe_close(streams[IN]);
		if (cmd->func)
			return (free(cmd->path), clean_exit(cmd->func(args)), 0);
		check_command(cmd->path);
		execve(cmd->path, args, g_data.environs);
		perror(args[0]);
		clean_exit(1);
	}
	if (pid < 0)
		ft_putendl_fd(FORK_FAILED, 2);
	return (close(streams[IN]), close(streams[OUT]), free(cmd->path), 1);
}

int	execute_pipeline(t_tree *tree, int *streams, t_cmd *builtins)
{
	int		pipefds[2];
	t_cmd	cmd;

	if (!tree)
		return (0);
	if (tree->type == OPERATOR_NODE)
	{
		if (pipe(pipefds) == -1)
			return (ft_putendl_fd(CREATE_PIPE_ERROR, 2), 1);
		execute_pipeline(tree->s_operator.left, (int []){streams[IN],
			pipefds[OUT], pipefds[IN], streams[PIPEEND1]}, builtins);
		execute_pipeline(tree->s_operator.right, (int []){pipefds[IN],
			streams[OUT], pipefds[OUT], streams[PIPEEND1]}, builtins);
		return (close(pipefds[IN]), close(pipefds[OUT]), 0);
	}
	if (open_heredocs(tree->s_cmd.heredocs, tree->s_cmd.hidx) == -1
		|| open_redirections(tree->s_cmd.rdrs, tree->s_cmd.ridx, streams) == -1
		|| !tree->s_cmd.args[0])
		return (1);
	cmd = (t_cmd){ft_strdup(tree->s_cmd.args[0]), NULL};
	if (find_command(&cmd, builtins) && cmd.func && streams[PIPEEND1] == -1)
		execute_builtin_command(&cmd, tree->s_cmd.args, streams);
	else
		execute_command(&cmd, tree->s_cmd.args, streams);
	return (0);
}
