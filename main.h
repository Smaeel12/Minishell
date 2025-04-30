/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:03:17 by iboubkri          #+#    #+#             */
/*   Updated: 2025/04/29 01:03:37 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/wait.h>
#include <sys/resource.h>

#include <signal.h>

#include <errno.h>

#include "libft/libft.h"

typedef char *t_cmd;
typedef char *t_path;

#define CMD_NF ": command not found\n"
#define PERM_IN ": Permission denied\n"

#endif