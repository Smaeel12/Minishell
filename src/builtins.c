/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:25:14 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/20 11:20:28 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	ft_exit(char **argv)
{
	int	i;
	int	j;
	int	is_alpha;

	i = 0;
	j = 0;
	is_alpha = 0;
	while (argv[i])
		i++;
	while (i > 1 && argv[1][j])
		if (ft_isalpha(argv[1][j++]))
			is_alpha = 1;
	printf("exit\n");
	if (i > 2 && !is_alpha)
		return (ft_putendl_fd("exit: too many arguments", 2), 1);
	clear_array(g_data.paths);
	clear_array(g_data.environs);
	if (is_alpha)
	{
		print_err("exit: ", argv[1], ": numeric argument required");
		exit(2);
	}
	else if (i > 1)
		exit((unsigned char)ft_atoi(argv[1]));
	exit(0);
}

int	ft_env(char **argv)
{
	(void)argv;
	print_env();
	return (0);
}

int	ft_pwd(char **argv)
{
	char	*buf;

	(void)argv;
	buf = getcwd(NULL, 0);
	if (!buf)
		return (perror("getcwd"), 1);
	printf("%s\n", buf);
	free(buf);
	return (0);
}

int	ft_echo(char **argv)
{
	size_t	newline;
	size_t	i;
	size_t	j;

	i = 1;
	newline = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][0] == '-' && argv[i][++j] == 'n')
			;
		if ((argv[i][j] != '\0') || (argv[i][0] == '-' && j == 1))
			break ;
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

int	ft_cd(char **argv)
{
	char	*owd;
	char	*cwd;
	int		i;

	i = 0;
	while (argv[i])
		i++;
	if (i < 2)
		return (0);
	else if (i == 2)
	{
		owd = getcwd(NULL, 0);
		if (!owd)
			return (perror("getcwd"), 1);
		if (chdir(argv[1]))
			return (perror("cd"), 1);
		cwd = getcwd(NULL, 0);
		if (!cwd)
			return (perror("getcwd"), 1);
		set_env(ft_strjoin("PWD=", cwd));
		set_env(ft_strjoin("OLDPWD=", owd));
		return (free(owd), free(cwd), 0);
	}
	else
		return (ft_putendl_fd("cd: too many arguments", 2), 1);
}
