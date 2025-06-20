/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:25:14 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/20 10:53:47 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int ft_exit(char **argv)
{
	clear_array(s_data.paths);
	clear_array(s_data.environs);
	if (argv[1])
		exit(ft_atoi(argv[1]));
	exit(0);
}

int ft_env(char **argv)
{
	(void)argv;
	print_env();
	return (0);
}

int ft_pwd(char **argv)
{
	char *buf;

	(void)argv;
	buf = NULL;
	printf("%s\n", getcwd(buf, 0));
	return (0);
}

int ft_echo(char **argv)
{
	size_t newline;
	size_t i;
	size_t j;

	i = 1;
	newline = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i] && argv[i][0] == '-' && argv[i][++j] == 'n')
			;
		if ((argv[i] && argv[i][j] != '\0' && j > 1) || argv[i][0] != '-')
			break;
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i++], 1);
		write(1, " ", !!argv[i]);
	}
	write(1, "\n", newline);
	return (0);
}

int ft_cd(char **argv)
{
	if (!argv[1])
		return (ft_putendl_fd("missing path", 2), 1);
	if (chdir(argv[1]) == -1)
		return (perror("cd"), 1);
	return (0);
}
