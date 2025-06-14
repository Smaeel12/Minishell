/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:25:14 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/14 21:52:36 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

// ◦ echo with option -n
// ◦ cd with only a relative or absolute path
// ◦ pwd with no options
// ◦ export with no options
// ◦ unset with no options
// ◦ env with no options or arguments
// ◦ exit with no options

int bexit(char **argv)
{
	clear_env();
	if (argv[1])
		exit(ft_atoi(argv[1]));
	exit(0);
}

int env(char **argv)
{
	(void)argv;
	print_env();
	return (0);
}

int pwd(char **argv)
{
	char *buf;

	(void)argv;
	buf = NULL;
	printf("%s\n", getcwd(buf, 0));
	return (0);
}

int echo(char **argv)
{
	size_t newline;
	size_t i;
	size_t j;

	i = 1;
	j = 0;
	newline = 1;
	while (argv[i] && argv[i][0] == '-' && argv[i][j] == 'n')
		j++;
	if (argv[i] && !argv[i][j])
		newline = 0;
	while (argv[i])
	{
		printf("%s", argv[i++]);
		write(1, " ", !!argv[i]);
	}
	write(1, "\n", newline);
	return (0);
}

int cd(char **argv)
{
	if (!argv[1])
		return (ft_putendl_fd("missing path", 2), 1);
	if (chdir(argv[1]) == -1)
		return (perror("cd"), 1);
	return (0);
}
