/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:09:10 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/27 18:39:31 by iboubkri         ###   ########.fr       */
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

void create_line(char **result, char *line, char *value)
{
	char *new;
	size_t len;
	size_t idx;

	idx = 0;
	len = ft_strlen(line);
	if (value)
		len += ft_strlen(value);
	if (*result)
		len += ft_strlen(*result);
	new = (char *)malloc((len + 1) * sizeof(char));
	if (!new)
		return;
	if (*result)
		idx = ft_strlcpy(new, *result, ft_strlen(*result) + 1);
	if (line)
		idx += ft_strlcpy(new + idx, line, ft_strlen(line) + 1);
	if (value)
		idx += ft_strlcpy(new + idx, value, ft_strlen(value) + 1);
	*result = new;
}

char *expand_line(char *line, bool skip_mode, bool expand_mode)
{
	char *new;
	char state;
	size_t beg;
	size_t end;
	size_t i;

	i = 0;
	beg = 0;
	new = NULL;
	state = line[beg];
	while (line[i++])
	{
		if (state == SQTS && line[i] && line[i] != (char)state && skip_mode)
			continue;
		if ((line[i] != SQTS && line[i] != DQTS) && line[i - 1] == '$' &&
			line[i] && !(ft_isalpha(line[i]) || line[i] == '_' || line[i] == '?'))
			continue;
		if (line[i - 1] != '$' && line[i] && state == DQTS && line[i] != (char)state)
			continue;
		if ((line[i - 1] == '$' && !expand_mode) || (line[i] && line[i - 1] != '$' && line[i] != DQTS && line[i] != SQTS))
			continue;
		end = i;
		while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		i += (line[i] && line[i] == '?');
		beg += (state == SQTS || state == DQTS) && (line[beg] == state) && skip_mode;
		create_line(&new, ft_substr(line, beg, end - beg - (i > end)), get_env(ft_substr(line, end, i - end)));
		i += (state == SQTS || state == DQTS) && (line[i] == state) && skip_mode;
		if (line[i - 1] == state || (state != DQTS && state != SQTS && (line[i] == SQTS || line[i] == DQTS)))
			state = line[i];
		beg = i;
	}
	return (new);
}
