/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:09:10 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/28 18:14:50 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

t_token	*create_token(enum e_token_type type, char *line, size_t len)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_substr(line, 0, len);
	return (token);
}

char	*create_line(char **strs, size_t nstrs)
{
	size_t	total_len;
	size_t	offset;
	size_t	srclen;
	char	*result;
	size_t	i;

	i = 0;
	offset = 0;
	total_len = 0;
	while (i < nstrs)
	{
		if (strs[i])
			total_len += ft_strlen(strs[i]);
		i++;
	}
	i = 0;
	result = (char *)malloc((total_len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	while (i < nstrs)
	{
		if (strs[i])
		{
			srclen = ft_strlen(strs[i]);
			ft_memcpy(result + offset, strs[i], srclen);
			offset += srclen;
		}
		i++;
	}
	result[total_len] = '\0';
	return (result);
}

char	*expand_line(char *line, size_t last_status)
{
	char	*result;
	char	*value;
	char	*key;
	size_t	i;

	result = NULL;
	while (*line)
	{
		i = 0;
		while (line[++i] && !(line[i - 1] == '$' && (ft_isalpha(line[i])
					|| line[i] == '_' || line[i] == '?')))
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
		result = create_line((char *[]){result, line, value}, 3);
		line = &line[i + (line[i] == '?')];
		free(key);
	}
	return (result);
}
