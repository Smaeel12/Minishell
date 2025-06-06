/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:00:32 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/06 05:49:27 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int tokenize_cmdline(t_list **lst, char *line)
{
	enum e_token_type *st;
	size_t i;

	i = 0;
	st = (enum e_token_type[]){SCAN, advance(line[i])};
	while (line[i++])
	{
		while (line[i] && (st[1] == DQTS || st[1] == SQTS) && st[1] != st[0])
			st[0] = advance(line[i++]);
		if ((st[1] == DQTS || st[1] == SQTS) && st[1] != st[0])
			return (ft_putendl_fd(QTS_ERR, 2), ft_lstclear(lst, free),
					*lst = NULL, 1);
		st[0] = advance(line[i]);
		if (st[0] == SCAN || (!st[1] && st[0] != st[1]))
		{
			if (st[1] != SCAN)
				ft_lstadd_back(lst, ft_lstnew(create_token(st[1], line, i)));
			st = (enum e_token_type[]){SCAN, advance(line[i])};
			line = &line[i];
			i = 0;
		}
		if (st[0] == DQTS || st[0] == SQTS || st[1] == DQTS || st[1] == SQTS)
			st = (enum e_token_type[]){SCAN, advance(line[i])};
	}
	return (0);
}

static int parse_redirection(t_tree *node, t_list **tokens)
{
	if (ft_strlen(((t_token *)(*tokens)->content)->value) > 2)
		return (ft_putendl_fd(INV_RDR, 2), 1);
	node->command.redirections[node->command.ridx++] = ((t_token *)(*tokens)->content)->value;
	*tokens = (*tokens)->next;
	if (!*tokens || ((t_token *)(*tokens)->content)->type != WORD)
		return (ft_putendl_fd(INV_RDR_FILE, 2), 1);
	node->command.redirections[node->command.ridx++] = expand_line((*tokens)->content);
	return (0);
}

static int is_valid_token(t_token *token)
{
	return (token->type == DQTS || token->type == SQTS || token->type == WORD || token->type == OUTRDR || token->type == INRDR);
}

static t_tree *parse_command(t_list **tokens)
{
	t_tree *node;

	if (!tokens || !*tokens || !is_valid_token((*tokens)->content))
		return (NULL);
	node = (t_tree *)malloc(sizeof(t_tree));
	if (!node)
		return (ft_putendl_fd(MALLOC_FAILED, 2), NULL);
	ft_bzero(node, sizeof(t_tree));
	while (*tokens)
	{
		if (!is_valid_token((*tokens)->content))
			break;
		else if (((t_token *)(*tokens)->content)->type == DQTS || ((t_token *)(*tokens)->content)->type == SQTS || ((t_token *)(*tokens)->content)->type == WORD)
			node->command.arguments[node->command.aidx++] = expand_line((*tokens)->content);
		else if (parse_redirection(node, tokens))
			return (free(node), NULL);
		*tokens = (*tokens)->next;
	}
	return (node->type = NODE_COMMAND, node);
}

t_tree *parse_pipeline(t_list *tokens)
{
	t_tree *left;
	t_tree *right;

	left = parse_command(&tokens);
	while (tokens && ((t_token *)tokens->content)->type == PIPE)
	{
		right = (t_tree *)malloc(sizeof(t_tree));
		if (!right)
			return (ft_putendl_fd(MALLOC_FAILED, 2), clear_tree(right), NULL);
		ft_bzero(right, sizeof(t_tree));
		right->operator.value = ((t_token *)tokens->content)->value;
		right->type = NODE_OPERATOR;
		tokens = tokens->next;
		right->operator.right = parse_command(&tokens);
		right->operator.left = left;
		if (!left || right->operator.right == NULL || ft_strlen(right->operator.value) > 2)
			return (ft_putendl_fd(INV_PIPE, 2), clear_tree(right), NULL);
		left = right;
	}
	return (left);
}
