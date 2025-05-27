/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:02:57 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/27 14:41:49 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

char *find_command(char *line, char **paths)
{
    char *command;
    size_t i;

    i = 0;
    command = ft_strdup(line);
    while (paths[i])
    {
        if (!access(command, X_OK) && command[0] == '/')
            return command;
        free(command);
        command = create_line((char *[]){paths[i], "/", line}, 3);
        i++;
    }
    return (NULL);
}

int open_streams(int *streams, char **redirections)
{
    size_t i;

    i = 0;
    while (redirections[i])
    {
        if (redirections[i][0] == '<' && redirections[i][1] == '\0')
        {
            streams[0] = open(redirections[++i], O_RDONLY);
            dup2(streams[0], 0);
        }

        if (redirections[i][0] == '>' && redirections[i][1] == '\0')
        {
            streams[1] = open(redirections[++i], O_CREAT | O_WRONLY | O_TRUNC, 0644);
            dup2(streams[1], 1);
        }
        if (redirections[i][0] == '>' && redirections[i][1] == '\0')
        {
            streams[1] = open(redirections[++i], O_CREAT | O_WRONLY | O_APPEND, 0644);
            dup2(streams[1], 1);
        }
        i++;
    }
    return (0);
}

int execute_commands(t_tree *tree, char **paths, int *streams, int unused)
{
    extern char **environ;
    char *command;
    int pipefds[2];
    pid_t pid;

    command = NULL;
    if (tree->type == NODE_COMMAND)
    {
        pid = fork();
        if (pid == -1)
            return (1);
        if (pid == 0)
        {
            open_streams(streams, tree->command.redirections);
            if (unused > 0)
                close(unused);
            close(streams[0]);
            close(streams[1]);
            command = find_command(tree->command.arguments[0], paths);
            printf("command found:: %s\n", command);
            execve(command, tree->command.arguments, environ);
            perror(tree->command.arguments[0]);
            free(command);
            exit(1);
        }
    }
    else
    {
        if (pipe(pipefds) == -1)
            return (1);
        execute_commands(tree->operator.left, paths, pipefds, 0);
        execute_commands(tree->operator.right, paths, pipefds, 1);
    }
    return (0);
}
