/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/18 09:07:17 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

// // tokenizer tester
// int	main(int ac, char **av)
// {
// 	t_token	*token;

// 	if (ac != 2)
// 		return (0);
// 	token = tokenizer(av[1]);
// 	while (1)
// 	{
// 		if (!token)
// 			break ;
// 		printf("type: %c || value: %s\n", token->type, token->value);
// 		free(token->value);
// 		free(token);
// 		token = tokenizer(NULL);
// 	}
// }

// int main(int ac, char **av)
// {
// 	t_command *command;

// 	if (ac != 2)
// 		return (0);
// 	command = parse_command(av[1]);
// 	printf("command: %s\n", command->command);
// 	for (size_t i = 0; command->args[i]; i++)
// 		printf("arg %li: %s\n", i, command->args[i]);
// }

void binary_tree_preorder(const t_node *tree, int (*func)(const char *))
{
	if (!tree || !func)
		return;
	if (tree->type == COMMAND)
	{
		for (size_t i = 0; tree->u_data.command[i]; i++)
			func(tree->u_data.command[i]);
	}
	else if (tree->type == INRDR || tree->type == OUTRDR)
		func(tree->u_data.value);
	binary_tree_preorder(tree->left, func);
	binary_tree_preorder(tree->right, func);
}
void binary_tree_delete(t_node *tree)
{
	if (tree == NULL)
		return;
	binary_tree_delete(tree->left);
	binary_tree_delete(tree->right);
	free(tree);
}

int main(int ac, char **av)
{
	t_node *head;

	if (ac != 2)
		return 0;
	head = parse_command(av[1]);
	binary_tree_preorder(head, puts);
	binary_tree_delete(head);
}