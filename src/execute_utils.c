/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 01:00:00 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/18 11:20:08 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	wait_processes(void)
{
	int	last_pid;
	int	status;
	int	pid;

	pid = 0;
	last_pid = 0;
	while (true)
	{
		pid = wait(&status);
		if (pid == -1)
			break ;
		if (last_pid < pid)
		{
			s_data.exit_status = status;
			last_pid = pid;
		}
	}
	return (0);
}

int	find_command(t_cmd *cmd, t_cmd *builtins)
{
	size_t	path_len;
	size_t	cmd_len;
	char	*path;
	int		i;

	i = -1;
	cmd_len = ft_strlen(cmd->path);
	while (builtins[++i].path)
		if (!ft_strncmp(builtins[i].path, cmd->path, cmd_len + 1))
			return (cmd->func = builtins[i].func, 0);
	if (cmd->path[0] == '/' || !ft_strncmp(cmd->path, "./", 2))
		return (0);
	i = -1;
	while (cmd->path[0] && s_data.paths[++i])
	{
		path_len = ft_strlen(s_data.paths[i]);
		path = (char *)malloc((path_len + cmd_len + 2) * sizeof(char));
		ft_strlcpy(path, s_data.paths[i], path_len + 1);
		ft_strlcpy(path + path_len, "/", 2);
		ft_strlcpy(path + path_len + 1, cmd->path, cmd_len + 1);
		if (!access(path, F_OK | X_OK))
			return (free(cmd->path), cmd->path = path, 0);
		free(path);
	}
	return (ft_putstr_fd(cmd->path, 2), ft_putendl_fd(COMMAND_NOT_FOUND, 2), 1);
}

static int	open_heredoc(char *delem)
{
	size_t	delem_len;
	char	*line;
	int		fd;

	delem_len = ft_strlen(delem);
	fd = open(HEREDOC_FILE, O_TRUNC | O_WRONLY | O_CREAT, 0644);
	while (fd > 0)
	{
		line = readline("heredoc> ");
		if (!line)
			return (ft_putendl_fd(HEREDOC_ERROR, 2), close(fd), -1);
		if (!ft_strncmp(line, delem, delem_len + 1))
			break ;
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	fd = open(HEREDOC_FILE, O_RDONLY);
	return (fd);
}

int	open_redirections(struct s_redirections *rdrs, int *fds)
{
	size_t	i;

	i = 0;
	while (rdrs[i].file)
	{
		if (rdrs[i].type == HEREDOC || rdrs[i].type == INRDR)
			close(fds[IN]);
		if (rdrs[i].type == OUTRDR || rdrs[i].type == APPEND)
			close(fds[OUT]);
		if (rdrs[i].type == HEREDOC)
			fds[IN] = open_heredoc(rdrs[i].file);
		if (rdrs[i].type == INRDR)
			fds[IN] = open(rdrs[i].file, O_RDONLY);
		if (rdrs[i].type == OUTRDR)
			fds[OUT] = open(rdrs[i].file, O_TRUNC | O_WRONLY | O_CREAT, 0644);
		if (rdrs[i].type == APPEND)
			fds[OUT] = open(rdrs[i].file, O_APPEND | O_WRONLY | O_CREAT, 0644);
		if (fds[OUT] == -1)
			return (perror(rdrs[i].file), close(fds[IN]), -1);
		if (fds[IN] == -1)
			return (perror(rdrs[i].file), close(fds[OUT]), -1);
		i++;
	}
	return (0);
}
