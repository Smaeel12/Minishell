/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/28 22:13:36 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/main.h"

int main(void)
{
	t_list *tokens = NULL;
	t_tree *tree = NULL;
	char **paths = NULL;
	char *line;

	paths = ft_split(getenv("PATH"), ':');
	while (1)
	{
		line = readline("$> ");
		if (!line)
			break;
		rl_on_new_line();
		add_history(line);
		tokenize_cmdline(&tokens, line);
		tree = parse_pipeline(tokens, 0);
		execute_commands(tree, paths, (int[]){dup(0), dup(1)}, -1);
		wait(NULL);
		clear_tree(tree);
		ft_lstclear(&tokens, clear_token);
		free(line);
	}
	rl_clear_history();
	clear_paths(paths);
	return (0);
}
