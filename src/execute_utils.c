/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 01:00:00 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/17 12:48:13 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static int	open_heredoc(char *delemiter)
{
	size_t	delem_len;
	char	*line;
	int		fd;

	delem_len = ft_strlen(delemiter);
	fd = open(HEREDOC_FILE, O_TRUNC | O_CREAT | O_WRONLY);
	while (fd)
	{
		line = readline("heredoc> ");
		if (!line)
			return (ft_putendl_fd(HEREDOC_ERROR, 2), close(fd), -1);
		if (!ft_strncmp(line, delemiter, delem_len) && line[delem_len] == '\0')
			break ;
		ft_putendl_fd(line, fd);
	}
	return (0);
}

int	open_redirections(struct s_redirections *rdrs, int *streams)
{
	size_t	i;

	i = 0;
	while (rdrs[i].file)
	{
		if (rdrs[i].type == HEREDOC || rdrs[i].type == INRDR)
			close(streams[IN]);
		if (rdrs[i].type == OUTRDR || rdrs[i].type == APPEND)
			close(streams[OUT]);
		if (rdrs[i].type == HEREDOC)
			streams[IN] = open_heredoc(rdrs[i].file);
		if (rdrs[i].type == INRDR)
			streams[IN] = open(rdrs[i].file, O_RDONLY);
		if (rdrs[i].type == OUTRDR)
			streams[OUT] = open(rdrs[i].file, O_TRUNC | O_CREAT | O_WRONLY);
		if (rdrs[i].type == APPEND)
			streams[OUT] = open(rdrs[i].file, O_APPEND | O_CREAT | O_WRONLY);
		if (streams[OUT] == -1)
			return (perror(rdrs[i].file), close(streams[IN]), -1);
		if (streams[IN] == -1)
			return (perror(rdrs[i].file), close(streams[OUT]), -1);
		i++;
	}
	return (0);
}
