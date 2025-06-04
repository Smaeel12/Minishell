/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_add.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:04:01 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/30 22:06:42 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

void	unset(char **argv, char **envp)
{
	(void)argv;
	(void)envp;
	exit(0);
}

void	export(char **argv, char **envp)
{
	(void)argv;
	(void)envp;
	exit(0);
}
