/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:48:20 by mbankhar          #+#    #+#             */
/*   Updated: 2024/08/27 18:40:54 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**split_and_substr(char **env, int i)
{
	char	**realpath;

	realpath = ft_split(env[i], ':');
	realpath[0] = ft_substr(realpath[0], 5, ft_strlen(realpath[0]) - 4);
	if (realpath[0] == NULL)
	{
		ft_free(realpath);
		exit(1);
	}
	return (realpath);
}

static char	*find_executable(char **realpath, char *cmd)
{
	char	*string;
	int		i;

	i = -1;
	while (realpath[++i])
	{
		string = ft_strjoinslesh(realpath[i], cmd);
		if (string == NULL)
		{
			ft_free(realpath);
			exit(1);
		}
		if (access(string, X_OK) == 0)
			return (string);
		free(string);
	}
	return (NULL);
}

char	*altpath(char **env, char *cmd, int i)
{
	t_altpath_vars	vars;

	vars.realpath = split_and_substr(env, i);
	vars.string = find_executable(vars.realpath, cmd);
	ft_free(vars.realpath);
	return (vars.string);
}

char	*get_path(char **env, char *cmd)
{
	int		i;
	char	*string;

	i = 0;
	if (ft_strchr(cmd, '/') != NULL)
		return (cmd);
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5) != NULL)
			break ;
		i++;
	}
	if (env[i] == NULL)
	{
		string = cmd;
		if (access(string, X_OK) != 0)
		{
			free(string);
			exit(1);
		}
	}
	else
		string = altpath(env, cmd, i);
	return (string);
}
