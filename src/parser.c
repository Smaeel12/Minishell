/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:00:32 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/14 09:53:57 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int tokenize_cmdline(t_list **lst, char *line)
{
	enum e_token_type cstate;
	enum e_token_type state;
	size_t i;

	i = 0;
	state = advance(line[i]);
	while (line[i++])
	{
		cstate = SCAN;
		while (line[i] && (state == DQTS || state == SQTS) && cstate != state)
			cstate = advance(line[i++]);
		if ((state == DQTS || state == SQTS) && cstate != state)
			return (ft_putendl_fd(QTS_ERR, 2), ft_lstclear(lst, free), 1);
		cstate = advance(line[i]);
		if (cstate != state)
		{
			if (add_token(lst, state, cstate, line, i))
			{
				line = &line[i];
				i = 0;
			}
			state = cstate;
		}
	}
	return (0);
}

static int parse_redirection(t_tree *node, t_list **tokens)
{
	size_t len;

	len = ft_strlen(((t_token *)(*tokens)->content)->value);
	if (len > 2)
		return (ft_putendl_fd(INV_RDR, 2), 1);
	else if (((t_token *)(*tokens)->content)->value[0] == '<' && len == 2)
		node->command.redirections[node->command.ridx].type = HEREDOC;
	else if (((t_token *)(*tokens)->content)->value[0] == '<' && len == 1)
		node->command.redirections[node->command.ridx].type = INRDR;
	else if (((t_token *)(*tokens)->content)->value[0] == '>' && len == 2)
		node->command.redirections[node->command.ridx].type = APPEND;
	else if (((t_token *)(*tokens)->content)->value[0] == '>' && len == 1)
		node->command.redirections[node->command.ridx].type = OUTRDR;
	*tokens = (*tokens)->next;
	if (!*tokens || ((t_token *)(*tokens)->content)->type != WORD)
		return (ft_putendl_fd(INV_RDR_FILE, 2), 1);
	node->command.redirections[node->command.ridx++].filename = expand_line((*tokens)->content);
	return (0);
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

t_tree *parse_line(void)
{
	t_list *tokens;
	t_tree *tree;
	char *line;

	tree = NULL;
	tokens = NULL;
	line = readline("$> ");
	if (line)
	{
		rl_on_new_line();
		add_history(line);
		tokenize_cmdline(&tokens, line);
		tree = parse_pipeline(tokens);
		ft_lstclear(&tokens, clear_token);
	}
	return (free(line), tree);
}