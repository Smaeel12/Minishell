/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:02:57 by iboubkri          #+#    #+#             */
/*   Updated: 2025/05/26 10:58:49 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

typedef struct
{
    int infile;
    int outfile;
} t_streams;

t_streams *open_files(char **redirections, size_t nrdr)
{
    t_streams *streams;
    size_t i;

    streams = (t_streams *)malloc(sizeof(t_streams));
    streams->infile = -1;
    streams->outfile = -1;
    if (!nrdr)
    {
        streams->infile = 0;
        streams->outfile = 1;
        return streams;
    }
    i = 0;
    while (i < nrdr)
    {
        if (streams->infile != -1)
            close(streams->infile);
        if (streams->outfile != -1)
            close(streams->outfile);
        if (redirections[i][0] == '<' && !redirections[i][1])
        {
            streams->infile = open(redirections[++i], O_RDONLY);
            if (streams->infile == -1)
                return ft_putstr_fd(redirections[i], 2), ft_putendl_fd(": file not found", 2), free(streams), NULL;
        }
        if (redirections[i][0] == '>')
        {
            if (redirections[i][1] == '>')
                streams->outfile = open(redirections[++i], O_WRONLY | O_APPEND);
            else
                streams->outfile = open(redirections[++i], O_WRONLY | O_TRUNC);
            if (streams->outfile == -1)
                return ft_putstr_fd(redirections[i], 2), ft_putendl_fd(": file not found", 2), free(streams), NULL;
        }
        i++;
    }
    return streams;
}

void run_command(char **command, t_streams *streams)
{
    extern char **environ;
    pid_t pid;

    pid = fork();
    if (pid == -1)
        return;
    if (pid == 0)
    {
        dup2(streams->infile, 0);
        dup2(streams->outfile, 1);
        execve(command[0], command, environ);
        exit(1);
    }
}

int execute_pipeline(t_tree *tree)
{
    t_streams *streams;

    if (tree == NULL)
        return 0;
    if (tree->type == NODE_COMMAND)
    {
        streams = open_files(tree->command.redirections, tree->command.ridx);
        run_command(tree->command.arguments, streams);
        free(streams);
    }
    else
    {
        execute_pipeline(tree->operator.left);
        execute_pipeline(tree->operator.right);
    }
    return (0);
}