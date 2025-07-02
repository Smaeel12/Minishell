/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:15:11 by iboubkri          #+#    #+#             */
/*   Updated: 2025/07/02 23:26:24 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int init_paths(char *path_line)
{
	clear_array(g_data.paths);
	g_data.paths = ft_split(path_line, ':');
	return (free(path_line), 0);
}

int init_ifs(char *ifs)
{
	free(g_data.ifs);
	g_data.ifs = ifs;
	if (!ifs)
		g_data.ifs = ft_strdup(" \t\n");
	return (0);
}

int init_env(char **environs, size_t size)
{
	char **new_array;
	size_t i;

	i = 0;
	new_array = malloc((size + 1) * sizeof(char *));
	ft_bzero(new_array, (size + 1) * sizeof(char *));
	while (environs[i])
	{
		new_array[i] = environs[i];
		i++;
	}
	g_data.env_size = i;
	free(g_data.environs);
	g_data.environs = new_array;
	return (0);
}

t_cmd *init_shell(void)
{
	extern char **environ;
	size_t size;
	size_t i;

	i = 0;
	size = 0;
	init_ifs(NULL);
	while (environ[size])
		size++;
	g_data.environs = malloc((size + 1) * sizeof(char *));
	ft_bzero(g_data.environs, (size + 1) * sizeof(char *));
	while (environ[i])
	{
		g_data.environs[i] = ft_strdup(environ[i]);
		i++;
	}
	g_data.env_size = size;
	init_paths(get_env(ft_strdup("PATH")));
	static t_cmd(builtins)[] = {{"echo", ft_echo}, {"cd", ft_cd}, {"pwd", ft_pwd}, {"export", ft_export}, {"unset", ft_unset}, {"env", ft_env}, {"exit", ft_exit}, {NULL, NULL}};
	return (builtins);
}
