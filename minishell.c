/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/24 21:19:11 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/main.h"

struct s_global_data	g_data;

int	main(void)
{
	char	*line;

	init_env();
	while (true)
	{
		g_data.tree = NULL;
		init_signals();
		line = readline("$> ");
		if (!line)
			break ;
		rl_on_new_line();
		add_history(line);
		tokenize_cmdline(&g_data.tokens, line);
		g_data.tree = parse_pipeline(g_data.tokens);
		execute_pipeline(g_data.tree, (int []){dup(IN), dup(OUT), -1});
		ft_lstclear(&g_data.tokens, clear_token);
		clear_tree(g_data.tree);
		unlink(HEREDOC_FILE);
		wait_processes();
		free(line);
	}
	return (rl_clear_history(), clean_exit(0), 0);
}
