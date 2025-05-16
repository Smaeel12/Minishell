/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/16 10:00:43 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	main(int ac, char **av)
{
	t_token	*token;

	if (ac != 2)
		return (0);
	token = tokenizer(av[1]);
	while (1)
	{
		if (!token)
			break ;
		printf("type: %#x || value: %s\n", token->type, token->value);
		free(token->value);
		free(token);
		token = tokenizer(NULL);
	}
}
