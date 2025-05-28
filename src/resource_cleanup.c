/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_cleanup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:13:59 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/28 18:14:48 by iboubkri         ###   ########.fr       */
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

void	clear_tree(t_tree *tree)
{
	if (tree == NULL || tree->type == NODE_COMMAND)
	{
		free(tree);
		return ;
	}
	clear_tree(tree->operator.left);
	clear_tree(tree->operator.right);
	free(tree);
}

int	clear_paths(char **paths)
{
	size_t i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
	return (0);
}