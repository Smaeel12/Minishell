#include "libft/libft.h"
#include <stdio.h>

char *ft_shell_expand(char *line, size_t lstatus);

int main(int ac, char **av)
{
    char *str;

    if (ac != 2)
        return (0);
    str = ft_shell_expand(av[1], 0);
    if (str)
        puts(str);
    free(str);
    return (0);
}

char *expand_line(char *origin, char *line, char *value)
{
    size_t origin_len;
    size_t value_len;
    size_t line_len;
    char *newline;

    value_len = 0;
    origin_len = 0;
    line_len = ft_strlen(line);
    if (value)
        value_len = ft_strlen(value);
    if (origin)
        origin_len = ft_strlen(origin);
    newline = (char *)malloc((line_len + value_len + origin_len + 1) * sizeof(char));
    if (!newline)
        return NULL;
    if (origin)
        ft_strlcpy(newline, origin, origin_len + 1);
    ft_strlcpy(newline + origin_len, line, line_len + 1);
    if (value)
        ft_strlcpy(newline + origin_len + line_len, value, value_len + 1);
    return (free(origin), free(value), newline);
}

char *ft_shell_expand(char *line, size_t lstatus)
{
    char *result;
    char *value;
    char *key;
    size_t i;

    result = NULL;
    while (*line)
    {
        i = 0;
        while (line[++i] && !(line[i - 1] == '$' &&
                              (ft_isalpha(line[i]) || line[i] == '_' || line[i] == '?')))
            ;
        key = &line[i];
        line[i - (*key && line[i - 1] == '$')] = '\0';
        while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
            i++;
        key = ft_substr(key, 0, (&line[i] - key) + (line[i] == '?'));
        value = getenv(key);
        if (value)
            value = ft_strdup(value);
        if (key[0] == '?')
            value = ft_itoa(lstatus);
        result = expand_line(result, line, value);
        line = &line[i];
        free(key);
    }
    return (result);
}
