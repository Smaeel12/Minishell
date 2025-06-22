/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/22 22:45:35 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/main.h"

struct s_global_data	g_data;

int	main(void)
{
	t_list	*tokens;
	t_tree	*tree;
	char	*line;

	init_env();
	tokens = NULL;
	init_signals();
	while (true)
	{
		line = readline("$> ");
		if (!line)
			break ;
		rl_on_new_line();
		add_history(line);
		tokenize_cmdline(&tokens, line);
		tree = parse_pipeline(tokens);
		execute_pipeline(tree, (int []){dup(IN), dup(OUT), -1});
		ft_lstclear(&tokens, clear_token);
		clear_tree(tree);
		wait_processes();
		free(line);
	}
	return (rl_clear_history(), unlink(HEREDOC_FILE),
		clear_array(g_data.environs), clear_array(g_data.paths));
}
