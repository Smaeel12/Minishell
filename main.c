/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/02 23:50:43 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/main.h"

char *get_line(void)
{
	char *line;

	line = readline("$> ");
	if (line)
	{
		rl_on_new_line();
		add_history(line);
	}
	return (line);
}

int main(void)
{
	t_list *tokens;
	t_tree *tree;
	char **paths;
	char *line;
	int lstatus;

	lstatus = 0;
	tokens = NULL;
	init_signals();
	paths = ft_split(getenv("PATH"), ':');
	while (1)
	{
		line = get_line();
		if (!line)
			break;
		tokenize_cmdline(&tokens, line);
		// ft_lstiter(tokens, ft_puts); // debug
		tree = parse_pipeline(tokens, WEXITSTATUS(lstatus));
		print_tree(tree);
		execute_pipeline(tree, paths, (int[]){dup(0), dup(1)}, -1);
		wait(&lstatus);
		ft_lstclear(&tokens, clear_token);
		clear_tree(tree);
		free(line);
	}
	return (rl_clear_history(), clear_paths(paths), 0);
}
