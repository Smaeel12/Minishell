/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:09:10 by iboubkri          #+#    #+#             */
/*   Updated: 2025/07/04 22:48:13 by iboubkri         ###   ########.fr       */
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

int	append_string(char **result, char *segment, size_t max, bool ifs_mode)
{
	char *(new), *(slice);
	size_t(len) = 0, (rdx) = 0, (i) = 0, (beg) = 0;
	if (!segment || !segment[0])
		return (free(segment), 0);
	len = ft_strlen(segment);
	while (i < len && rdx < max)
	{
		while (segment[i] && (!ifs_mode || !ft_strchr(g_data.ifs, segment[i])))
			i++;
		slice = ft_substr(segment, beg, i);
		if (!result[rdx])
			new = slice;
		else
		{
			new = ft_strjoin(result[rdx], slice);
			free(slice);
		}
		free(result[rdx]);
		result[rdx] = new;
		rdx += ifs_mode;
		beg = ++i;
	}
	return (free(segment), rdx);
}

int	should_continue(enum e_type state, bool modes[], char *line, size_t i)
{
	if (!line[i])
		return (0);
	if (modes[EXPAND] && (state != SQTS || !modes[SKIP]) && line[i - 1] == '$'
		&& (ft_isalpha(line[i]) || line[i] == '_' || line[i] == '?'))
		return (0);
	if (modes[SKIP] && (state == SQTS || state == DQTS)
		&& line[i] == (char)state)
		return (0);
	if (modes[SKIP] && (line[i] == SQTS || line[i] == DQTS) && (state != DQTS
			&& state != SQTS))
		return (0);
	return (1);
}

int	expand_line(char **result, char *line, bool modes[], size_t max)
{
	char (st) = line[0];
	size_t (i) = 0, (beg) = 0, (end) = 0, (idx) = 0;
	while (line[i++])
	{
		if (should_continue(st, modes, line, i))
			continue ;
		beg += (st == SQTS || st == DQTS) && (line[beg] == st) && modes[SKIP];
		end = i;
		while ((ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		i += (line[i] && line[i] == '?');
		idx += append_string(&result[idx], ft_substr(line, beg, end - beg
					- (i > end)), max, false);
		idx += append_string(&result[idx], get_env(ft_substr(line, end, i
						- end)), max, (st != SQTS && st != DQTS && max > 2));
		i += (st == SQTS || st == DQTS) && line[i] == st && modes[SKIP];
		if ((line[i] == st && modes[SKIP]) || (st != DQTS && st != SQTS
				&& (line[i] == SQTS || line[i] == DQTS)))
			st = line[++i];
		beg = i;
	}
	return (idx += !!result[idx], idx);
}
