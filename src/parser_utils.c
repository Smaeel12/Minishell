/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:09:10 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/19 14:12:01 by iboubkri         ###   ########.fr       */
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

char *ft_strjoin_helper(char *s1, char *s2)
{
	char *result;

	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
	result = ft_strjoin(s1, s2);
	return (free(s1), free(s2), result);
}

char *expand_line(char *line)
{
	size_t i = 0;
	size_t start = 0;
	size_t shift = 0;
	char *key = NULL;
	char *result = NULL;

	while (line[i++])
	{
		shift = 0;

		if (line[start] != SQTS && line[i - 1] == '$' &&
			(ft_isalpha(line[i]) || line[i] == '_' || line[i] == '?'))
		{
			key = &line[i];
			shift = line[start] == DQTS || line[start] == SQTS;
			result = ft_strjoin_helper(result,
									   ft_substr(line, start + shift, i - start - 1));
			while (line[++i] && (ft_isalnum(line[i]) || line[i] == '_'))
				;
			key = ft_substr(key, 0, (&line[i] - key));
			result = ft_strjoin_helper(result, get_env(key));
			start = i + shift;
		}
		if (line[i] == '\0' || line[i] == SQTS || line[i] == DQTS)
		{
			if ((line[i] == '\0' && i <= start) || line[start] != line[i])
				continue;
			shift = line[start] == DQTS || line[start] == SQTS;
			result = ft_strjoin_helper(result,
									   ft_substr(line, start + shift, (i - start - shift)));
			start = i + shift;
		}
	}
	return (0);
}
