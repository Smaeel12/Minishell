/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/26 09:21:04 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

void ft_puts(void *arg)
{
	t_token *token = arg;
	printf("%#x || %s\n", token->type, token->value);
}

void free_token(void *arg)
{
	t_token *token = arg;
	free(token->value);
	free(token);
}

void print_tree(t_tree *node)
{
	if (!node)
		return;
	if (node->type == NODE_COMMAND)
	{
		printf("arguments:\n");
		for (size_t i = 0; node->command.arguments[i]; i++)
			printf("\t%li: %s\n", i, node->command.arguments[i]);
		printf("redirections:\n");
		for (size_t i = 0; node->command.redirections[i]; i++)
			printf("\t%li: %s\n", i, node->command.redirections[i]);
		return;
	}
	print_tree(node->operator.left);
	print_tree(node->operator.right);
	free(node);
}

int main(int ac, char **av)
{
	t_list *tokens = NULL;
	t_list *temp = NULL;
	t_tree *tree;

	if (ac != 2)
		return 0;
	enum e_errors tokenizer_err = tokenizer(av[1], &tokens);
	if (tokenizer_err != OK)
		return (printf("Unbalenced Quotes\n"), 0);
	// ft_lstiter(tokens, ft_puts);

	temp = tokens;
	tree = parse_pipeline(&temp);
	if (tree == NULL)
		return (ft_lstclear(&tokens, free_token), 0);
	print_tree(tree);
	clear_tree(tree);
	ft_lstclear(&tokens, free_token);
}