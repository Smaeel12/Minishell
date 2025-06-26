/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/25 18:02:29 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/main.h"

struct s_global_data g_data;

void ft_puts(void *arg)
{
	t_token *token = arg;
	printf("%#x || [%s]\n", token->type, token->value);
}

void print_tree(t_tree *tree)
{
	if (!tree)
		return;
	if (tree->type == COMMAND_NODE)
	{
		printf("arguments:\n");
		for (size_t i = 0; tree->s_command.arguments[i]; i++)
			printf("\t%li: %s\n", i, tree->s_command.arguments[i]);
		printf("redirections:\n");
		for (size_t i = 0; i < tree->s_command.ridx; i++)
			printf("\t%li: [%#x] %s\n", i, tree->s_command.redirections[i].type,
				   tree->s_command.redirections[i].file);
		printf("heredocs:\n");
		for (size_t i = 0; i < tree->s_command.hidx; i++)
			printf("\t%li: [%s] %s\n", i, (tree->s_command.heredocs[i].mode ? "true" : "false"),
				   tree->s_command.heredocs[i].delim);
		return;
	}
	print_tree(tree->s_operator.left);
	print_tree(tree->s_operator.right);
}

int main(void)
{
	char *line;

	init_env();
	while (true)
	{
		init_signals();
		line = readline("$> ");
		if (!line)
			break;
		rl_on_new_line();
		add_history(line);
		tokenize_cmdline(&g_data.tokens, line);
		g_data.tree = parse_pipeline(g_data.tokens);
		print_tree(g_data.tree);
		signal(SIGINT, SIG_IGN);
		execute_pipeline(g_data.tree, (int[]){dup(IN), dup(OUT), -1});
		ft_lstclear(&g_data.tokens, clear_token);
		clear_tree(g_data.tree);
		unlink(HEREDOC_FILE);
		wait_processes();
		free(line);
	}
	return (rl_clear_history(), clear_array(g_data.paths),
			clear_array(g_data.environs), printf("exit\n"), 0);
}
