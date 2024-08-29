/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 11:11:08 by mbankhar          #+#    #+#             */
/*   Updated: 2024/08/28 17:21:22 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_export(t_cmds *cmds, char ***env)
{
	int	i;
	int	exit_code;

	exit_code = 0;
	i = 1;
	if (cmds->cmd_args[1] == NULL)
	{
		print_envs(*env);
		return (0);
	}
	while (cmds->cmd_args[i] != NULL)
	{
		exit_code |= process_export_arg(cmds->cmd_args[i], env);
		i++;
	}
	return (exit_code);
}

int	process_export_arg(char *arg, char ***env)
{
	char	var[256];
	char	*value;
	int		append;

	extract_var_value(arg, var, &value, &append);
	if (is_valid_identifier(var))
	{
		return (handle_export_var(var, value, append, env));
	}
	else
	{
		fprintf(stderr, "export: `%s': not a valid identifier\n", arg);
		return (1);
	}
}

int	handle_export_var(char *var, char *value, int append, char ***env)
{
	int	idx;

	if (value != NULL || append)
	{
		idx = find_env_var(*env, var);
		if (idx != -1)
		{
			update_var(env, idx, var, value);
		}
		else
		{
			add_new_var(env, var, value);
		}
	}
	else
	{
		idx = find_env_var(*env, var);
		if (idx == -1)
		{
			add_new_var(env, var, NULL);
		}
	}
	return (0);
}

void	update_var(char ***env, int idx, const char *var, const char *value)
{
	t_update_var_params	params;

	params.env = env;
	params.idx = idx;
	params.var = var;
	params.value = value;
	if (value != NULL && strchr(value, '+') != NULL)
	{
		params.append = 1;
	}
	else
	{
		params.append = 0;
	}
	update_existing_var(&params);
}

int	execute_unset(t_cmds *cmds, char ***env)
{
	int		i;
	char	*var;
	int		exit_code;

	exit_code = 0;
	i = 1;
	if (cmds->cmd_args[i] == NULL)
	{
		return (0);
	}
	while (cmds->cmd_args[i] != NULL)
	{
		var = cmds->cmd_args[i];
		if (is_valid_identifier(var))
		{
			remove_env_var(env, var);
		}
		else
		{
			fprintf(stderr, "unset: `%s': not a valid identifier\n", var);
			exit_code = 1;
		}
		i++;
	}
	return (exit_code);
}
