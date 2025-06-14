/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_cleanup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:13:59 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/13 15:36:16 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

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
	if (tree->type == NODE_COMMAND)
	{
		while (tree->command.arguments[i])
			free(tree->command.arguments[i++]);
		while (tree->command.redirections[j].filename)
			free(tree->command.redirections[j++].filename);
		free(tree);
		return (0);
	}
	clear_tree(tree->operator.left);
	clear_tree(tree->operator.right);
	free(tree);
	return (0);
}

int	clear_paths(char **paths)
{
	size_t	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
	return (0);
}

int	clear_env(void)
{
	size_t	i;

	i = 0;
	while (g_data.environs && g_data.environs[i])
	{
		free(g_data.environs[i]);
		i++;
	}
	free(g_data.environs);
	return (0);
}
