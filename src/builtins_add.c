/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_add.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:04:01 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/17 06:11:55 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	ft_unset(char **argv)
{
	char	*key;

	key = argv[1];
	unset_env(key);
	return (0);
}

int	ft_export(char **argv)
{
	char	**key_value;

	if (argv[1] && argv[1][0] == '=')
		return (ft_putendl_fd("missing key", 2), 1);
	key_value = ft_split(argv[1], '=');
	set_env(key_value[0], key_value[1]);
	free(key_value);
	return (0);
}
