/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:03:17 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/13 12:43:54 by iboubkri         ###   ########.fr       */
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

#include "../libft/libft.h"

#include <limits.h>

// typedef char *t_cmd;
// typedef char *t_path;

// #define CMD_NF ": command not found\n"
// #define PERM_IN ": Permission denied\n"

// typedef enum e_token_state
// {
// 	NORMAL,
// 	WORD,
// 	PIPE = '|',
// 	BACKGROUND = '&',
// 	REDIRECT_IN = '<',
// 	REDIRECT_OUT = '>',
// 	DOUBLE_QUOTE = '"',
// 	SINGLE_QUOTE = '\'',
// 	OPEN_PARENTHESIS = '(',
// 	CLOSE_PARENTHESIS = ')',
// } t_token_state;

#endif