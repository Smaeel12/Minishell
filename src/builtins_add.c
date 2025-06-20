/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_add.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:04:01 by iboubkri          #+#    #+#             */
/*   Updated: 2025/06/19 12:35:42 by iboubkri         ###   ########.fr       */
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
	if (argv[1] && argv[1][0] == '=')
		return (ft_putendl_fd("missing key", 2), 1);
	set_env(argv[1]);
	return (0);
}
