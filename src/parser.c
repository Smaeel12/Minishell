/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:00:32 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/25 16:17:08 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

// TODO: handle unclosed quotes
enum e_errors tokenizer(char *line, t_list **lst)
{
	enum e_token_type cstate;
	enum e_token_type state;
	size_t i;

	i = 0;
	cstate = SCAN;
	state = advance(line[i]);
	while (line[i++])
	{
		cstate = advance(line[i]);
		while (line[i] && (state == DQTS || state == SQTS) && state != cstate)
			cstate = advance(line[i++]);
		cstate = advance(line[i]);
		if (state != cstate)
		{
			if (state != SCAN)
				ft_lstadd_back(lst, ft_lstnew(create_token(state, line, i)));
			state = cstate;
			line = &line[i];
			i = 0;
		}
	}
	return (OK);
}

// TODO: handle invalid pipe
t_tree *parse_pipeline(t_list **tokens)
{
	t_tree *pipe;
	t_tree *node;

	if (!tokens || !*tokens)
		return (NULL);
	pipe = parse_command(tokens);
	while (pipe && (*tokens) && ((t_token *)(*tokens)->content)->type == PIPE)
	{
		node = (t_tree *)malloc(sizeof(t_tree));
		ft_bzero(node, sizeof(t_tree));
		node->operator.value = ((t_token *)(*tokens)->content)->value;
		node->type = NODE_OPERATOR;
		*tokens = (*tokens)->next;
		node->operator.right = parse_command(tokens);
		node->operator.left = pipe;
		pipe = node;
	}
	return (pipe);
}

t_tree *parse_command(t_list **tokens)
{
	t_tree *node;

	if (!tokens || !*tokens)
		return (NULL);
	node = (t_tree *)malloc(sizeof(t_tree));
	ft_bzero(node, sizeof(t_tree));
	while ((*tokens) && (((t_token *)(*tokens)->content)->type == OUTRDR ||
						 ((t_token *)(*tokens)->content)->type == INRDR ||
						 is_word(((t_token *)(*tokens)->content), last_status)))
	{
		if (((t_token *)(*tokens)->content)->type == WORD)
			node->command.arguments[node->command.aidx++] =
				((t_token *)(*tokens)->content)->value;
		else
		{
			node->command.redirections[node->command.ridx++] =
				((t_token *)(*tokens)->content)->value;
			*tokens = (*tokens)->next;
			node->command.redirections[node->command.ridx++] =
				((t_token *)(*tokens)->content)->value;
		}
		*tokens = (*tokens)->next;
	}
	return (node->type = NODE_COMMAND, node);
}
