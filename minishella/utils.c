/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 10:51:09 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/11 11:02:58 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirection_error_checks(char *line)
{
	int	i;

	if (strstr(line, "<>") != NULL)
		return (1);
	if (strstr(line, "><") != NULL)
		return (1);
	if (strstr(line, "<<<") != NULL)
		return (1);
	if (strstr(line, ">>>") != NULL)
		return (1);
	i = -1;
	while (line[++i] != '\0')
	{
		if (line[i] == '>' && line[i+1] == '>' && line[i+2] == '>')
			return (1);
		if (line[i] == '<' && line[i+1] == '<' && line[i+2] == '<')
			return (1);
	}
	return (0);
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
	int		y;
	char	**duplicate;

	i = 0;
	y = -1;
	while (str[i] != NULL)
		i++;
	duplicate = malloc(sizeof(char *) * (i + 1));
	duplicate[i] = NULL;
	while (str[++y] != NULL)
		duplicate[y] = ft_strdup(str[y]);
	return (duplicate);
}

int	is_builtin_count(char *cmd)
{
	return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "pwd") == 0);
}

void count_commands(t_cmds *cmds, char **env)
{
	int		i;
	int		y;
	int		count;
	char	*path;

	count = 0;
	i = 0;
	while (i < cmds->size)
	{
		y = 0;
		if (cmds[i].cmd_args[y] != NULL)
		{
			if (is_builtin_count(cmds[i].cmd_args[y]))
				count++;
			else
			{
				path = get_path(env, cmds[i].cmd_args[y]);
				if (path != NULL)
				{
					count++;
					if (path != cmds[i].cmd_args[y])
						free(path);
				}
			}
		}
		i++;
	}
	cmds->cmd_number = count;
}