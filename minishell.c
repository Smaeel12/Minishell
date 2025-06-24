/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/24 14:29:41 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/main.h"

struct s_global_data	g_data;

void	ft_puts(void *arg)
{
	t_token	*token;

	token = arg;
	printf("%#x || [%s]\n", token->type, token->value);
}

void	print_tree(t_tree *tree)
{
	if (!tree)
		return ;
	if (tree->type == COMMAND_NODE)
	{
		printf("arguments:\n");
		for (size_t i = 0; tree->s_command.arguments[i]; i++)
			printf("\t%li: %s\n", i, tree->s_command.arguments[i]);
		printf("redirections:\n");
		for (size_t i = 0; tree->s_command.redirections[i].file; i++)
			printf("\t%li: %u %s\n", i, tree->s_command.redirections[i].type,
				tree->s_command.redirections[i].file);
		return ;
	}
	print_tree(tree->s_operator.left);
	print_tree(tree->s_operator.right);
}

int	main(void)
{
	t_list	*tokens;
	t_tree	*tree;
	char	*line;

	init_env();
	tokens = NULL;
	while (true)
	{
		init_signals();
		line = readline("$> ");
		if (!line)
			break ;
		rl_on_new_line();
		add_history(line);
		tokenize_cmdline(&tokens, line);
		// ft_lstiter(tokens, ft_puts);
		tree = parse_pipeline(tokens);
		// print_tree(tree);
		execute_pipeline(tree, (int[]){dup(IN), dup(OUT), -1});
		ft_lstclear(&tokens, clear_token);
		unlink(HEREDOC_FILE);
		wait_processes();
		clear_tree(tree);
		free(line);
	}
	return (rl_clear_history(), clear_array(g_data.environs),
		clear_array(g_data.paths));
}
