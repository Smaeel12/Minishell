/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:50:42 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/24 23:47:15 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int unset_env(char *key)
{
	size_t key_len;
	size_t i;

	i = 0;
	key_len = ft_strlen(key);
	while (g_data.environs[i])
	{
		if (!ft_strncmp(g_data.environs[i], key, key_len) && (!g_data.environs[i][key_len] || g_data.environs[i][key_len] == '='))
		{
			free(g_data.environs[i]);
			while (g_data.environs[++i])
				g_data.environs[i - 1] = g_data.environs[i];
			g_data.environs[i - 1] = g_data.environs[i];
			g_data.env_size -= 1;
			return (0);
		}
		i++;
	}
	return (0);
}

int set_env(char *new_var)
{
	char **new_array;
	size_t key_len;
	char *key;
	int i;

	i = -1;
	key_len = ft_strchr(new_var, '=') - new_var;
	key = ft_substr(new_var, 0, key_len);
	while (g_data.environs[++i] && !(!ft_strncmp(g_data.environs[i], key,
												 key_len) &&
									 (!g_data.environs[i][key_len] || g_data.environs[i][key_len] == '=')))
		;
	free(key);
	if (g_data.environs[i])
		return (free(g_data.environs[i]), g_data.environs[i] = new_var, 0);
	i = -1;
	new_array = malloc((g_data.env_size + 2) * sizeof(char *));
	ft_bzero(new_array, (g_data.env_size + 2) * sizeof(char *));
	while (g_data.environs[++i])
		new_array[i] = g_data.environs[i];
	new_array[i] = new_var;
	g_data.env_size += 1;
	free(g_data.environs);
	g_data.environs = new_array;
	return (0);
}

char *get_env(char *key)
{
	size_t key_len;
	size_t i;

	i = 0;
	key_len = ft_strlen(key);
	if (key[i] == '?')
		return (free(key), ft_itoa(WEXITSTATUS(g_data.exit_status)));
	while (g_data.environs[i] && !(!ft_strncmp(g_data.environs[i], key, key_len) && g_data.environs[i][key_len] == '='))
		i++;
	if (g_data.environs[i])
		return (free(key), ft_strdup(&g_data.environs[i][key_len + 1]));
	return (free(key), NULL);
}

int print_env(void)
{
	size_t i;

	i = 0;
	while (g_data.environs && g_data.environs[i])
	{
		if (ft_strchr(g_data.environs[i], '='))
			printf("%s\n", g_data.environs[i]);
		i++;
	}
	return (0);
}

int init_env(void)
{
	extern char **environ;
	size_t size;
	size_t i;

	i = 0;
	size = 0;
	while (environ[size++])
		;
	g_data.env_size = size;
	g_data.paths = ft_split(getenv("PATH"), ':');
	g_data.environs = malloc(size * sizeof(char *));
	ft_bzero(g_data.environs, size * sizeof(char *));
	while (g_data.environs && environ[i])
	{
		g_data.environs[i] = ft_strdup(environ[i]);
		i++;
	}
	return (0);
}
