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

void	print_err(char *cmd, char *arg, char *msg)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(msg, 2);
}

int	ft_unset(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
		unset_env(argv[i++]);
	return (0);
}

int	ft_export(char **argv)
{
	int	i;
	int	j;
	int	ret;

	i = 0;
	j = 0;
	ret = 0;
	while (argv[i])
		i++;
	while (i == 1 && g_data.environs[j])
		printf("declare -x %s\n", g_data.environs[j++]);
	j = 1;
	while (i > 1 && argv[j])
	{
		if (!ft_isalpha(argv[j][0]) && !(argv[j][0] == '_'))
		{
			print_err("export: `", argv[j++], "': not a valid identifier");
			ret = 1;
		}
		else
			set_env(ft_strdup(argv[j++]));
	}
	return (ret);
}
