/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/15 09:08:51 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/main.h"

int main(void)
{
	t_tree *	tree;
	int *streams;
	char **paths;

	init_env();
	init_signals();
	paths = ft_split(getenv("PATH"), ':');
	while (true)
	{
		tree = parse_line();
		if (!tree)
			break;
		streams = (int[]){dup(IN), dup(OUT), -1};
		execute_pipeline(tree, paths, (int[]){streams[IN], streams[OUT], -1});
		wait(&g_data.exit_status);
		clear_tree(tree);
	}
	unlink("temp_pipe_file");
	rl_clear_history();
	clear_paths(paths);
	clear_env();
	return (0);
}
