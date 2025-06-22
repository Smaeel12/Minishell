/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:09:10 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/22 11:28:01 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int add_token(t_list **lst, enum e_type state, char *line, size_t idx)
{
	t_token *token;

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

int append_string(char **result, char *segment)
{
	char *new_res;

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

int should_skip(char *line, size_t start, size_t end)
{
	if (line[start] == SQTS && line[end] != line[start])
		return (1);
	if (line[end - 1] == '$' && !(ft_isalpha(line[end]) || line[end] == '_' || line[end] == '?'))
		return (1);
	if (line[end - 1] != '$' && line[start] == DQTS && line[end] != line[start])
		return (1);
	if (line[end - 1] != '$' && line[end] && line[end] != DQTS && line[end] != SQTS)
		return (1);
	return (0);
}

int expand_line(char **result, char *line)
{
	char *key;
	size_t start;
	size_t end;
	size_t i;

	i = 0;
	start = 0;
	while (line[i++])
	{
		end = i;
		if (should_skip(line, start, end))
			continue;
		key = &line[i];
		start += (line[start] == DQTS || line[start] == SQTS);
		while (key[0] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		i += (key[0] && key[0] == '?');
		key = ft_substr(key, 0, (&line[i] - key));
		append_string(result, ft_substr(line, start, end - start - !!key[0]));
		append_string(result, get_env(key));
		i += (line[i] == DQTS || line[i] == SQTS);
		start = i;
	}
	return (0);
}
