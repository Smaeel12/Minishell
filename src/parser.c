/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:00:32 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/16 10:13:55 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static enum e_type	advance(char c)
{
	if (ft_strchr(VALID_TOKENS, c))
		return (c);
	if (c && c != ' ')
		return (WORD);
	return (SCAN);
}

t_token	*tokenizer(char *line)
{
	static char	*st_line;
	enum e_type	cstate;
	enum e_type	state;
	t_token		*token;
	size_t		i;

	if (line)
		st_line = line;
	while (st_line && *st_line && *st_line == ' ')
		st_line++;
	i = 0;
	cstate = SCAN;
	state = advance(st_line[i]);
	token = (t_token *)malloc(sizeof(t_token));
	while (st_line && st_line[i++])
	{
		while (st_line[++i] && (state == INDQTS || state == INSQTS)
			&& state != cstate)
			cstate = advance(st_line[i]);
		cstate = advance(st_line[i]);
		if (cstate != state || state == OPARENTHSIS || state == CPARENTHSIS)
			return (token->value = ft_substr(st_line, 0, i),
				token->type = state, st_line = &st_line[i], token);
	}
	return (free(token), NULL);
}
