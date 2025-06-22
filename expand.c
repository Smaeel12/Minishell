#include "libft/libft.h"
#include <stdio.h>

void print_segment(char *line, size_t start, size_t end)
{
	for (size_t j = start; j < end; j++)
		putc(line[j], stdout);
	putc('\n', stdout);
}

char *get_env(char *key);
int append_string(char **result, char *segment);
void expand_line(char **result, char *line);

enum e_type
{
	NONE,
	WORD,
	APPEND,
	HEREDOC,
	PIPE = '|',
	DQTS = '"',
	SQTS = '\'',
	INRDR = '<',
	OUTRDR = '>',
};

int main(int ac, char **av)
{
	char *result;

	if (ac != 2)
		return (0);
	result = NULL;
	expand_line(&result, av[1]);
	printf("INP: %s\n", av[1]);
	printf("OUT: %s\n", result);
	free(result);
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

int append_string(char **result, char *segment)
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

void expand_line(char **result, char *line)
{
	char *key;
	enum e_type state;
	size_t keylen;
	size_t lshift;
	size_t rshift;
	size_t start;
	size_t i;

	i = 0;
	start = 0;
	state = line[start];
	while (line[i++])
	{
		if (state == SQTS && line[i] != (char)state)
			continue;
		if (line[i - 1] == '$' && !(ft_isalpha(line[i]) || line[i] == '_' || line[i] == '?'))
			continue;
		if (line[i - 1] != '$' && state == DQTS && line[i] != (char)state)
			continue;
		if (line[i - 1] != '$' && line[i] && line[i] != DQTS && line[i] != SQTS)
			continue;
		key = &line[i];
		while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		i += (key[0] && key[0] == '?');
		keylen = (&line[i] - key);
		key = ft_substr(key, 0, keylen);
		lshift = (state == SQTS || state == DQTS) && (line[start] == (char)state);
		rshift = (state == SQTS || state == DQTS) && (line[i] == line[start]);
		append_string(result,
					  ft_substr(line, (start + lshift), (i - keylen - !!key[0]) - (start + lshift)));
		append_string(result, get_env(key));
		i += rshift;
		start = i;
		if (rshift)
			state = line[i];
	}
}