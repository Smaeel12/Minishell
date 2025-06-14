/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/14 12:13:04 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/main.h"

int	main(void)
{
	t_tree	*tree;
	int		*streams;
	char	**paths;

	init_env();
	init_signals();
	g_data.exit_status = 0;
	paths = ft_split(getenv("PATH"), ':');
	while (true)
	{
		tree = parse_line();
		if (!tree)
			break ;
		streams = (int[]){dup(IN), dup(OUT), -1};
		execute_pipeline(tree, paths, streams);
		wait(&g_data.exit_status);
		unlink("temp_pipe_file");
		clear_tree(tree);
	}
	return (rl_clear_history(), clear_paths(paths), 0);
}
