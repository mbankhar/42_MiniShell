/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_helpers3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 20:44:18 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/27 20:44:27 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_command(char *cmd, char **env)
{
	char	*path;

	if (is_builtin_count(cmd))
	{
		return (1);
	}
	else
	{
		path = get_path(env, cmd);
		if (path != NULL)
		{
			if (path != cmd)
				free(path);
			return (1);
		}
	}
	return (0);
}

int	count_commands_helper(t_cmds *cmds, char **env)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (i < cmds->size)
	{
		if (cmds[i].cmd_args[0] != NULL)
		{
			count += is_valid_command(cmds[i].cmd_args[0], env);
		}
		i++;
	}
	return (count);
}
