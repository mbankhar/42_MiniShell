/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers03.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 14:24:44 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 14:52:05 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execution_helper(t_cmds *cmds, char ***env, t_shell *shell)
{
	int	prev_fd;

	prev_fd = -1;
	if (cmds->cmd_number == 1 && is_builtin(cmds))
	{
		shell->exit_status = execute_builtin(cmds, env, shell);
		cmds->exit_code = shell->exit_status;
		return (shell->exit_status);
	}
	else
	{
		execution_loop(cmds, env, shell, &prev_fd);
		wait_for_children(cmds, shell);
	}
	return (shell->exit_status);
}

void	execution_loop(t_cmds *cmds, char ***env, t_shell *shell, int *prev_fd)
{
	t_exec_params_local	params;
	int					i;

	params.cmds = cmds;
	params.env = env;
	params.shell = shell;
	params.prev_fd = prev_fd;
	i = -1;
	while (++i < cmds->size)
	{
		params.i = i;
		if (i < cmds->size - 1 && pipe(params.pipefd) == -1)
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
		handle_fork_local(&params);
	}
}

void	handle_fork_local(t_exec_params_local *params)
{
	if (params->cmds[params->i].pid == 0)
	{
		handle_child_process(params->cmds, params->env, params->shell,
			params->i);
	}
	else
	{
		handle_parent_process(params->prev_fd, params->pipefd, params->i,
			params->cmds->size);
	}
}
