/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:50:42 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/17 15:39:29 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	unset_env(char *key)
{
	size_t	key_len;
	size_t	i;

	i = 0;
	key_len = ft_strlen(key);
	while (data.environs[i])
	{
		if (!ft_strncmp(data.environs[i], key, key_len)
			|| data.environs[i][key_len] == '=')
		{
			free(data.environs[i]);
			while (data.environs[i++])
				data.environs[i - 1] = data.environs[i];
			data.environs[i] = NULL;
			data.env_size -= 1;
			return (0);
		}
		i++;
	}
	return (0);
}

int	set_env(char *key, char *value)
{
	char	**new_array;
	size_t	key_len;
	char	*new_env;
	int		i;

	i = -1;
	key_len = ft_strlen(key);
	new_env = create_line((char *[]){key, ft_strdup("="), value}, 3);
	while (data.environs[i] && !(!ft_strncmp(data.environs[i], key, key_len)
			&& data.environs[i][key_len] == '='))
		i++;
	if (data.environs[i])
		return (free(data.environs[i]), data.environs[i] = new_env, 0);
	i = -1;
	new_array = malloc((data.env_size + 2) * sizeof(char *));
	ft_bzero(new_array, (data.env_size + 2) * sizeof(char *));
	while (data.environs[++i])
		new_array[i] = data.environs[i];
	data.env_size += 1;
	free(data.environs);
	new_array[i] = new_env;
	data.environs = new_array;
	return (0);
}

char	*get_env(char *key)
{
	size_t	key_len;
	size_t	i;

	i = 0;
	key_len = ft_strlen(key);
	if (key[i] == '?')
		return (ft_itoa(WEXITSTATUS(data.exit_status)));
	while (data.environs[i] && !(!ft_strncmp(data.environs[i], key, key_len)
			&& data.environs[i][key_len] == '='))
		i++;
	if (data.environs[i])
		return (ft_strdup(&data.environs[i][key_len + 1]));
	return (NULL);
}

int	print_env(void)
{
	size_t	i;

	i = 0;
	while (data.environs && data.environs[i])
		printf("%s\n", data.environs[i++]);
	return (0);
}

int	init_env(void)
{
	extern char	**environ;
	size_t		size;
	size_t		i;

	i = 0;
	size = 0;
	while (environ[size++])
		;
	data.env_size = size;
	data.paths = ft_split(getenv("PATH"), ':');
	data.environs = malloc(size * sizeof(char *));
	ft_bzero(data.environs, size * sizeof(char *));
	while (data.environs && environ[i])
	{
		data.environs[i] = ft_strdup(environ[i]);
		i++;
	}
	return (0);
}
