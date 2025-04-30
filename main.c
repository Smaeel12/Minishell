/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:53:44 by iboubkri          #+#    #+#             */
/*   Updated: 2025/04/29 18:56:16 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int clear_2d_arr(t_path *paths)
{
	int i;

	i = 0;
	while (paths && paths[i])
		free(paths[i++]);
	free(paths);
	return (0);
}

t_cmd *create_command(t_cmd line, t_path *paths)
{
	t_cmd *cmd_args;
	size_t path_len;
	size_t cmd_len;
	size_t i;

	i = 0;
	cmd_args = ft_split(line, ' ');
	if (cmd_args[0][0] == '/' || cmd_args[0][1] == '/')
		return (cmd_args);
	cmd_len = ft_strlen(cmd_args[0]);
	while (paths[i])
	{
		free(cmd_args[0]);
		path_len = ft_strlen(paths[i]);
		cmd_args[0] = (char *)malloc((path_len + cmd_len + 2) * sizeof(char));
		ft_strlcpy(cmd_args[0], paths[i], path_len + 1);
		cmd_args[0][path_len] = '/';
		ft_strlcpy(cmd_args[0] + path_len + 1, line, cmd_len + 1);
		if (!access(cmd_args[0], X_OK))
			return (cmd_args);
		i++;
	}
	ft_putstr_fd(line, 2);
	ft_putstr_fd(CMD_NF, 2);
	return (clear_2d_arr(cmd_args), NULL);
}

int execute_command(t_cmd line, t_path *paths)
{
	t_cmd *cmd_args;
	pid_t pid;

	printf("%s\n", line);
	cmd_args = create_command(line, paths);
	if (!cmd_args)
		return (1);
	pid = fork();
	if (!pid)
	{
		execve(cmd_args[0], cmd_args, __environ);
		perror(line);
		return (clear_2d_arr(cmd_args), clear_2d_arr(paths), exit(1), 1);
	}
	return (wait(NULL), clear_2d_arr(cmd_args), 0);
}

typedef struct s_cmdline
{
	char *infile;
	char **cmd_args;
	char *outfile;
} t_cmdline;

int main(int ac, char **av)
{
	t_list *command_list;
	char **pipes;
	t_cmdline *cmdline;

	command_list = NULL;
	if (ac < 2 || ac > 2)
		return (0);
	pipes = ft_split(av[1], '|');
	for (size_t i = 0; pipes && pipes[i]; i++)
	{
		cmdline = (t_cmdline *)malloc(sizeof(t_cmdline));
		cmdline->cmd_args = ft_split(pipes[i], '<');
		cmdline->infile = cmdline->cmd_args[1];
		ft_lstadd_back(&command_list, ft_lstnew(cmdline));
	}
}