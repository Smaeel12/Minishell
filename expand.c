#include "libft/libft.h"
#include <stdio.h>

void	print_segment(char *line, size_t start, size_t end)
{
	for (size_t j = start; j < end; j++)
		putc(line[j], stdout);
	putc('\n', stdout);
}

char	*get_env(char *key);
int		append_string(char **result, char *segment);
void	expand_line(char **result, char *line);

enum	e_type
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

int	main(int ac, char **av)
{
	char	*result;

	if (ac != 2)
		return (0);
	result = NULL;
	expand_line(&result, av[1]);
	printf("INP: %s\n", av[1]);
	printf("OUT: %s\n", result);
	free(result);
}

char	*get_env(char *key)
{
	char	*value;

	if (key[0] == '?')
		return (ft_strdup("0"));
	value = getenv(key);
	if (value)
		return (free(key), ft_strdup(value));
	return (free(key), NULL);
}

int	append_string(char **result, char *segment)
{
	char	*new_res;

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

int	should_continue(enum e_type state, char *line, size_t i)
{
	if (state == SQTS && line[i] && line[i] != (char)state)
		return (1);
	if (line[i] != SQTS && line[i] != DQTS && line[i - 1] == '$' && line[i]
		&& !(ft_isalpha(line[i]) || line[i] == '_' || line[i] == '?'))
		return (1);
	if (line[i - 1] != '$' && line[i] && state == DQTS
		&& line[i] != (char)state)
		return (1);
	if (line[i - 1] != '$' && line[i] && line[i] != DQTS && line[i] != SQTS)
		return (1);
	return (0);
}
void	expand_line(char **result, char *line)
{
	enum e_type	state;
	size_t		beg;
	size_t		end;
	size_t		i;

	i = 0;
	beg = 0;
	state = line[beg];
	while (line[i++])
	{
		if (should_continue(state, line, i))
			continue ;
		end = i;
		while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		i += (line[i] && line[i] == '?');
		beg += (state == SQTS || state == DQTS) && (line[beg] == (char)state);
		append_string(result, ft_substr(line, beg, end - beg - (line[i]
					&& line[i - 1] == '$')));
		append_string(result, ft_substr(line, end, i - end));
		i += (state == SQTS || state == DQTS) && (line[i] == (char)state);
		if (line[i] == SQTS || line[i] == DQTS)
			state = line[i];
		beg = i;
	}
}
