/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:09:10 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/29 02:23:37 by iboubkri         ###   ########.fr       */
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

char *ft_strjoin_helper(char *s1, char *s2)
{
	char *new;

	if (!s1)
		new = ft_strdup(s2);
	else if (!s2)
		new = ft_strdup(s1);
	else
		new = ft_strjoin(s1, s2);
	return (free(s1), free(s2), new);
}

int should_continue(enum e_type state, char *line, size_t expand_mode)
{
	if (!*line)
		return (0);
	if ((state == SQTS || state == DQTS) && *line == (char)state)
		return (0);
	if (*(line - 1) == '$' &&
		(ft_isalpha(*line) || *line == '_' || *line == '?'))
		return (0);
	return (1);
}

char **expand_line(char *line, bool skip_mode, bool expand_mode)
{
	char **new;
	char *value;
	char **splitted;
	char state;
	size_t beg;
	size_t end;
	size_t i;
	size_t instr;
	size_t ivals;

	i = 0;
	beg = 0;
	instr = 0;
	state = line[beg];
	new = (char **)malloc((MAX_ARGS / 2) * sizeof(char *));
	ft_bzero(new, (MAX_ARGS / 2) * sizeof(char *));
	while (line[i++])
	{
		if (should_continue(state, line, expand_mode))
			continue;
		end = i;
		while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		i += (line[i] && line[i] == '?');
		beg += (state == SQTS || state == DQTS) && (line[beg] == state) && skip_mode;
		new[instr] = ft_strjoin_helper(new[instr], ft_substr(line, beg, end - beg - (i > end)));
		value = get_env(ft_substr(line, end, i - end));
		if (state != SQTS && state != DQTS)
		{
			ivals = 0;
			splitted = ft_split(value, ' ');
			while (splitted && splitted[ivals])
			{
				new[instr] = ft_strjoin_helper(new[instr], splitted[ivals]);
				ivals++;
				instr++;
			}
			free(splitted);
			free(value);
		}
		else
			new[instr] = ft_strjoin_helper(new[instr], value);
		i += (state == SQTS || state == DQTS) && (line[i] == state) && skip_mode;
		if (line[i - 1] == state || (state != DQTS && state != SQTS && (line[i] == SQTS || line[i] == DQTS)))
			state = line[i];
		beg = i;
	}
	return (new);
}
