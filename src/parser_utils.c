/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:09:10 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/04 01:36:17 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

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

char *create_line(char **strs, size_t nstrs)
{
	size_t total_len;
	size_t offset;
	char *result;
	size_t i;

	i = -1;
	total_len = 0;
	while (++i < nstrs)
	{
		if (strs[i])
			total_len += ft_strlen(strs[i]);
	}
	i = -1;
	offset = 0;
	result = (char *)malloc((total_len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	while (++i < nstrs)
	{
		if (strs[i])
			offset += ft_strlcpy(result + offset, strs[i], ft_strlen(strs[i]) + 1);
		free(strs[i]);
	}
	return (result[total_len] = '\0', result);
}

char *get_value(char *key, size_t last_status)
{
	char *value;

	value = getenv(key);
	if (value)
		value = ft_strdup(value);
	if (key[0] == '?')
		value = ft_itoa(last_status);
	return (free(key), value);
}

char *join_string(char *line)
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
	return (command);
}

char *expand_line(t_token *token, size_t last_status)
{
	char *result;
	size_t start;
	char *key;
	size_t i;

	i = 0;
	result = NULL;
	while (token->value[i])
	{
		start = i;
		while ((token->type == SQTS || token->value[start] == '\'') && token->value[++i] != '\'')
			;
		if (i > 0)
			result = create_line((char *[]){result, ft_substr(token->value, start + 1, i - 1)}, 2);

		start = i;
		while (token->value[++i] && !(token->value[i - 1] == '$' && (ft_isalpha(token->value[i]) || token->value[i] == '_' || token->value[i] == '?')))
			;

		if (token->value[i])
		{
			key = &token->value[i];
			while (*key && token->value[++i] && (ft_isalnum(token->value[i]) || token->value[i] == '_'))
				;
			key = ft_substr(key, 0, (&token->value[i] - key));
			result = create_line((char *[]){result, ft_substr(token->value, start, i - start - ft_strlen(key) - 1), get_value(key, last_status)}, 3);
		}
	}
	return (free(token->value), token->value = join_string(result), free(result), token->value);
}
