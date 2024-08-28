/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 10:51:09 by mbankhar          #+#    #+#             */
/*   Updated: 2024/08/27 21:27:57 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirection_error_checks(char *line)
{
	if (strstr(line, "<>") != NULL || strstr(line, "><") != NULL || strstr(line,
			"<<<") != NULL || strstr(line, ">>>") != NULL)
	{
		return (1);
	}
	return (check_triple_redirection(line));
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

char	**init_env(char **env)
{
	if (env[0] == NULL)
	{
		env[0] = "PWD=/Users/mbankhar/Desktop/myrepo";
		env[1] = "SHLVL=1";
		env[2] = "_=/usr/bin/env";
		env[3] = "PATH=/usr:/bin:/env";
		env[4] = NULL;
	}
	return (env);
}

char	**duplicate_environ(char **str)
{
	int		i;
	char	**duplicate;

	i = 0;
	while (str[i] != NULL)
		i++;
	duplicate = malloc(sizeof(char *) * (i + 1));
	if (!duplicate)
		return (NULL);
	duplicate[i] = NULL;
	i = 0;
	while (str[i] != NULL)
	{
		duplicate[i] = ft_strdup(str[i]);
		i++;
	}
	return (duplicate);
}

void	count_commands(t_cmds *cmds, char **env)
{
	cmds->cmd_number = count_commands_helper(cmds, env);
}
