/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:09:10 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/25 15:38:10 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

void clear_tree(t_tree *tree)
{
    if (tree == NULL || tree->type == NODE_COMMAND)
    {
        free(tree);
        return;
    }
    clear_tree(tree->operator.left);
    clear_tree(tree->operator.right);
    free(tree);
}

t_token *create_token(enum e_token_type type, char *line, size_t len)
{
    t_token *token;

    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = ft_substr(line, 0, len);
    return (token);
}

char *create_line(char *origin, char *line, char *value)
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

char *expand_line(char *line, size_t last_status)
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
            value = ft_itoa(last_status);
        result = create_line(result, line, value);
        line = &line[i + (line[i] == '?')];
        free(key);
    }
    return (result);
}

int is_word(t_token *token, size_t last_status)
{
    char *new_string;

    if (token->type != SQTS && token->type != DQTS && token->type != WORD)
        return (0);
    if (token->type == DQTS || token->type == WORD)
    {
        new_string = expand_line(token->value, last_status);
        free(token->value);
        token->value = new_string;
    }
    token->type = WORD;
    return (1);
}