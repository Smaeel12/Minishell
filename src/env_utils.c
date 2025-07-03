/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:50:42 by iboubkri          #+#    #+#             */
/*   Updated: 2025/07/03 02:11:52 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	unset_env(char *key)
{
	size_t	key_len;
	size_t	i;

	i = 0;
	key_len = ft_strlen(key);
	while (g_data.environs[i])
	{
		if (!ft_strncmp(g_data.environs[i], key, key_len)
			&& (!g_data.environs[i][key_len]
				|| g_data.environs[i][key_len] == '='))
		{
			if (!ft_strncmp(key, "IFS", 5))
				init_ifs(NULL);
			if (!ft_strncmp(key, "PATH", 5))
				init_paths(NULL);
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

int	set_env(char *new_var)
{
	size_t	key_len;
	char	*key;
	int		i;

	i = -1;
	key_len = ft_strchr(new_var, '=') - new_var;
	key = ft_substr(new_var, 0, key_len);
	while (g_data.environs[++i] && !(!ft_strncmp(g_data.environs[i], key,
				key_len) && (!g_data.environs[i][key_len]
				|| g_data.environs[i][key_len] == '=')))
		;
	if (!ft_strncmp(key, "IFS", 5))
		init_ifs(ft_substr(new_var, key_len, ft_strlen(new_var) - key_len));
	if (!ft_strncmp(key, "PATH", 5))
		init_paths(ft_substr(new_var, key_len, ft_strlen(new_var) - key_len));
	free(key);
	if (g_data.environs[i])
		return (free(g_data.environs[i]), g_data.environs[i] = new_var, 0);
	init_env(g_data.environs, (g_data.env_size + 1));
	g_data.environs[g_data.env_size] = new_var;
	g_data.env_size += 1;
	return (0);
}

char	*get_env(char *key)
{
	size_t	key_len;
	size_t	i;

	i = 0;
	key_len = ft_strlen(key);
	if (key[i] == '?')
		return (free(key), ft_itoa(g_data.st_exit));
	while (g_data.environs[i] && !(!ft_strncmp(g_data.environs[i], key, key_len)
			&& g_data.environs[i][key_len] == '='))
		i++;
	if (g_data.environs[i])
		return (free(key), ft_strdup(&g_data.environs[i][key_len + 1]));
	return (free(key), NULL);
}

int	print_env(void)
{
	size_t	i;

	i = 0;
	while (g_data.environs && g_data.environs[i])
	{
		if (ft_strchr(g_data.environs[i], '='))
			printf("%s\n", g_data.environs[i]);
		i++;
	}
	return (0);
}
