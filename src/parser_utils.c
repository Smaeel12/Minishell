/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:09:10 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/20 18:57:45 by iboubkri         ###   ########.fr       */
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

int concatenate_string(char **result, char *key, char *segment)
{
	size_t seg_len;
	size_t val_len;
	char *value;
	char *new;

	val_len = 0;
	value = get_env(key);
	seg_len = ft_strlen(segment);
	if (value)
		val_len = ft_strlen(value);
	new = malloc((seg_len + val_len + 1) * sizeof(char));
	if (!new)
		return (1);
	ft_strlcpy(new, segment, seg_len + 1);
	if (value)
	{
		ft_strlcpy(new + seg_len, value, val_len + 1);
		free(value);
	}
	free(segment);
	*result = new;
	return (0);
}

void expand_line(char **result, char *line)
{
	size_t start;
	size_t end;
	size_t i;

	i = 0;
	start = 0;
	while (line[i++])
	{
		if (line[i - 1] == '$' || !line[i] || line[i] == SQTS || line[i] == DQTS)
		{
			end = i;
			if ((end <= start) || (line[i - 1] != '$' && (line[start] == SQTS || line[start] == DQTS) && line[start] != line[i]) || (line[i - 1] == '$' && (line[start] == SQTS || !(ft_isalpha(line[i]) || line[i] == '_' || line[i] == '?'))))
				continue;
			start += line[start] == DQTS || line[start] == SQTS;
			while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
				i++;
			concatenate_string(result, ft_substr(line, end, i - end), ft_substr(line, start, end - start - (line[end - 1] == '$')));
			start = i + (line[start] == DQTS || line[start] == SQTS);
		}
	}
}
