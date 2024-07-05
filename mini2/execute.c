/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:18:10 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/05 21:44:51 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute(char **env, char **cmd);
void execute_builtin(char **cmd);

// Execute a single command
void	execute(char **env, char **cmd);

int	is_builtin_command(char *cmd) {
    if (strcmp(cmd, "cd") == 0 ||
        strcmp(cmd, "echo") == 0 ||
        strcmp(cmd, "export") == 0 ||
        strcmp(cmd, "unset") == 0 ||
        strcmp(cmd, "env") == 0) {
        return 1;
    }
    return 0;
}

void execute_builtin(char **cmd) {
    if (strcmp(cmd[0], "cd") == 0) {
        if (cmd[1] == NULL) {
            fprintf(stderr, "cd: missing argument\n");
        } else {
            change_directory(cmd[1]);
        }
    } else if (strcmp(cmd[0], "echo") == 0) {
        execute_echo(cmd);
    } else if (strcmp(cmd[0], "export") == 0) {
        if (cmd[1] == NULL) {
            fprintf(stderr, "export: missing argument\n");
        } else {
            char *var = strtok(cmd[1], "=");
            char *value = strtok(NULL, "=");
            if (value != NULL) {
                execute_export(var, value);
            } else if (value == NULL && strchr(cmd[1], '=')) {
                execute_export(var, "");
            }
        }
    } else if (strcmp(cmd[0], "unset") == 0) {
        if (cmd[1] == NULL) {
            fprintf(stderr, "unset: missing argument\n");
        } else {
            execute_unset(cmd[1]);
        }
    } else if (strcmp(cmd[0], "env") == 0) {
        print_env();
    }
}

int	execution(t_cmds *cmds, char **env, t_exec *exec)
{
	int			pipefd[2];
	int			prev_fd;
	int			i;

	prev_fd = -1;
	i = -1;
	while (++i < cmds->size)
	{
		if (i < cmds->size - 1 && pipe(pipefd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		cmds[i].pid = fork();
		if (cmds[i].pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (cmds[i].pid == 0)
		{
			if (i == 0)
			{
				if (cmds[i].fd_in != -2)
					dup2(cmds[i].fd_in, 0);
				if (cmds[i].fd_out != -2)
					dup2(cmds[i].fd_out, 1);
				else
					dup2(pipefd[1], 1);
			}
			else if (i == cmds->size - 1)
			{
				if (cmds[i].fd_in != -2)
					dup2(cmds[i].fd_in, 0);
				else
					dup2(prev_fd, 0);
				if (cmds[i].fd_out != -2)
					dup2(cmds[i].fd_out, 1);
			}
			else
			{
				if (cmds[i].fd_in != -2)
					dup2(cmds[i].fd_in, 0);
				else
					dup2(prev_fd, 0);
				if (cmds[i].fd_out != -2)
					dup2(cmds[i].fd_out, 1);
				else
					dup2(pipefd[1], 1);
			}
			if (cmds[i].fd_in != -2)
				close(cmds[i].fd_in);
			if (cmds[i].fd_out != -2)
				close(cmds[i].fd_out);
			if (prev_fd != -1)
				close(prev_fd);
			if (i < cmds->size - 1)
			{
				close(pipefd[0]);
				close(pipefd[1]);
			}
			execute(env, cmds[i].cmd_args);
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (i < cmds->size - 1)
			{
				prev_fd = pipefd[0];
				close(pipefd[1]);
			}
		}
	}
	i = -1;
	while (++i < cmds->size)
	{
		waitpid(cmds[i].pid, NULL, 0);
	}
	return (0);
}

void	execute(char **env, char **cmd)
{
    if (is_builtin_command(cmd[0])) {
        execute_builtin(cmd);
        return;
    }
	

    char	*path_cmd;

    path_cmd = get_path(env, cmd[0]);
    if (!path_cmd)
    {
        ft_putstr_fd("pipex: command not found: ", 2);
        ft_putendl_fd(cmd[0], 2);
        exit(EXIT_FAILURE);
    }
    else
    {
        remove_quotes(cmd);
        if (execve(path_cmd, cmd, env) == -1)
        {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    }
}
