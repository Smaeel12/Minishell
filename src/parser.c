/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:00:32 by iboubkri          #+#    #+#             */
/*   Updated: 2025/07/04 22:49:09 by iboubkri         ###   ########.fr       */
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
			return (ft_putendl_fd(UNCLOS_QUOTES, 2), ft_lstclear(lst, free), 1);
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

static int	parse_heredoc(t_tree *node, t_token *token)
{
	node->s_cmd.heredocs[node->s_cmd.hidx].mode = (token->type == WORD);
	if (!token || !(token->type == WORD || token->type == SQTS
			|| token->type == DQTS))
		return (ft_putendl_fd(MISSING_DELIM, 2), 1);
	node->s_cmd.ridx += expand_line(&node->s_cmd.rdrs[node->s_cmd.ridx],
			token->value, (bool[]){false, true}, MAX_REDIRECTIONS
			- node->s_cmd.ridx);
	node->s_cmd.heredocs[node->s_cmd.hidx++].delim = (
			node->s_cmd.rdrs[node->s_cmd.ridx - 1]);
	return (0);
}

static int	parse_redirection(t_tree *node, t_list **tokens, int *error)
{
	t_token	*token;
	size_t	len;

	if (node->s_cmd.ridx + 1 >= MAX_REDIRECTIONS)
		return (*tokens = (*tokens)->next, 0);
	token = (t_token *)(*tokens)->content;
	len = ft_strlen(token->value);
	if (len > 2)
		return (ft_putendl_fd(RDR_INVALID, 2), 1);
	node->s_cmd.rdrs[node->s_cmd.ridx++] = ft_strdup(token->value);
	*tokens = (*tokens)->next;
	token = (t_token *)(*tokens)->content;
	if (node->s_cmd.rdrs[node->s_cmd.ridx - 1][0] == '<' && len == 2)
	{
		if (node->s_cmd.hidx >= MAX_HEREDOCS)
			return (ft_putendl_fd(MAX_HEREDOCS_ERROR, 2), *error = 2, 1);
		return (parse_heredoc(node, token), 0);
	}
	else if (!token || !(token->type == WORD || token->type == SQTS
			|| token->type == DQTS))
		return (ft_putendl_fd(MISSING_FILENAME, 2), *error = 1, 1);
	node->s_cmd.ridx += expand_line(&node->s_cmd.rdrs[node->s_cmd.ridx],
			token->value, (bool[]){true, true}, MAX_REDIRECTIONS
			- node->s_cmd.ridx);
	return (0);
}

static t_tree	*parse_command(t_tree *node, t_list **tokens, int *error)
{
	t_token	*token;

	if (!node)
		return (ft_putendl_fd(MALLOC_FAILED, 2), NULL);
	if (!tokens || !*tokens || ((t_token *)(*tokens)->content)->type == PIPE)
		return (free(node), NULL);
	ft_bzero(node, sizeof(t_tree));
	while (*tokens && ((t_token *)(*tokens)->content)->type != PIPE)
	{
		token = (t_token *)(*tokens)->content;
		if (token->type == WORD || token->type == SQTS || token->type == DQTS)
		{
			if (node->s_cmd.aidx + 1 < MAX_ARGS)
				node->s_cmd.aidx
					+= expand_line(&node->s_cmd.args[node->s_cmd.aidx],
						token->value, (bool[]){true, true}, MAX_ARGS
						- node->s_cmd.aidx);
		}
		else if (parse_redirection(node, tokens, error))
			return (free(node), NULL);
		*tokens = (*tokens)->next;
	}
	return (node->type = COMMAND_NODE, node);
}

t_tree	*parse_pipeline(t_list *tokens, int *error)
{
	t_tree	*right;
	t_tree	*left;

	left = parse_command((t_tree *)malloc(sizeof(t_tree)), &tokens, error);
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
				(t_tree *)malloc(sizeof(t_tree)), &tokens, error);
		if (right->s_operator.left == NULL || right->s_operator.right == NULL
			|| ft_strlen(right->s_operator.value) > 2)
			return (ft_putendl_fd(PIPE_INVALID, 2), clear_tree(right),
				*error = 1, NULL);
		left = right;
	}
	return (left);
}
