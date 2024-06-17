/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:18:10 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/14 10:09:41 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

// Execute a single command
void	execute(char **env, char *cmd);

void	ft_free(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	main(void)
{
	extern char	**environ;
	int			fd_in = -2;
	int			fd_out = -2;
	int			number_of_pipes;
	char		*commands[] = {"grep samurai", "wc -l"};
	int			num_commands = sizeof(commands) / sizeof(commands[0]);
	int			pipefd[2];
	int			prev_fd = -1;
	pid_t		pid;
	char		**env;
	int			i;

	fd_in = open("test", O_RDONLY);
	fd_out = open("test_out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_in < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	env = environ;
	number_of_pipes = num_commands - 1;
	i = -1;
	while (++i < num_commands)
	{
		if (i < number_of_pipes && pipe(pipefd) == -1)
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
				if (fd_in != -2)
					dup2(fd_in, 0);
				dup2(pipefd[1], 1);
			}
			else if (i == num_commands - 1)
			{
				if (fd_out != -2)
					dup2(fd_out, 1);
				dup2(prev_fd, 0);
			}
			else
			{
				dup2(prev_fd, 0);
				dup2(pipefd[1], 1);
			}
			close(fd_in);
			if (prev_fd != -1)
				close(prev_fd);
			if (i < number_of_pipes)
			{
				close(pipefd[0]);
				close(pipefd[1]);
			}
			execute(env, commands[i]);
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (i < number_of_pipes)
			{
				prev_fd = pipefd[0];
				close(pipefd[1]);
			}
		}
	}
	close(fd_in);
	i = -1;
	while (++i < num_commands)
	{
		wait(NULL);
	}
	return (0);
}

void	execute(char **env, char *cmd)
{
	char	*path_cmd;
	char	**cmd_args;

	cmd_args = ft_split(cmd, ' ');
	path_cmd = get_path(env, cmd_args[0]);
	if (execve(path_cmd, cmd_args, env) == -1)
	{
		perror("execve");
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(cmd_args[0], 2);
		ft_free(cmd_args);
		exit(EXIT_FAILURE);
	}
}
