/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_helpers2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 20:37:20 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/27 20:38:54 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_strnlen(const char *s, size_t maxlen)
{
	size_t	len;

	len = 0;
	while (len < maxlen && s[len])
		len++;
	return (len);
}

static int	handle_builtin_count(t_cmds *cmds, int i, int *count)
{
	if (is_builtin_count(cmds[i].cmd_args[0]))
	{
		(*count)++;
		return (1);
	}
	return (0);
}

static void	handle_path_count(t_cmds *cmds, char **env, int i, int *count)
{
	char	*path;

	path = get_path(env, cmds[i].cmd_args[0]);
	if (path != NULL)
	{
		(*count)++;
		if (path != cmds[i].cmd_args[0])
			free(path);
	}
}

int	count_commands_helper(t_cmds *cmds, char **env)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < cmds->size)
	{
		if (cmds[i].cmd_args[0] != NULL)
		{
			if (!handle_builtin_count(cmds, i, &count))
				handle_path_count(cmds, env, i, &count);
		}
		i++;
	}
	return (count);
}
