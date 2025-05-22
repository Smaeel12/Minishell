/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/22 09:25:28 by iboubkri         ###   ########.fr       */
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

void print_tree(t_ASTNode *node)
{
	if (!node)
		return;
	if (node->type == NODE_COMMAND)
	{
		printf("redirections: ");
		for (size_t i = 0; node->command.redirections[i]; i++)
			printf("%s ", node->command.redirections[i]);
		printf("\n");
		printf("values: ");
		for (size_t i = 0; node->command.args[i]; i++)
			printf("%s ", node->command.args[i]);
		printf("\n");
	}
	free(node);
}

int main(int ac, char **av)
{
	t_list *tokens = NULL;
	t_list *temp = NULL;
	t_ASTNode *node;

	if (ac != 2)
		return 0;
	tokenizer(av[1], &tokens);
	temp = tokens;
	node = parse_command(&temp);
	print_tree(node);
	// ft_lstiter(tokens, ft_puts);
	ft_lstclear(&tokens, free_token);
}