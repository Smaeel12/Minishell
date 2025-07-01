/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/07/01 03:10:31 by iboubkri         ###   ########.fr       */
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
		init_signals();
		line = readline("$> ");
		if (!line)
			break ;
		rl_on_new_line();
		add_history(line);
		g_data.exit_status = tokenize_cmdline(&g_data.tokens, line) << 8;
		g_data.tree = parse_pipeline(g_data.tokens);
		signal(SIGINT, SIG_IGN);
		execute_pipeline(g_data.tree, (int []){dup(IN), dup(OUT), -1});
		ft_lstclear(&g_data.tokens, clear_token);
		clear_tree(g_data.tree);
		unlink(HEREDOC_FILE);
		wait_processes();
		free(line);
	}
	return (rl_clear_history(), clear_array(g_data.paths),
		clear_array(g_data.environs), printf("exit\n"), 0);
}
