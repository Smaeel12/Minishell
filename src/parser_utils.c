/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:09:10 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/14 09:26:42 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int is_valid_token(t_token *token)
{
	return (token->type == DQTS || token->type == SQTS || token->type == WORD || token->type == OUTRDR || token->type == INRDR);
}

int add_token(t_list **lst, enum e_token_type state, enum e_token_type cstate,
			  char *line, size_t len)
{
	t_token *token;

	if (((state == DQTS || state == SQTS || state == WORD) && (cstate == WORD || cstate == DQTS || cstate == SQTS)))
		return (0);
	if (state != SCAN)
	{
		token = (t_token *)malloc(sizeof(t_token));
		if (!token)
			return (1);
		token->type = state;
		token->value = ft_substr(line, 0, len);
		ft_lstadd_back(lst, ft_lstnew(token));
	}
	return (1);
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

char *concatenate_string(char *line)
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
	return (free(line), command);
}

char *expand_line(t_token *token)
{
	char *result;
	char *key;
	size_t start;
	size_t i;

	i = 0;
	result = NULL;
	while (token->value[i])
	{
		start = i;
		while (token->value[start] == SQTS && token->value[++i] != SQTS)
			;
		while (token->value[++i] && !(token->value[i - 1] == '$' && (ft_isalpha(token->value[i]) || token->value[i] == '_' || token->value[i] == '?')))
			;
		key = &token->value[i];
		while (*key && token->value[++i] && (ft_isalnum(token->value[i]) || token->value[i] == '_'))
			;
		key = ft_substr(key, 0, (&token->value[i] - key));
		result = create_line((char *[]){result, ft_substr(token->value, start, i - start - ft_strlen(key) - (*key != '\0')), get_env(key)}, 3);
		free(key);
	}
	return (concatenate_string(result));
}
