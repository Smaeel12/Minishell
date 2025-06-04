/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:25:14 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/02 23:44:16 by iboubkri         ###   ########.fr       */
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

void	bexit(char **argv, char **envp)
{
	(void)argv;
	(void)envp;
	exit(0);
}

void	env(char **argv, char **envp)
{
	size_t	i;

	(void)argv;
	i = 0;
	while (envp[i])
		printf("%s\n", envp[i++]);
	exit(0);
}

void	pwd(char **argv, char **envp)
{
	char	*buf;

	(void)argv;
	(void)envp;
	buf = NULL;
	printf("%s\n", getcwd(buf, 0));
	exit(0);
}

void	echo(char **argv, char **envp)
{
	size_t	newline;
	size_t	i;

	(void)envp;
	i = 0;
	newline = 1;
	if (argv[1] && argv[1][i++] == '-')
	{
		while (argv[1][i] == 'n')
			i++;
	}
	if (!argv[1][i])
		newline = 0;
	i = 1 + !newline;
	while (argv[i])
	{
		if (i > 1 + !newline)
			printf(" ");
		printf("%s", argv[i++]);
	}
	if (newline)
		printf("\n");
	exit(0);
}

void	cd(char **argv, char **envp)
{
	(void)envp;
	printf("changing directory\n");
	if (!argv[1])
	{
		ft_putendl_fd("missing path", 2);
		exit(1);
	}
	chdir(argv[1]);
	exit(0);
}
