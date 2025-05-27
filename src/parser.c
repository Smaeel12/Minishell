/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:00:32 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/26 16:58:40 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

enum e_errors tokenize_cmdline(t_list **lst, char *line)
{
	enum e_token_type cstate;
	enum e_token_type state;
	size_t i;

	i = 0;
	cstate = SCAN;
	state = advance(line[i]);
	while (line[i])
	{
		while (line[i++] && (state == DQTS || state == SQTS) && state != cstate)
			cstate = advance(line[i]);
		cstate = advance(line[i]);
		if (state != cstate || state == DQTS || state == SQTS)
		{
			if ((state == DQTS || state == SQTS) && line[i - 1] != (char)state)
				return QTS_ERR;
			if (state != SCAN)
				ft_lstadd_back(lst, ft_lstnew(create_token(state, line, i)));
			line = &line[i];
			state = cstate;
			cstate = SCAN;
			i = 0;
		}
	}
	return (OK);
}

t_tree *parse_pipeline(t_list *tokens, size_t last_status)
{
	t_tree *left;
	t_tree *right;

	left = parse_command(&tokens, last_status);
	while (tokens && ((t_token *)tokens->content)->type == PIPE)
	{
		right = (t_tree *)malloc(sizeof(t_tree));
		ft_bzero(right, sizeof(t_tree));
		right->operator.value = ((t_token *)tokens->content)->value;
		right->type = NODE_OPERATOR;
		tokens = tokens->next;
		right->operator.right = parse_command(&tokens, last_status);
		right->operator.left = left;
		if (!left || right->operator.right == NULL || ft_strlen(right->operator.value) > 2)
			return ft_putendl_fd(INV_PIPE, 2), clear_tree(right), NULL;
		left = right;
	}
	return (left);
}

t_tree *parse_command(t_list **tokens, size_t last_status)
{
	char *expanded_line;
	t_token *token;
	t_tree *node;

	if (!tokens || !*tokens)
		return NULL;
	token = (t_token *)(*tokens)->content;
	if (!(token->type == DQTS || token->type == SQTS || token->type == WORD ||
		  token->type == OUTRDR || token->type == INRDR))
		return (NULL);
	node = (t_tree *)malloc(sizeof(t_tree));
	ft_bzero(node, sizeof(t_tree));
	while (*tokens)
	{
		token = (t_token *)(*tokens)->content;
		if (!(token->type == DQTS || token->type == SQTS || token->type == WORD ||
			  token->type == OUTRDR || token->type == INRDR))
			break;
		if (token->type == DQTS || token->type == WORD)
		{
			expanded_line = expand_line(token->value, last_status);
			free(token->value);
			token->value = expanded_line;
		}
		if (token->type == SQTS || token->type == DQTS || token->type == WORD)
			node->command.arguments[node->command.aidx++] = token->value;
		if (token->type == OUTRDR || token->type == INRDR)
		{
			if (ft_strlen(token->value) > 2)
				return ft_putendl_fd(INV_RDR, 2), free(node), NULL;
			node->command.redirections[node->command.ridx++] = token->value;
			*tokens = (*tokens)->next;
			if (!*tokens || ((t_token *)(*tokens)->content)->type != WORD)
				return ft_putendl_fd(INV_RDR_FILE, 2), free(node), NULL;
			node->command.redirections[node->command.ridx++] =
				((t_token *)(*tokens)->content)->value;
		}
		*tokens = (*tokens)->next;
	}
	return (node->type = NODE_COMMAND, node);
}
