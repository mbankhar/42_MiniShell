/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 18:04:47 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/27 17:57:26 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>

int	*get_exit_helper(void)
{
	static int	exitcode = 0;

	return (&exitcode);
}

static void	handle_empty_cmd(char ***cmd, t_shell *shell)
{
	if ((*cmd)[1] != NULL)
		*cmd = &(*cmd)[1];
	else
	{
		shell->exit_status = 0;
		return ;
	}
}

static void	handle_execve_error(t_shell *shell)
{
	perror("minishell");
	if (errno == ENOENT)
	{
		shell->exit_status = 127;
		exit(127);
	}
	else
	{
		shell->exit_status = 126;
		exit(126);
	}
}

static void	execute_cmd(char **env, char **cmd, t_shell *shell)
{
	char	*path_cmd;

	path_cmd = get_path(env, cmd[0]);
	if (!path_cmd)
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmd[0]);
		shell->exit_status = 127;
		exit(127);
	}
	else
	{
		if (execve(path_cmd, cmd, env) == -1)
			handle_execve_error(shell);
	}
}

void	execute_helper(char **env, char **cmd, t_shell *shell)
{
	t_cmds	cmds;

	cmds.fd_in = -1;
	cmds.fd_out = -1;
	if (handle_redirections_helper(cmd, &cmds, env, shell) == 1)
	{
		shell->exit_status = 1;
		return ;
	}
	if (cmd[0] == NULL || cmd[0][0] == '\0' || strcmp(cmd[0], "$EMPTY") == 0)
		handle_empty_cmd(&cmd, shell);
	expand_in_2darray(&cmd, env);
	remove_quotes(cmd);
	if (cmd[0] == NULL || cmd[0][0] == '\0')
	{
		shell->exit_status = 0;
		return ;
	}
	execute_cmd(env, cmd, shell);
}
