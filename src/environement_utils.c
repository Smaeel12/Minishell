/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:50:42 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/13 22:45:59 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	print_env(void)
{
	size_t	i;

	i = 0;
	while (g_data.environs && g_data.environs[i])
	{
		printf("%s\n", g_data.environs[i]);
		i++;
	}
	return (0);
}

int	init_env(void)
{
	extern char	**environ;
	size_t		size;
	size_t		i;

	i = 0;
	size = 0;
	if (!g_data.environs)
	{
		while (environ[size++])
			;
		g_data.environs = malloc(size * sizeof(char *));
	}
	while (g_data.environs && environ[i])
	{
		g_data.environs[i] = ft_strdup(environ[i]);
		i++;
	}
	g_data.env_size = size;
	g_data.environs[i] = NULL;
	return (0);
}

char	*get_env(char *key)
{
	size_t	key_len;
	size_t	i;

	i = 0;
	if (key[i] == '?')
		return (ft_itoa(WEXITSTATUS(g_data.exit_status)));
	key_len = ft_strlen(key);
	while (g_data.environs[i] && !(!ft_strncmp(g_data.environs[i], key, key_len)
			&& g_data.environs[i][key_len] == '='))
		i++;
	if (g_data.environs[i])
		return (ft_strdup(&g_data.environs[i][key_len + 1]));
	return (NULL);
}

int	set_env(char *key, char *value)
{
	size_t	key_len;
	size_t	i;

	i = 0;
	key_len = ft_strlen(key);
	while (g_data.environs[i])
	{
		if (!ft_strncmp(g_data.environs[i], key, key_len)
			|| g_data.environs[i][key_len] == '=')
		{
			free(g_data.environs[i]);
			g_data.environs[i] = create_line((char *[]){key, ft_strdup("="),
					value}, 3);
			return (0);
		}
		i++;
	}
	clear_env();
	if (i > g_data.env_size)
		g_data.environs = malloc((i + 2) * sizeof(char *));
	init_env();
	g_data.environs[i] = create_line((char *[]){key, ft_strdup("="), value}, 3);
	g_data.environs[i + 1] = NULL;
	g_data.env_size += 1;
	return (0);
}

int	unset_env(char *key)
{
	size_t	key_len;
	size_t	i;

	i = 0;
	key_len = ft_strlen(key);
	while (g_data.environs[i])
	{
		if (!ft_strncmp(g_data.environs[i], key, key_len)
			|| g_data.environs[i][key_len] == '=')
		{
			free(g_data.environs[i]);
			while (g_data.environs[i + 1])
			{
				g_data.environs[i] = g_data.environs[i + 1];
				i++;
			}
			g_data.environs[i] = NULL;
			g_data.env_size -= 1;
			return (0);
		}
		i++;
	}
	return (0);
}
