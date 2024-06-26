/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:18:10 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/26 14:59:09 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Execute a single command
void	execute(char **env, char **cmd);

int	execution(t_cmds *cmds, char **env, t_exec *exec)
{
	int			pipefd[2];
	int			prev_fd;
	pid_t		pid;
	int			i;

	prev_fd = -1;
	i = -1;
	// printf("COMES HERE");
	while (++i <= exec->number_of_pipes)
	{
		if (i < exec->number_of_pipes && pipe(pipefd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			if (i == 0)
			{
				if (exec->fd_in != -2)
					dup2(exec->fd_in, 0);
				dup2(pipefd[1], 1);
			}
			else if (i == exec->num_commands - 1)
			{
				if (exec->fd_out != -2)
					dup2(exec->fd_out, 1);
				dup2(prev_fd, 0);
			}
			else
			{
				dup2(prev_fd, 0);
				dup2(pipefd[1], 1);
			}
			close(exec->fd_in);
			if (prev_fd != -1)
				close(prev_fd);
			if (i < exec->number_of_pipes)
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
			if (i < exec->number_of_pipes)
			{
				prev_fd = pipefd[0];
				close(pipefd[1]);
			}
		}
	}
	close(exec->fd_in);
	i = -1;
	while (++i < exec->num_commands)
	{
		wait(NULL);
	}
	return (0);
}

void	execute(char **env, char **cmd)
{
	char	*path_cmd;
	char	**cmd_args;

	path_cmd = get_path(env, cmd[0]);
	if (execve(path_cmd, cmd, env) == -1)
	{
		perror("execve");
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(cmd[0], 2);
		// ft_free(cmd);
		// exit(EXIT_FAILURE);
	}
}
