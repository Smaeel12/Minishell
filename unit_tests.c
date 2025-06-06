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
// 	char *result = create_line((char *[]){ft_strdup(av[1]), NULL,
// ft_strdup(av[2])}, 3);
// 	printf("%s\n", result);
// 	free(result);
// }

int last_status;
char *create_line(char **strs, size_t nstrs);

char *get_env_value(char *key)
{
	char *value;

	if (!key || !*key)
		return (NULL);
	value = getenv(key);
	if (value)
		value = ft_strdup(value);
	if (key[0] == '?')
		value = ft_itoa(WEXITSTATUS(exit_status));
	return (free(key), value);
}
char *concatenate_string(char *line)
{
	char *command;
	size_t len;
	size_t i;
	size_t j;

	i = 0;
	j = 0;
	len = ft_strlen(line);
	command = (char *)malloc((len + 1) * sizeof(char));
	while (i < len)
	{
		while (line[i] == '\'' || line[i] == '"')
			i++;
		command[j++] = line[i++];
	}
	command[j] = '\0';
	return (free(line), command);
}

char *expand_line(t_token *token)
{
	char *result;
	char *key;
	size_t start;
	size_t i;

	i = 0;
	result = NULL;
	while (token->value[i])
	{
		start = i;
		while (token->value[start] == SQTS && token->value[++i] != SQTS)
			;
		while (token->value[++i] && !(token->value[i - 1] == '$' && (ft_isalpha(token->value[i]) || token->value[i] == '_' || token->value[i] == '?')))
			;
		key = &token->value[i];
		while (*key && token->value[++i] && (ft_isalnum(token->value[i]) || token->value[i] == '_'))
			;
		key = ft_substr(key, 0, (&token->value[i] - key));
		result = create_line((char *[]){result,
										ft_substr(token->value, start, i - start - ft_strlen(key) - (*key != '\0')),
										get_env_value(key)},
							 3);
	}
	return (concatenate_string(result));
}

int main(int ac, char **av)
{
	t_list *tokens;
	t_list *temp;
	char *result;

	tokens = NULL;
	temp = NULL;
	result = NULL;
	if (ac == 1)
		return (0);
	tokenize_cmdline(&tokens, av[1]);
	ft_lstiter(tokens, ft_puts);
	temp = tokens;
	while (temp)
	{
		result = expand_line(temp->content);
		printf("%s\n", result);
		temp = temp->next;
		free(result);
	}
	ft_lstclear(&tokens, clear_token);
}
