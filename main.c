/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/27 14:41:26 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/main.h"

void ft_puts(void *arg)
{
	t_token *token = arg;
	printf("%#x || %s\n", token->type, token->value);
}

void print_tree(t_tree *tree)
{
	if (!tree)
		return;
	if (tree->type == NODE_COMMAND)
	{
		printf("arguments:\n");
		for (size_t i = 0; tree->command.arguments[i]; i++)
			printf("\t%li: %s\n", i, tree->command.arguments[i]);
		printf("redirections:\n");
		for (size_t i = 0; tree->command.redirections[i]; i++)
			printf("\t%li: %s\n", i, tree->command.redirections[i]);
		return;
	}
	print_tree(tree->operator.left);
	print_tree(tree->operator.right);
}

// int main()
// {
// 	t_tree *head;

// 	head = (t_tree *)malloc(sizeof(t_tree));
// 	memset(head, 0, sizeof(t_tree));
// 	head->type = NODE_OPERATOR;

// 	head->operator.left = (t_tree *)malloc(sizeof(t_tree));
// 	head->operator.right = (t_tree *)malloc(sizeof(t_tree));

// 	memset(head->operator.left, 0, sizeof(t_tree));
// 	memset(head->operator.right, 0, sizeof(t_tree));

// 	head->operator.left->type = NODE_COMMAND;
// 	head->operator.right->type = NODE_COMMAND;

// 	char *cmd1[] = {"echo", "hello", NULL};
// 	for (size_t i = 0; cmd1[i]; i++)
// 		head->operator.left->command.arguments[i] = cmd1[i];

// 	char *cmd2[] = {"cat", NULL};
// 	for (size_t i = 0; cmd2[i]; i++)
// 		head->operator.right->command.arguments[i] = cmd2[i];

// 	print_tree(head);

// 	char **paths = ft_split(getenv("PATH"), ':');

// 	execute_commands(head, paths, -1, -1);

// 	free(head->operator.left);
// 	free(head->operator.right);
// 	free(head);
// }

int main(int ac, char **av)
{
	t_list *tokens = NULL;
	t_tree *tree = NULL;
	char **paths = NULL;

	if (ac != 2)
		return (0);

	paths = ft_split(getenv("PATH"), ':');

	tokenize_cmdline(&tokens, av[1]);
	if (tokens)
	{
		tree = parse_pipeline(tokens, 0);
		if (tree)
		{
			print_tree(tree);
			execute_commands(tree, paths, (int[]){dup(0), dup(1)}, -1);
			clear_tree(tree);
		}
		ft_lstclear(&tokens, clear_token);
	}
	for (size_t i = 0; paths[i]; i++)
		free(paths[i]);
	free(paths);
	return (0);
}