/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:00:32 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/22 10:26:01 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

t_token *ft_newtoken(enum e_TOKENType type, char *line, size_t len)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_substr(line, 0, len);
	return (token);
}

void tokenizer(char *line, t_list **lst)
{
	enum e_TOKENType cstate;
	enum e_TOKENType state;
	size_t i;

	i = 0;
	cstate = SCAN;
	state = advance(line[i]);
	while (line[i++])
	{
		cstate = advance(line[i]);
		while (line[i] && (state == INDQTS || state == INSQTS) && state != cstate)
			cstate = advance(line[i++]);
		cstate = advance(line[i]);
		if (state != cstate)
		{
			if (state != SCAN)
				ft_lstadd_back(lst, ft_lstnew(ft_newtoken(state, line, i)));
			state = cstate;
			line = &line[i];
			i = 0;
		}
	}
}

t_ASTNode *parse_command(t_list **tokens)
{
	t_ASTNode *command;
	size_t rdr_carg;
	size_t val_carg;

	rdr_carg = 0;
	val_carg = 0;
	command = (t_ASTNode *)malloc(sizeof(t_ASTNode));
	ft_bzero(command, sizeof(t_ASTNode));
	while ((*tokens) && (((t_token *)(*tokens)->content)->type == INRDR ||
						 ((t_token *)(*tokens)->content)->type == OUTRDR ||
						 ((t_token *)(*tokens)->content)->type == WORD))
	{
		if (((t_token *)(*tokens)->content)->type == WORD)
			command->command.args[val_carg++] = ((t_token *)(*tokens)->content)->value;
		else
		{
			command->command.redirections[rdr_carg++] = ((t_token *)(*tokens)->content)->value;
			*tokens = (*tokens)->next;
			command->command.redirections[rdr_carg++] = ((t_token *)(*tokens)->content)->value;
		}
		*tokens = (*tokens)->next;
	}
	command->type = NODE_COMMAND;
	return (command);
}
