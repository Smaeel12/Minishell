/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 01:00:00 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/26 16:36:14 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int wait_processes(void)
{
	int last_pid;
	int status;
	int pid;

	pid = 0;
	last_pid = 0;
	while (true)
	{
		pid = wait(&status);
		if (pid == -1)
			break;
		if (last_pid < pid)
		{
			g_data.exit_status = WEXITSTATUS(status);
			if (WIFSIGNALED(status))
			{
				g_data.exit_status = WTERMSIG(status) + 128;
				printf("\n");
			}
			last_pid = pid;
		}
	}
	return (0);
}

int check_command(char *path)
{
	struct stat cmd_stat;

	if (!path[0] || !(path[0] == '/' || !ft_strncmp(path, "./", 2)))
		return (ft_putstr_fd(path, 2), ft_putendl_fd(NO_CMD, 2), free(path),
				clean_exit(127), 1);
	if (stat(path, &cmd_stat) == -1)
	{
		perror("ERROR");
		if (errno == EACCES)
			return (free(path), clean_exit(126), 1);
		return (free(path), clean_exit(127), 1);
	}
	if (S_ISDIR(cmd_stat.st_mode))
		return (ft_putstr_fd(path, 2), ft_putendl_fd(DIR_CMD, 2), free(path),
				clean_exit(126), 1);
	if (!(cmd_stat.st_mode & S_IXUSR))
		return (ft_putstr_fd(path, 2), ft_putendl_fd(NO_PERM, 2), free(path),
				clean_exit(126), 1);
	return (0);
}

int find_command(t_cmd *cmd)
{
	t_cmd *builtins;
	size_t path_len;
	size_t cmd_len;
	char *path;
	int i;

	builtins = (t_cmd[]){{"echo", ft_echo}, {"cd", ft_cd}, {"pwd", ft_pwd}, {"export", ft_export}, {"unset", ft_unset}, {"env", ft_env}, {"exit", ft_exit}, {NULL, NULL}};
	if (cmd->path[0] == '/' || !ft_strncmp(cmd->path, "./", 2))
		return (0);
	i = -1;
	cmd_len = ft_strlen(cmd->path);
	while (builtins[++i].path)
		if (!ft_strncmp(builtins[i].path, cmd->path, cmd_len + 1))
			return (cmd->func = builtins[i].func, 0);
	i = -1;
	while (cmd->path[0] && g_data.paths[++i])
	{
		path_len = ft_strlen(g_data.paths[i]);
		path = (char *)malloc((path_len + cmd_len + 2) * sizeof(char));
		ft_strlcpy(path, g_data.paths[i], path_len + 1);
		ft_strlcpy(path + path_len, "/", 2);
		ft_strlcpy(path + path_len + 1, cmd->path, cmd_len + 1);
		if (!access(path, F_OK | X_OK))
			return (free(cmd->path), cmd->path = path, 0);
		free(path);
	}
	return (0);
}
