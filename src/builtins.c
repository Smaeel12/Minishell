/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:25:14 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/06 06:16:33 by iboubkri         ###   ########.fr       */
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

void bexit(char **argv, char **envp)
{
	(void)argv;
	(void)envp;
	exit(0);
}

void env(char **argv, char **envp)
{
	size_t i;

	(void)argv;
	i = 0;
	while (envp[i])
		printf("%s\n", envp[i++]);
	exit(0);
}

void pwd(char **argv, char **envp)
{
	char *buf;

	(void)argv;
	(void)envp;
	buf = NULL;
	printf("%s\n", getcwd(buf, 0));
	exit(0);
}

void echo(char **argv, char **envp)
{
	size_t i = 0;

	(void)envp;
	while (argv[i])
		printf("%s\n", argv[i++]);
	exit(0);
}

void cd(char **argv, char **envp)
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
