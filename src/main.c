/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/14 19:45:03 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int print_token(char *line, size_t start, size_t end)
{
	while (start < end)
		putc(line[start++], stdout);
	putc('\n', stdout);
	return (0);
}

// void tokenizer(char *line, t_list **tokens, enum e_tokentype *state)
// {
// 	enum e_tokentype cstate;
// 	size_t s;
// 	size_t i;

// 	i = 0;
// 	s = 0;
// 	while (1)
// 	{
// 		cstate = advance(line[i]);
// 		while ((*state == INDQTS || *state == INSQTS) && *state != cstate && line[i])
// 			cstate = advance(line[i++]);
// 		if ((*state == INDQTS || *state == INSQTS) && *state == cstate)
// 			cstate = advance(line[i]);
// 		if (cstate != *state || *state == OPEN_PARENT || *state == CLOSE_PARENT)
// 		{
// 			if (*state != SCAN)
// 				ft_lstadd_back(tokens, ft_lstnew(ft_substr(line, s, i - s)));
// 			*state = cstate;
// 			s = i;
// 		}
// 		if (!line[i])
// 			break;
// 		i++;
// 	}
// }

// int main()
// {
// 	t_list *tokens = NULL;
// 	enum e_tokentype state = SCAN;

// 	// char cmd[] = "((((cat < input1.txt && grep \"pattern closed\" input2.txt) || (ls *.txt | sort > output.txt)";
// 	// char cmd[] = "(cmd > output.txt)";
// 	// char cmd[] = "aerqr  <<  ((((echo 'This is a test \"with quotes\" and spaces))   ) ) ) )anything you mean";
// 	char cmd[] = "< infile ((cmd || cmd) && cmd > /dev/stdout > file) | cmd";

// 	tokenizer(cmd, &tokens, &state);
// 	if (state)
// 		printf("Unclosed Quotes\n");
// 	ft_lstiter(tokens, (void (*)(void *))puts);
// 	ft_lstclear(&tokens, free);
// }

#define VALID_TOKENS "<>()\"'*|"

enum e_type
{
	SCAN,
	WORD,
	PIPE = '|',
	INRDR = '<',
	ANDOP = '&',
	INDQTS = '"',
	OUTRDR = '>',
	INSQTS = '\'',
	ASETRIC = '*',
	OPARENTHSIS = '(',
	CPARENTHSIS = ')',
};

typedef struct s_token
{
	char *value;
	enum e_type type;
} t_token;

enum e_type advance(char c)
{
	if (ft_strchr(VALID_TOKENS, c))
		return c;
	if (c && c != ' ')
		return WORD;
	return SCAN;
}

t_token *ft_strtok_s(char *line)
{
	static char *st_line;
	enum e_type cstate;
	enum e_type state;
	t_token *token;
	size_t i;

	if (line)
		st_line = line;
	if (st_line == NULL || *st_line == '\0')
		return NULL;
	i = 0;
	state = advance(st_line[i]);
	token = (t_token *)malloc(sizeof(t_token));
	while (st_line[i])
	{
		if (state == SCAN)
		{
			while (st_line[i] && st_line[i] == ' ')
				st_line++;
			i = 0;
			state = advance(st_line[i]);
		}
		if (state == OPARENTHSIS || state == CPARENTHSIS)
		{
			token->value = ft_substr(st_line, 0, i + 1);
			token->type = state;
			st_line++;
			return token;
		}
		if (state == INSQTS || state == INDQTS)
		{
			while (1)
			{
				cstate = advance(st_line[++i]);
				if (state == cstate || !st_line[i])
					break;
			}
			token->value = ft_substr(st_line, 0, i + 1);
			token->type = state;
			st_line = &st_line[i + 1];
			return token;
		}
		if (state == WORD || state == PIPE || state == INRDR || state == OUTRDR || state == ANDOP)
		{
			while (1)
			{
				cstate = advance(st_line[++i]);
				if (state != cstate || !st_line[i])
					break;
			}
			token->value = ft_substr(st_line, 0, i);
			token->type = state;
			st_line = &st_line[i];
			return token;
		}
	}
	return (free(token), NULL);
}

int main(int ac, char **av)
{
	t_token *token;

	if (ac != 2)
		return (0);
	token = ft_strtok_s(av[1]);
	while (1)
	{
		if (!token)
			break;
		printf("type: %#x || value: %s\n", token->type, token->value);
		free(token->value);
		free(token);
		token = ft_strtok_s(NULL);
	}
}