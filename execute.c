/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:18:10 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/27 17:04:36 by mbankhar         ###   ########.fr       */
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
	while (++i < exec->num_commands)
	{
		if (i < exec->num_commands - 1 && pipe(pipefd) == -1)
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
			else if (i == exec->num_commands - 1)
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
			if (i < exec->num_commands - 1)
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
			if (i < exec->num_commands - 1)
			{
				prev_fd = pipefd[0];
				close(pipefd[1]);
			}
		}
	}
	i = -1;
	while (++i < exec->num_commands)
	{
		waitpid(cmds[i].pid, NULL, 0);
	}
	return (0);
}



bool	is_builtin(const char *cmd)
{
	static const char *builtins[] =
	{
		"cd", "echo", "export", "unset", "env", "exit", NULL
	};
	for (int i = 0; builtins[i]; ++i)
	{
		if (strcmp(cmd, builtins[i]) == 0)
		{
			return true;
		}
	}
	return false;
}

void	handle_builtin(t_cmds *cmds, int index, char **env, t_exec *exec)
{
	printf("%d", exec->fd_in);
	printf("%d", exec->fd_out);
	const char *cmd = cmds[index].cmd_args[0];
	int arg_count = 0;
	
	// if (exec->fd_in != -2)
	// {
		
	// }
	// if (exec->fd_out != -2)
	// {
		
	// }
	while (cmds[index].cmd_args[arg_count] != NULL)
	{
		arg_count++;
	}
	if (strcmp(cmd, "cd") == 0)
	{
		change_directory(cmds[index].cmd_args[1]);
	} else if (strcmp(cmd, "echo") == 0)
	{
		execute_echo(cmds[index].cmd_args);
	} else if (strcmp(cmd, "export") == 0)
	{
		if (arg_count > 1)
		{
			if (strchr(cmds[index].cmd_args[1], '=') != NULL)
			{
				char *var = strtok(cmds[index].cmd_args[1], "=");
				char *value = strtok(NULL, "=");
				execute_export(var, value ? value : "");
			}
			else
			{
				execute_export(cmds[index].cmd_args[1], "");
			}
		}
		else
		{
			fprintf(stderr, "export: missing argument\n");
		}

	} else if (strcmp(cmd, "unset") == 0)
	{
		execute_unset(cmds[index].cmd_args[1]);
	}
	else if (strcmp(cmd, "env") == 0)
	{
		print_env();
	}
	else if (strcmp(cmd, "exit") == 0)
	{
		exit(0);
	}
}

void execute(char **env, char **cmd)
{
	char *path_cmd = get_path(env, cmd[0]);
	if (!path_cmd)
	{
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(cmd[0], 2);
		exit(EXIT_FAILURE);
	}
	else
	{
		if (execve(path_cmd, cmd, env) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE); // Child exits on failure
		}
	}
}

