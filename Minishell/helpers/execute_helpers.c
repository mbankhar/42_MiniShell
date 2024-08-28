/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 12:52:22 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 14:48:53 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>

int	is_builtin_helper(t_cmds *cmds)
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
	else if (ft_strcmp(cmds[0].cmd_args[0], "pwd") == 0)
		return (1);
	else
		return (0);
}

static void	setup_redirect_data(t_redirect_data *data, int i,
		t_redirect_helper_data *helper_data)
{
	data->commands = helper_data->cmd;
	data->index = i;
	data->cmds = helper_data->cmds;
	data->env = helper_data->env;
	data->shell = helper_data->shell;
}

static int	handle_redirect(int *i, t_redirect_helper_data *helper_data)
{
	t_redirect_data	data;

	setup_redirect_data(&data, *i, helper_data);
	look_for_redirect(&data);
	if (helper_data->shell->exit_status == 1)
		return (1);
	*i += 2;
	return (0);
}

int	handle_redirections_helper(char **cmd, t_cmds *cmds, char **env,
		t_shell *shell)
{
	int						i;
	t_redirect_helper_data	helper_data;

	i = 0;
	helper_data.cmd = cmd;
	helper_data.cmds = cmds;
	helper_data.env = env;
	helper_data.shell = shell;
	while (cmd[i] != NULL)
	{
		if (cmd[i][0] == '<' || cmd[i][0] == '>')
		{
			if (handle_redirect(&i, &helper_data))
				return (1);
		}
		else
			i++;
	}
	return (0);
}

int	execute_command(t_cmds *cmds, char ***env, t_shell *shell)
{
	if (ft_strcmp(cmds[0].cmd_args[0], "cd") == 0)
		return (change_directory(cmds[0].cmd_args[1], env, shell));
	else if (ft_strcmp(cmds[0].cmd_args[0], "echo") == 0)
		return (execute_echo(cmds, *env, shell));
	else if (ft_strcmp(cmds[0].cmd_args[0], "export") == 0)
		return (execute_export(cmds, env));
	else if (ft_strcmp(cmds[0].cmd_args[0], "unset") == 0)
		return (execute_unset(cmds, env));
	else if (ft_strcmp(cmds[0].cmd_args[0], "env") == 0)
	{
		print_env(*env);
		return (0);
	}
	else if (ft_strcmp(cmds[0].cmd_args[0], "pwd") == 0)
		return (execute_pwd(cmds[0].cmd_args));
	return (1);
}
