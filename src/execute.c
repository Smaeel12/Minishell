/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:02:57 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/22 22:44:07 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	execute_command(t_cmd *cmd, t_cmd *builtins, char **arguments, int *streams)
{
	pid_t	pid;

	find_command(cmd, builtins);
	if (cmd->func && streams[UNUSED] == -1)
		return (g_data.exit_status = cmd->func(arguments) << 8, free(cmd->path),
			0);
	pid = fork();
	if (pid < 0)
		return (free(cmd->path), ft_putendl_fd(FORK_FAILED, 2), 1);
	if (pid > 0)
		return (free(cmd->path), close(streams[IN]), close(streams[OUT]), 1);
	if (streams[UNUSED] != -1)
		close(streams[UNUSED]);
	dup2(streams[OUT], OUT);
	dup2(streams[IN], IN);
	close(streams[OUT]);
	close(streams[IN]);
	if (cmd->func)
		return (free(cmd->path), exit(cmd->func(arguments)), 0);
	if (cmd->path[0] == '\0' || !(cmd->path[0] == '/' || !ft_strncmp(cmd->path,
				"./", 2)))
		return (free(cmd->path), exit(127), 1);
	execve(cmd->path, arguments, g_data.environs);
	perror(arguments[0]);
	return (free(cmd->path), exit(127 - !(errno == ENONET)), 1);
}

int	execute_pipeline(t_tree *tree, int *streams)
{
	int		pipefds[2];
	t_cmd	cmd;

	if (!tree)
		return (0);
	if (tree->type == OPERATOR_NODE)
	{
		if (pipe(pipefds) == -1)
			return (ft_putendl_fd(CREATE_PIPE_ERROR, 2), 1);
		execute_pipeline(tree->s_operator.left,
			(int []){streams[IN], pipefds[OUT], pipefds[IN]});
		execute_pipeline(tree->s_operator.right,
			(int []){pipefds[IN], streams[OUT], pipefds[OUT]});
		return (0);
	}
	if (open_redirections(tree->s_command.redirections, streams) == -1)
		return (g_data.exit_status = 1 << 8, 1);
	cmd = (t_cmd){ft_strdup(tree->s_command.arguments[0]), NULL};
	execute_command(&cmd, (t_cmd[]){{"echo", ft_echo}, {"cd", ft_cd}, {"pwd",
		ft_pwd}, {"export", ft_export}, {"unset", ft_unset}, {"env", ft_env},
	{"exit", ft_exit}, {NULL, NULL}}, tree->s_command.arguments, streams);
	return (0);
}
