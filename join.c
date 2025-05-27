#include <stdlib.h>
#include "libft/libft.h"

char *create_line(char **strs)
{
    size_t total_len;
    size_t offset;
    size_t srclen;
    char *result;
    size_t i;

    i = 0;
    offset = 0;
    total_len = 0;
    while (strs[i])
        total_len += ft_strlen(strs[i++]);
    i = 0;
    result = (char *)malloc((total_len + 1) * sizeof(char));
    if (!result)
        return NULL;
    while (strs[i])
    {
        srclen = ft_strlen(strs[i]);
        ft_memcpy(result + offset, strs[i], srclen);
        offset += srclen;
        i++;
    }
    result[total_len] = '\0';
    return result;
}

#include <stdio.h>
int main()
{
    char *result = join_strs((char *[]){"hello", " world", " this is a test", NULL});
    printf("%s\n", result);
    free(result);
    return (0);
}