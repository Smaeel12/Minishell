/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_cleanup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:13:59 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/24 21:18:33 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	clean_exit(int status)
{
	clear_tree(g_data.tree);
	clear_array(g_data.paths);
	clear_array(g_data.environs);
	ft_lstclear(&g_data.tokens, clear_token);
	exit(status);
}

void	clear_token(void *arg)
{
	t_token	*token;

	token = arg;
	free(token->value);
	free(token);
}

int	clear_tree(t_tree *tree)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (tree == NULL)
		return (0);
	if (tree->type == COMMAND_NODE)
	{
		while (tree->s_command.arguments[i])
			free(tree->s_command.arguments[i++]);
		while (tree->s_command.redirections[j].file)
			free(tree->s_command.redirections[j++].file);
		free(tree);
		return (0);
	}
	clear_tree(tree->s_operator.left);
	clear_tree(tree->s_operator.right);
	free(tree);
	return (0);
}

int	clear_array(char **array)
{
	size_t	i;

	i = 0;
	while (array && array[i])
		free(array[i++]);
	free(array);
	return (0);
}
