#include "include/main.h"

char *expand_line(char *line, bool skip_mode, bool expand_mode);

char *get_env(char *key)
{
	char *value;
	size_t i;

	i = 0;
	if (key[i] == '?')
		return (free(key), ft_strdup("0"));
	value = getenv(key);
	if (value)
		value = ft_strdup(value);
	return (free(key), value);
}

int main(int ac, char **av)
{
	if (ac < 2)
		return (0);
	char *delim = expand_line(av[1], false, false);
	printf("%s\n", delim);
	free(delim);
	return (0);
}