/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:00:32 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/23 14:03:14 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

void clear_tree(t_ASTNode *tree)
{
	if (tree == NULL || tree->type == NODE_COMMAND)
	{
		free(tree);
		return;
	}
	clear_tree(tree->operator.left);
	clear_tree(tree->operator.right);
	free(tree);
}

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

t_ASTNode *parse_pipeline(t_list **tokens)
{
	t_ASTNode *pipe;
	t_ASTNode *node;

	if (!tokens || !*tokens)
		return (NULL);
	pipe = parse_command(tokens);
	while (pipe && (*tokens) && ((t_token *)(*tokens)->content)->type == PIPE)
	{
		node = (t_ASTNode *)malloc(sizeof(t_ASTNode));
		ft_bzero(node, sizeof(t_ASTNode));
		node->operator.value = ((t_token *)(*tokens)->content)->value;
		node->type = NODE_OPERATOR;
		*tokens = (*tokens)->next;
		node->operator.right = parse_command(tokens);
		node->operator.left = pipe;
		pipe = node;
	}
	return (pipe);
}

int is_word(t_token *token)
{
	// TODO: expand $VAR and "$VAR"
	char *new_string;
	char *value;
	char *key;
	size_t i;

	new_string = (char *)malloc(1024 * sizeof(char));
	if (token->type != INSQTS && token->type != INDQTS && token->type != WORD)
		return (0);
	while (token->value[i] != token->type)
	{
		if ((token->type == INDQTS || token->type == WORD) &&
			token->value[i] == '$')
	}
	if ((token->type == INDQTS || token->type == INSQTS) &&
		token->value[i] != (char)token->type)
		printf("bash: syntax error near unexpected token `%c'", token->type);
	token->type = WORD;
	return (1);
}

t_ASTNode *parse_command(t_list **tokens)
{
	t_ASTNode *node;

	if (!tokens || !*tokens)
		return (NULL);
	node = (t_ASTNode *)malloc(sizeof(t_ASTNode));
	ft_bzero(node, sizeof(t_ASTNode));
	while ((*tokens) && (((t_token *)(*tokens)->content)->type == OUTRDR ||
						 ((t_token *)(*tokens)->content)->type == INRDR ||
						 is_word(((t_token *)(*tokens)->content)->value)))
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
