/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:00:29 by iboubkri          #+#    #+#             */
/*   Updated: 2025/07/01 01:55:39 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static int	open_heredoc(char *delim, bool mode)
{
	size_t	delim_len;
	char	*expanded;
	char	*line;
	int		fd;

	delim_len = ft_strlen(delim);
	fd = open(HEREDOC_FILE, O_TRUNC | O_WRONLY | O_CREAT, 0644);
	while (fd > 0)
	{
		expanded = NULL;
		line = readline("heredoc> ");
		if (!line || !ft_strncmp(line, delim, delim_len + 1))
			break ;
		expand_line(&expanded, line, (bool[]){mode, false}, 1);
		ft_putendl_fd(expanded, fd);
		free(expanded);
		free(line);
	}
	if (fd == -1)
		perror(HEREDOC_FILE);
	else if (!line)
		ft_putendl_fd(HEREDOC_EOF, 2);
	close(fd);
	return (0);
}

int	open_heredocs(struct s_heredoc *heredocs, size_t size)
{
	int		status;
	pid_t	pid;
	size_t	i;

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

int	open_redirections(char **rdrs, size_t size, int *fds)
{
	size_t	len;
	int		i;

	i = -1;
	while (++i < (int)size && rdrs[i])
	{
		len = ft_strlen(rdrs[i]);
		if (rdrs[i][0] == '<')
			close(fds[IN]);
		if (rdrs[i][0] == '>')
			close(fds[OUT]);
		if (rdrs[i][0] == '<' && len == 1)
			fds[IN] = open(rdrs[++i], O_RDONLY);
		if (rdrs[i][0] == '<' && len == 2 && rdrs[++i])
			fds[IN] = open(HEREDOC_FILE, O_RDONLY);
		if (rdrs[i][0] == '>' && len == 1)
			fds[OUT] = open(rdrs[++i], O_TRUNC | O_WRONLY | O_CREAT, 0644);
		if (rdrs[i][0] == '>' && len == 2)
			fds[OUT] = open(rdrs[++i], O_APPEND | O_WRONLY | O_CREAT, 0644);
		if (fds[OUT] == -1)
			return (perror(rdrs[i]), close(fds[IN]), -1);
		if (fds[IN] == -1)
			return (perror(rdrs[i]), close(fds[OUT]), -1);
	}
	return (0);
}
