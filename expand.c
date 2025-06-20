#include "include/main.h"

void print_segment(char *line, size_t start, size_t end)
{
    for (size_t j = start; j < end; j++)
        putc(line[j], stdout);
    putc('\n', stdout);
}

char *ft_strjoin_helper(char *s1, char *s2)
{
    char *result;

    if (!s1)
        return (s2);
    if (!s2)
        return (s1);
    result = ft_strjoin(s1, s2);
    return (free(s1), free(s2), result);
}

char *get_env(char *key)
{
    char *value;

    value = getenv(key);
    if (value)
        return (free(key), ft_strdup(value));
    return (free(key), NULL);
}

char *expanding_line(char *line)
{
    size_t i = 0;
    size_t start = 0;
    size_t shift = 0;
    char *key = NULL;
    char *result = NULL;

    while (line[i++])
    {
        shift = 0;

        if (line[start] != SQTS && line[i - 1] == '$' &&
            (ft_isalpha(line[i]) || line[i] == '_' || line[i] == '?'))
        {
            key = &line[i];
            shift = line[start] == DQTS || line[start] == SQTS;
            result = ft_strjoin_helper(result,
                                       ft_substr(line, start + shift, i - start - 1));
            while (line[++i] && (ft_isalnum(line[i]) || line[i] == '_'))
                ;
            key = ft_substr(key, 0, (&line[i] - key));
            result = ft_strjoin_helper(result, get_env(key));
            start = i + shift;
        }
        if (line[i] == '\0' || line[i] == SQTS || line[i] == DQTS)
        {
            if (((line[start] == SQTS || line[start] == DQTS) &&
                 line[start] != line[i]) || i <= start)
                continue;
            shift = line[start] == DQTS || line[start] == SQTS;
            result = ft_strjoin_helper(result,
                                       ft_substr(line, start + shift, (i - start - shift)));
            start = i + shift;
        }
    }
    return (result);
}

int main(int ac, char **av)
{
    if (ac != 2)
        return (1);

    char *result = expanding_line(av[1]);
    printf("%s\n", result);
    free(result);
}
