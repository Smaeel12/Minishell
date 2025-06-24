/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:00:32 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/24 23:50:36 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	tokenize_cmdline(t_list **lst, char *line)
{
	enum e_type	cstate;
	enum e_type	state;
	char		*temp;

	temp = line;
	state = determine_token(*temp);
	while (*(temp++))
	{
		cstate = NONE;
		while (*temp && (state == DQTS || state == SQTS) && cstate != state)
			cstate = determine_token(*(temp++));
		if ((state == DQTS || state == SQTS) && cstate != state)
			return (ft_putendl_fd(QUOTES_ERR, 2), ft_lstclear(lst, free), 1);
		cstate = determine_token(*temp);
		if (((state == DQTS || state == SQTS || state == WORD)
				&& (cstate == WORD || cstate == DQTS || cstate == SQTS)))
			state = cstate;
		if (cstate != state)
		{
			add_token(lst, state, line, temp - line);
			state = cstate;
			line = temp;
		}
	}
	return (0);
}

static int	parse_redirection(t_tree *node, t_list **tokens)
{
	t_token	*token;
	size_t	len;

	token = (t_token *)(*tokens)->content;
	len = ft_strlen(token->value);
	if (len > 2)
		return (ft_putendl_fd(RDR_ERROR, 2), 1);
	if (token->value[0] == '<' && len == 2)
		node->s_command.redirections[node->s_command.ridx].type = HEREDOC;
	else if (token->value[0] == '>' && len == 2)
		node->s_command.redirections[node->s_command.ridx].type = APPEND;
	else if (token->value[0] == '>' && len == 1)
		node->s_command.redirections[node->s_command.ridx].type = OUTRDR;
	else if (token->value[0] == '<' && len == 1)
		node->s_command.redirections[node->s_command.ridx].type = INRDR;
	*tokens = (*tokens)->next;
	token = (t_token *)(*tokens)->content;
	if (!*tokens || !(token->type == WORD || token->type == SQTS
			|| token->type == DQTS))
		return (ft_putendl_fd(MISSING_FILE_ERROR, 2), 1);
	expand_line(&node->s_command.redirections[node->s_command.ridx++].file,
		token->value);
	return (0);
}

static t_tree	*parse_command(t_tree *node, t_list **tokens)
{
	char	*line;

	if (!node)
		return (ft_putendl_fd(MALLOC_FAILED, 2), NULL);
	if (!tokens || !*tokens || ((t_token *)(*tokens)->content)->type == PIPE)
		return (free(node), NULL);
	ft_bzero(node, sizeof(t_tree));
	while (*tokens && ((t_token *)(*tokens)->content)->type != PIPE)
	{
		line = NULL;
		if (((t_token *)(*tokens)->content)->type == WORD
			|| ((t_token *)(*tokens)->content)->type == SQTS
			|| ((t_token *)(*tokens)->content)->type == DQTS)
		{
			expand_line(&line, ((t_token *)(*tokens)->content)->value);
			if (line[0])
				node->s_command.arguments[node->s_command.aidx++] = line;
			else
				free(line);
		}
		else if (parse_redirection(node, tokens))
			return (free(node), NULL);
		*tokens = (*tokens)->next;
	}
	return (node->type = COMMAND_NODE, node);
}

t_tree	*parse_pipeline(t_list *tokens)
{
	t_tree	*right;
	t_tree	*left;

	left = parse_command((t_tree *)malloc(sizeof(t_tree)), &tokens);
	while (tokens && ((t_token *)tokens->content)->type == PIPE)
	{
		right = (t_tree *)malloc(sizeof(t_tree));
		if (!right)
			return (ft_putendl_fd(MALLOC_FAILED, 2), clear_tree(left), NULL);
		ft_bzero(right, sizeof(t_tree));
		right->s_operator.value = ((t_token *)tokens->content)->value;
		tokens = tokens->next;
		right->type = OPERATOR_NODE;
		right->s_operator.left = left;
		right->s_operator.right = parse_command(
				(t_tree *)malloc(sizeof(t_tree)), &tokens);
		if (right->s_operator.left == NULL || right->s_operator.right == NULL
			|| ft_strlen(right->s_operator.value) > 2)
			return (ft_putendl_fd(PIPE_ERROR, 2), clear_tree(right), NULL);
		left = right;
	}
	return (left);
}
