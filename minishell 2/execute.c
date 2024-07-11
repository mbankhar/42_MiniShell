/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:18:10 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/11 12:40:20 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	// else if (ft_strcmp(cmds[0].cmd_args[0], "exit") == 0)
	// 	return (1);
	else
		return (0);
}
void	execute_builtin(t_cmds *cmds, char ***env)
{
	if (ft_strcmp(cmds[0].cmd_args[0], "cd") == 0)
	{
		if (cmds[0].cmd_args[1] == NULL)
			fprintf(stderr, "cd: missing argument\n");
		else
			change_directory(cmds[0].cmd_args[1], env);
	}
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


int	execution(t_cmds *cmds, char ***env)
{
	int			pipefd[2];
	int			prev_fd;
	int			i;
	int			status;

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
			waitpid(cmds[i].pid, get_exit_status(), 0);
		}
	}
	return (0);
}

void	execute(char **env, char **cmd)
{
	char	*path_cmd;

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

