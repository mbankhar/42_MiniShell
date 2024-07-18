/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:18:10 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/18 16:20:43 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute(char **env, char **cmd);

int	is_builtin(t_cmds *cmds)
{
	if (ft_strcmp(cmds[0].cmd_args[0], "cd") == 0)
		return (1);
	else if (ft_strcmp(cmds[0].cmd_args[0], "echo") == 0)
		return (1);
	else if (ft_strcmp(cmds[0].cmd_args[0], "export") == 0)
		return (1);
	else if (ft_strcmp(cmds[0].cmd_args[0], "unset") == 0)
		return (1);
	else if (ft_strcmp(cmds[0].cmd_args[0], "env") == 0)
		return (1);
	// else if (ft_strcmp(cmds[0].cmd_args[0], "pwd") == 0)
	// 	return (1);
	else
		return (0);
}
void	execute_builtin(t_cmds *cmds, char ***env)
{
	if (ft_strcmp(cmds[0].cmd_args[0], "cd") == 0)
		change_directory(cmds[0].cmd_args[1], env);
	else if (ft_strcmp(cmds[0].cmd_args[0], "echo") == 0)
		execute_echo(cmds, *env);
	else if (ft_strcmp(cmds[0].cmd_args[0], "export") == 0)
		execute_export(cmds, env);
	else if (ft_strcmp(cmds[0].cmd_args[0], "unset") == 0)
	{
		if (cmds[0].cmd_args[1] == NULL)
			fprintf(stderr, "unset: missing argument\n");
		else
			execute_unset(cmds, env);
	}
	else if (ft_strcmp(cmds[0].cmd_args[0], "env") == 0)
		print_env(*env);
}

// Execute a single command
void	execute(char **env, char **cmd);

int	*get_exit(void)
{
	static int	exitcode = 0;

	return (&exitcode);
}

int execution(t_cmds *cmds, char ***env)
{
    int pipefd[2];
    int prev_fd;
    int i;
    int status;
    int heredoc_pipe[2];

    prev_fd = -1;
    i = -1;
    if (cmds->cmd_number == 1 && is_builtin(cmds))
        execute_builtin(cmds, env);
    else
    {
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
                        dup2(cmds[i].fd_in, STDIN_FILENO);
                    if (cmds[i].fd_out != -2)
                        dup2(cmds[i].fd_out, STDOUT_FILENO);
                    else
                        dup2(pipefd[1], STDOUT_FILENO);
                }
                else if (i == cmds->size - 1)
                {
                    if (cmds[i].fd_in != -2)
                        dup2(cmds[i].fd_in, STDIN_FILENO);
                    else
                        dup2(prev_fd, STDIN_FILENO);
                    if (cmds[i].fd_out != -2)
                        dup2(cmds[i].fd_out, STDOUT_FILENO);
                }
                else
                {
                    if (cmds[i].fd_in != -2)
                        dup2(cmds[i].fd_in, STDIN_FILENO);
                    else
                        dup2(prev_fd, STDIN_FILENO);
                    if (cmds[i].fd_out != -2)
                        dup2(cmds[i].fd_out, STDOUT_FILENO);
                    else
                        dup2(pipefd[1], STDOUT_FILENO);
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
                if (cmds[i].heredoc_content)
                {
                    if (pipe(heredoc_pipe) == -1)
                    {
                        perror("pipe");
                        exit(EXIT_FAILURE);
                    }
                    for (int j = 0; j < cmds[i].heredoc_line_count; j++)
                    {
                        if (cmds[i].heredoc_content[j])
                        {
                            write(heredoc_pipe[1], cmds[i].heredoc_content[j],
                                strlen(cmds[i].heredoc_content[j]));
                            write(heredoc_pipe[1], "\n", 1);
                        }
                    }
                    close(heredoc_pipe[1]);
                    dup2(heredoc_pipe[0], STDIN_FILENO);
                    close(heredoc_pipe[0]);
                }
                execute(*env, cmds[i].cmd_args);
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
            waitpid(cmds[i].pid, &status, 0);
            free_heredoc_content(&cmds[i]);
        }
        cmds->exit_code = status;
    }
    return (0);
}

	void execute(char **env, char **cmd)
	{
		char *path_cmd;

		expand_in_2darray(&cmd, env);
		remove_quotes(cmd);
		path_cmd = get_path(env, cmd[0]);
		if (!path_cmd)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putendl_fd(cmd[0], 2);
			ft_putstr_fd(" ", 2);
			ft_putstr_fd("command not found", 2);
			write(2, "\n", 1);
			exit(EXIT_FAILURE);
		}
		else
		{
			if (execve(path_cmd, cmd, env) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
	}
