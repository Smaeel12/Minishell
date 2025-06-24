/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 01:00:00 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/24 21:17:14 by iboubkri         ###   ########.fr       */
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
			g_data.exit_status = status;
			last_pid = pid;
		}
	}
	return (0);
}

int	check_command(char *path)
{
	struct stat	cmd_stat;

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

int	find_command(t_cmd *cmd)
{
	t_cmd	*builtins;
	size_t	cmd_len;
	char	*path;
	int		i;

	builtins = (t_cmd[]){{"echo", ft_echo}, {"cd", ft_cd}, {"pwd", ft_pwd},
	{"export", ft_export}, {"unset", ft_unset}, {"env", ft_env}, {"exit",
		ft_exit}, {NULL, NULL}};
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
		path = create_path_line(g_data.paths[++i], cmd->path, cmd_len);
		if (!access(path, F_OK | X_OK))
			return (free(cmd->path), cmd->path = path, 0);
		free(path);
	}
	return (0);
}

static int	open_heredoc(char *delem)
{
	size_t	delem_len;
	char	*line;
	int		fd;

	delem_len = ft_strlen(delem);
	fd = open(HEREDOC_FILE, O_APPEND | O_WRONLY | O_CREAT, 0644);
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
	return (close(fd), open(HEREDOC_FILE, O_RDONLY));
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
