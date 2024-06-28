/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:18:10 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/28 13:48:25 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute(char **env, char **cmd);

// Execute a single command
void	execute(char **env, char **cmd);

int	execution(t_cmds *cmds, char **env, t_exec *exec)
{
	int			pipefd[2];
	int			prev_fd;
	int			i;

	prev_fd = -1;
	i = -1;
	// if (cmds->size == 1 && is_builtin(cmds[0].cmd_args[0]))
	// {
	// 	handle_builtin(cmds, 0, env, exec);
	// 	return (0);
	// }
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

