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

#define VALID_TOKENS "<>()\"'*|"

enum e_tokentype
{
	SCAN,
	WORD,
	INRDR = '<',
	OUTRDR = '>',
	INSQTS = '\'',
	INDQTS = '"',
	OPEN_PARENT = '(',
	CLOSE_PARENT = ')',
};

enum e_tokentype advance(char c)
{
	if (ft_strchr(VALID_TOKENS, c))
		return c;
	if (c && c != ' ')
		return WORD;
	return SCAN;
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

static char *st_line;

typedef struct s_token
{
	char *value;
	enum e_tokentype type;
} t_token;

t_token *ft_strtok_a()
{
	enum e_tokentype cstate;
	enum e_tokentype state;
	t_token *token;
	size_t i;

	i = 0;
	state = advance(st_line[i]);
	while (st_line[i])
	{
		cstate = advance(st_line[i]);
		if (state != cstate && state == SCAN)
		{
			state = cstate;
			st_line = &st_line[i];
			i = 0;
		}
		while ((state == INDQTS || state == INSQTS) && state != cstate && st_line[i])
			cstate = advance(st_line[i++]);
		if (state != cstate)
		{
			token = (t_token *)malloc(sizeof(t_token));
			token->value = ft_substr(st_line, 0, i);
			token->type = state;
			st_line = &st_line[i];
			return token;
		}
		i++;
	}
	return NULL;
}

int main()
{
	char cmd[] = "< infile ((cmd \"arg seprated\" || cmd) && cmd > /dev/stdout > file) | cmd";
	t_token *token;

	st_line = cmd;
	while ((token = ft_strtok_a()))
	{
		printf("%s %#x\n", token->value, token->type);
		free(token->value);
		free(token);
	}
}