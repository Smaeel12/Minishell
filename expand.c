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

int concatenate_result(char **result, char *segment, char *value)
{
	char *new_res;
	size_t idx;
	size_t len;

	idx = 0;
	len = ft_strlen(segment);
	if (value)
		len += ft_strlen(value);
	if (*result)
		len += ft_strlen(*result);
	new_res = (char *)malloc((len + 1) * sizeof(char));
	if (!new_res)
		return (1);
	if (*result)
		idx = ft_strlcpy(new_res + idx, *result, ft_strlen(*result) + 1);
	idx += ft_strlcpy(new_res + idx, segment, ft_strlen(segment) + 1);
	if (value)
		idx += ft_strlcpy(new_res + idx, value, ft_strlen(value) + 1);
	free(value);
	free(segment);
	free(*result);
	*result = new_res;
	return (0);
}

// void expanding_line_v2(char **result, char *line)
// {
// 	char *key;
// 	size_t beg;
// 	size_t i;

// 	i = 0;
// 	beg = 0;
// 	while (line[i++])
// 	{
// 		if (((line[beg] == DQTS || line[beg] == SQTS) && line[i] != line[beg]))
// 			if ((line[i - 1] == '$' && !(ft_isalpha(line[i]) || line[i] == '_'
// || line[i] == '?')) || line[beg] == SQTS)
// 				continue ;
// 		if (line[i] && line[i - 1] != '$' && line[i] != SQTS && line[i] != DQTS)
// 			continue ;
// 		key = &line[i];
// 		beg += (line[beg] == DQTS || line[beg] == SQTS);
// 		while (key[0] && (ft_isalnum(line[i]) || line[i] == '_'))
// 			i++;
// 		i += (key[0] && key[0] == '?');
// 		key = ft_substr(key, 0, (&line[i] - key));
// 		concatenate_result(result, ft_substr(line, beg, i - ft_strlen(key) - beg
// - !!key[0]), get_env(key));
// 		i += (line[i] == DQTS || line[i] == SQTS);
// 		beg = i;
// 	}
// }

int ft_strjoin_helper(char **result, char *segment)
{
	char *new_res;

	if (!segment)
		return (0);
	if (*result)
	{
		new_res = ft_strjoin(*result, segment);
		free(*result);
		free(segment);
		segment = new_res;
	}
	*result = segment;
	return (0);
}

int check_valid(char *line, size_t start, size_t end)
{
	if (line[start] == SQTS && line[end] != line[start])
		return (1);
	if (line[end - 1] == '$' && !(ft_isalpha(line[end]) || line[end] == '_' || line[end] == '?'))
		return (1);
	if (line[end - 1] != '$' && line[start] == DQTS && line[end] != line[start])
		return (1);
	if (line[end - 1] != '$' && line[end] && line[end] != DQTS && line[end] != SQTS)
		return (1);
	return (0);
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
		end = i;
		if (check_valid(line, start, end))
			continue;
		key = &line[i];
		start += (line[start] == DQTS || line[start] == SQTS);
		while (key[0] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		i += (key[0] && key[0] == '?');
		key = ft_substr(key, 0, (&line[i] - key));
		ft_strjoin_helper(result, ft_substr(line, start, end - start - !!key[0]));
		ft_strjoin_helper(result, get_env(key));
		i += (line[i] == DQTS || line[i] == SQTS);
		start = i;
	}
	return (0);
}

int main(int ac, char **av)
{
	char *result2;

	// char *result1;
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
