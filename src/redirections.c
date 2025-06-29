/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:00:29 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/27 23:35:12 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static int open_heredoc(char *delim, bool mode)
{
	size_t delim_len;
	char *line;
	int fd;

	delim_len = ft_strlen(delim);
	fd = open(HEREDOC_FILE, O_TRUNC | O_WRONLY | O_CREAT, 0644);
	while (fd > 0)
	{
		line = readline("heredoc> ");
		if (!line || !ft_strncmp(line, delim, delim_len + 1))
			break;
		char **strs = expand_line(line, true, mode);
		ft_putendl_fd(strs[0], fd);
		free(strs);
		free(line);
	}
	if (fd == -1)
		perror(HEREDOC_FILE);
	else if (!line)
		ft_putendl_fd(HEREDOC_EOF, 2);
	close(fd);
	free(delim);
	return (0);
}

int open_heredocs(struct s_heredoc *heredocs, size_t size)
{
	int status;
	pid_t pid;
	size_t i;

	i = 0;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, clean_exit);
		while (i < size)
		{
			open_heredoc(heredocs[i].delim, heredocs[i].mode);
			i++;
		}
		clean_exit(0);
	}
	if (pid < 0)
		ft_putendl_fd(FORK_FAILED, 2);
	wait(&status);
	if (WTERMSIG(status))
	{
		unlink(HEREDOC_FILE);
		return (-1);
	}
	return (0);
}

int open_redirections(struct s_redirections *rdrs, size_t size, int *fds)
{
	size_t i;

	i = 0;
	while (i < size)
	{
		if (rdrs[i].type == HEREDOC || rdrs[i].type == INRDR)
			close(fds[IN]);
		if (rdrs[i].type == OUTRDR || rdrs[i].type == APPEND)
			close(fds[OUT]);
		if (rdrs[i].type == HEREDOC)
			fds[IN] = open(HEREDOC_FILE, O_RDONLY);
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
