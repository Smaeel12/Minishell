/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:09:10 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/16 15:14:50 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	add_token(t_list **lst, enum e_type state, char *line, size_t idx)
{
	t_token	*token;

	if (state == NONE)
		return (0);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (1);
	token->type = state;
	token->value = ft_substr(line, 0, idx);
	ft_lstadd_back(lst, ft_lstnew(token));
	return (0);
}

char	*create_line(char **strs, size_t nstrs)
{
	size_t	total_len;
	size_t	offset;
	char	*result;
	size_t	i;

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
			offset += ft_strlcpy(result + offset, strs[i], ft_strlen(strs[i])
					+ 1);
		free(strs[i]);
	}
	return (result[total_len] = '\0', result);
}

char	*concatenate_string(char *line)
{
	char	*command;
	size_t	len;
	size_t	i;
	size_t	j;

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
	return (free(line), command);
}

int	expand_line(char **result, char *line)
{
	char	*key;
	size_t	start;
	size_t	i;

	i = 0;
	while (line[i])
	{
		start = i;
		while (line[start] == SQTS && line[++i] != SQTS)
			;
		while (line[++i] && !(line[i - 1] == '$' && (ft_isalpha(line[i])
					|| line[i] == '_' || line[i] == '?')))
			;
		key = &line[i];
		while (*key && line[++i] && (ft_isalnum(line[i]) || line[i] == '_'))
			;
		key = ft_substr(key, 0, (&line[i] - key));
		*result = create_line((char *[]){*result, ft_substr(line, start, i
					- start - ft_strlen(key) - (*key != '\0')), get_env(key)},
				3);
		free(key);
	}
	*result = concatenate_string(*result);
	return (0);
}
