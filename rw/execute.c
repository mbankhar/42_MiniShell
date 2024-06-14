/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 10:00:00 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/14 11:40:35 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_command(Command *cmd) {
    int pipe_fds[MAX_COMMANDS - 1][2];
    pid_t pid[MAX_COMMANDS];
    int num_pipes = cmd->num_commands - 1;

    // Create pipes
    for (int i = 0; i < num_pipes; i++) {
        if (pipe(pipe_fds[i]) == -1) {
            perror("minishell: pipe");
            exit(1);
        }
    }

    // Iterate over commands and fork
    for (int i = 0; i < cmd->num_commands; i++) {
        pid[i] = fork();
        if (pid[i] == 0) { // Child process
            // If not the first command, read from the previous pipe
            if (i > 0) {
                dup2(pipe_fds[i - 1][0], STDIN_FILENO);
            }
            // If not the last command, write to the next pipe
            if (i < num_pipes) {
                dup2(pipe_fds[i][1], STDOUT_FILENO);
            }

            // Close all pipe file descriptors
            for (int j = 0; j < num_pipes; j++) {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
            }

            // Execute the command
            execvp(cmd->commands[i].tokens[0], cmd->commands[i].tokens);
            perror("minishell: execvp");
            exit(1);
        } else if (pid[i] < 0) { // Fork failed
            perror("minishell: fork");
            exit(1);
        }
    }

    // Parent process: close all pipe file descriptors
    for (int i = 0; i < num_pipes; i++) {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < cmd->num_commands; i++) {
        waitpid(pid[i], NULL, 0);
    }
}
