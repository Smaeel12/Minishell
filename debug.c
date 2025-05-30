
#include "include/main.h"

void ft_puts(void *arg)
{
	t_token *token = arg;
	printf("%#x || [%s]\n", token->type, token->value);
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