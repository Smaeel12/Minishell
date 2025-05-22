/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oldparser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:59:45 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/18 16:22:16 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// int print_token(char *line, size_t start, size_t end)
// {
//     while (start < end)
//         putc(line[start++], stdout);
//     putc('\n', stdout);
//     return (0);
// }

// void tokenizer(char *line, t_list **tokens, enum e_tokentype *state)
// {
// 	enum e_tokentype cstate;
// 	size_t s;
// 	size_t i;

// 	i = 0;
// 	s = 0;
// 	while (1)
// 	{
// 		cstate = advance(line[i]);
// 		while ((*state == INDQTS || *state == INSQTS) && *state != cstate
// && line[i])
// 			cstate = advance(line[i++]);
// 		if ((*state == INDQTS || *state == INSQTS) && *state == cstate)
// 			cstate = advance(line[i]);
// 		if (cstate != *state || *state == OPEN_PARENT || *state == CLOSE_PARENT)
// 		{
// 			if (*state != SCAN)
// 				ft_lstadd_back(tokens, ft_lstnew(ft_substr(line, s, i - s)));
// 			*state = cstate;
// 			s = i;
// 		}
// 		if (!line[i])
// 			break ;
// 		i++;
// 	}
// }

// t_token *tokenizer(char *line)
// {
// 	static char *st_line;
// 	enum e_type cstate;
// 	enum e_type state;
// 	t_token *token;
// 	size_t i;

// 	if (line)
// 		st_line = line;
// 	while (st_line && *st_line && *st_line == ' ')
// 		st_line++;
// 	i = 0;
// 	cstate = SCAN;
// 	state = advance(st_line[i]);
// 	token = (t_token *)malloc(sizeof(t_token));
// 	while (token && st_line && st_line[i++])
// 	{
// 		while (st_line[i] && (state == INDQTS || state == INSQTS) && state != cstate)
// 			cstate = advance(st_line[i++]);
// 		cstate = advance(st_line[i]);
// 		if (cstate != state || state == OPARENTHSIS || state == CPARENTHSIS)
// 			return (token->value = ft_substr(st_line, 0, i),
// 					token->type = state, st_line = &st_line[i], token);
// 	}
// 	return (free(token), NULL);
// }
