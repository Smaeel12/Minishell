/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/07/03 04:02:32 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/main.h"

struct s_global_data g_data;

int main(void)
{
	t_cmd *builtins;
	char *line;

	builtins = init_shell();
	while (true)
	{
		init_signals();
		line = readline("$> ");
		if (!line)
			break;
		rl_on_new_line();
		add_history(line);
		g_data.st_exit |= tokenize_cmdline(&g_data.tokens, line);
		g_data.tree = parse_pipeline(g_data.tokens, &g_data.st_exit);
		signal(SIGINT, SIG_IGN);
		execute_pipeline(g_data.tree, (int[]){dup(IN), dup(OUT), -1}, builtins);
		ft_lstclear(&g_data.tokens, clear_token);
		clear_tree(g_data.tree);
		unlink(HEREDOC_FILE);
		wait_processes();
		free(line);
	}
	return (rl_clear_history(), clear_array(g_data.paths),
			clear_array(g_data.environs), printf("exit\n"), 0);
}
