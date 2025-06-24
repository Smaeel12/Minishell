/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:09:10 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/24 20:47:26 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	add_token(t_list **lst, enum e_type state, char *line, size_t idx)
{
	t_token	*token;

	if (state == NONE)
		return (0);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (1);
	token->type = state;
	token->value = ft_substr(line, 0, idx);
	ft_lstadd_back(lst, ft_lstnew(token));
	return (0);
}

int	append_string(char **result, char *segment)
{
	char	*new_res;

	if (!segment)
		return (0);
	if (*result)
	{
		new_res = ft_strjoin(*result, segment);
		free(*result);
		free(segment);
		segment = new_res;
	}
	*result = segment;
	return (0);
}

int	should_continue(enum e_type state, char *line, size_t i)
{
	if (state == SQTS && line[i] && line[i] != (char)state)
		return (1);
	if (line[i] != SQTS && line[i] != DQTS && line[i - 1] == '$' && line[i]
		&& !(ft_isalpha(line[i]) || line[i] == '_' || line[i] == '?'))
		return (1);
	if (line[i - 1] != '$' && line[i] && state == DQTS
		&& line[i] != (char)state)
		return (1);
	if (line[i - 1] != '$' && line[i] && line[i] != DQTS && line[i] != SQTS)
		return (1);
	return (0);
}

void	expand_line(char **result, char *line)
{
	size_t	beg;
	size_t	end;
	size_t	i;
	char	state;

	i = 0;
	beg = 0;
	state = line[beg];
	while (line[i++])
	{
		if (should_continue(state, line, i))
			continue ;
		end = i;
		while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		i += (line[i] && line[i] == '?');
		beg += (state == SQTS || state == DQTS) && (line[beg] == state);
		append_string(result, ft_substr(line, beg, end - beg - (i > end)));
		append_string(result, get_env(ft_substr(line, end, i - end)));
		i += (state == SQTS || state == DQTS) && (line[i] == state);
		if (line[i - 1] == state || (state != DQTS && state != SQTS
				&& (line[i] == SQTS || line[i] == DQTS)))
			state = line[i];
		beg = i;
	}
}

char	*create_path_line(char *env_path, char *cmd, size_t cmd_len)
{
	size_t	path_len;
	char	*path;

	path_len = ft_strlen(env_path);
	path = (char *)malloc((path_len + cmd_len + 2) * sizeof(char));
	ft_strlcpy(path, env_path, path_len + 1);
	ft_strlcpy(path + path_len, "/", 2);
	ft_strlcpy(path + path_len + 1, cmd, cmd_len + 1);
	return (path);
}
