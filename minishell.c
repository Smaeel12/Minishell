/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/17 15:39:26 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/main.h"

int wait_processes()
{
	while (wait(&data.exit_status) > 0)
		;
	return (0);
}

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
		for (size_t i = 0; tree->command.arguments[i]; i++)
			printf("\t%li: %s\n", i, tree->command.arguments[i]);
		printf("redirections:\n");
		for (size_t i = 0; i < tree->command.ridx; i++)
			printf("\t%li: [%#x] %s\n", i, tree->command.redirections[i].type, tree->command.redirections[i].file);
		return;
	}
	print_tree(tree->operator.left);
	print_tree(tree->operator.right);
}

int main(void)
{
	t_list *tokens;
	t_tree *tree;
	char *line;

	init_env();
	tokens = NULL;
	init_signals();
	while (true)
	{
		line = readline("$> ");
		if (!line)
			break;
		rl_on_new_line();
		add_history(line);
		tokenize_cmdline(&tokens, line);
		tree = parse_pipeline(tokens);
		execute_pipeline(tree, (int[]){dup(IN), dup(OUT), -1});
		ft_lstclear(&tokens, clear_token);
		clear_tree(tree);
		wait_processes();
		free(line);
	}
	rl_clear_history();
	unlink("temp_pipe_file");
	return (clear_array(data.environs), clear_array(data.paths));
}
