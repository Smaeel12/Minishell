#include "include/main.h"

void print_segment(char *line, size_t start, size_t end)
{
	for (size_t j = start; j < end; j++)
		putc(line[j], stdout);
	putc('\n', stdout);
}

char *get_env(char *key)
{
	char *value;

	if (key[0] == '?')
		return (ft_strdup("0"));
	value = getenv(key);
	if (value)
		return (free(key), ft_strdup(value));
	return (free(key), NULL);
}

void ft_strjoin_helper(char **result, char *segment)
{
	char *new_res;

	if (!segment)
		return;
	if (*result)
	{
		new_res = ft_strjoin(*result, segment);
		free(*result);
		free(segment);
		segment = new_res;
	}
	*result = segment;
}

int expanding_line_v2(char **result, char *line)
{
	char *key;
	size_t start;
	size_t end;
	size_t i;

	i = 0;
	start = 0;
	while (line[i++])
	{
		if ((line[start] == DQTS && line[i] != line[start]) ||
			(line[start] == SQTS && line[i] != line[start]))
			if (line[start] == SQTS || (line[i - 1] == '$' &&
										!(ft_isalpha(line[i]) || line[i] == '_' || line[i] == '?')))
				continue;
		if (line[i] &&
			!(line[i - 1] == '$' || line[i] == SQTS || line[i] == DQTS))
			continue;
		end = i;
		key = &line[i];
		start += (line[start] == DQTS || line[start] == SQTS);
		while (key[0] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		i += (key[0] && key[0] == '?');
		key = ft_substr(key, 0, (&line[i] - key));
		ft_strjoin_helper(result, ft_substr(line, start, end - start - !!key[0]));
		ft_strjoin_helper(result, get_env(key));
		start = i + (line[i] == DQTS || line[i] == SQTS);
		i = start;
	}
	return (0);
}

int main(int ac, char **av)
{
	// char *result1;
	char *result2;

	if (ac != 2)
		return (1);
	// result1 = NULL;
	result2 = NULL;
	// expanding_line(&result1, av[1]);
	expanding_line_v2(&result2, av[1]);
	printf("expanding line: %s\n", av[1]);
	// printf("result line: %s\n", result1);
	printf("result line: %s\n", result2);
	// free(result1);
	free(result2);
}
