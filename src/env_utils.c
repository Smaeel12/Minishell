/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:50:42 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/18 07:40:34 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	unset_env(char *key)
{
	size_t	key_len;
	size_t	i;

	i = 0;
	key_len = ft_strlen(key);
	while (s_data.environs[i])
	{
		if (!ft_strncmp(s_data.environs[i], key, key_len)
			|| s_data.environs[i][key_len] == '=')
		{
			free(s_data.environs[i]);
			while (s_data.environs[i++])
				s_data.environs[i - 1] = s_data.environs[i];
			s_data.environs[i] = NULL;
			s_data.env_size -= 1;
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
	while (s_data.environs[i] && !(!ft_strncmp(s_data.environs[i], key, key_len)
			&& s_data.environs[i][key_len] == '='))
		i++;
	if (s_data.environs[i])
		return (free(s_data.environs[i]), s_data.environs[i] = new_env, 0);
	i = -1;
	new_array = malloc((s_data.env_size + 2) * sizeof(char *));
	ft_bzero(new_array, (s_data.env_size + 2) * sizeof(char *));
	while (s_data.environs[++i])
		new_array[i] = s_data.environs[i];
	s_data.env_size += 1;
	free(s_data.environs);
	new_array[i] = new_env;
	s_data.environs = new_array;
	return (0);
}

char	*get_env(char *key)
{
	size_t	key_len;
	size_t	i;

	i = 0;
	key_len = ft_strlen(key);
	if (key[i] == '?')
		return (ft_itoa(WEXITSTATUS(s_data.exit_status)));
	while (s_data.environs[i] && !(!ft_strncmp(s_data.environs[i], key, key_len)
			&& s_data.environs[i][key_len] == '='))
		i++;
	if (s_data.environs[i])
		return (ft_strdup(&s_data.environs[i][key_len + 1]));
	return (NULL);
}

int	print_env(void)
{
	size_t	i;

	i = 0;
	while (s_data.environs && s_data.environs[i])
		printf("%s\n", s_data.environs[i++]);
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
	s_data.env_size = size;
	s_data.paths = ft_split(getenv("PATH"), ':');
	s_data.environs = malloc(size * sizeof(char *));
	ft_bzero(s_data.environs, size * sizeof(char *));
	while (s_data.environs && environ[i])
	{
		s_data.environs[i] = ft_strdup(environ[i]);
		i++;
	}
	return (0);
}
