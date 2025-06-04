#include "include/main.h"

// int main(int ac, char **av)
// {
// 	t_token token;

// 	if (ac != 2)
// 		return (0);
// 	token.type = DQTS;
// 	token.value = ft_strdup(av[1]);
// 	expand_line(&token, 0);
// 	printf("%s\n", token.value);
// 	free(token.value);
// }

// int main(int ac, char *av[])
// {
// 	if (ac == 1)
// 		return (0);
// 	char *result = create_line((char *[]){ft_strdup(av[1]), NULL, ft_strdup(av[2])}, 3);
// 	printf("%s\n", result);
// 	free(result);
// }

int last_status;
char *get_value(char *key, size_t last_status);

char *expanding_line(t_token *token)
{
	char *result;
	size_t start;
	char *key;
	size_t i;

	i = 0;
	result = NULL;
	while (token->value[i])
	{
		start = i;
		while (token->value[start] == '\'' && token->value[++i] != '\'')
			;
		if (i > 0)
			result = create_line((char *[]){result, ft_substr(token->value, start + 1, i - 1)}, 2);

		start = i;
		while (token->value[++i] && !(token->value[i - 1] == '$' && (ft_isalpha(token->value[i]) || token->value[i] == '_' || token->value[i] == '?')))
			;

		if (token->value[i])
		{
			key = &token->value[i];
			while (*key && token->value[++i] && (ft_isalnum(token->value[i]) || token->value[i] == '_'))
				;
			key = ft_substr(key, 0, (&token->value[i] - key));
			result = create_line((char *[]){result, ft_substr(token->value, start, i - start - ft_strlen(key) - 1), get_value(key, last_status)}, 3);
		}
	}
	return (result);
	// return (free(token->value), token->value = join_string(result), free(result), token->value);
}

int main(int ac, char **av)
{
	t_list *tokens = NULL;
	t_list *temp = NULL;
	char *result = NULL;

	if (ac == 1)
		return (0);
	tokenize_cmdline(&tokens, av[1]);
	ft_lstiter(tokens, ft_puts);

	temp = tokens;
	while (temp)
	{
		result = expanding_line(temp->content);
		printf("%s\n", result);
		temp = temp->next;
		free(result);
	}

	ft_lstclear(&tokens, clear_token);
}
