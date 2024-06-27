/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:48:20 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/27 14:27:24 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*altpath(char **env, char *cmd, int i)
{
	char	**realpath;
	char	*string;

	realpath = ft_split(env[i], ':');
	realpath[0] = ft_substr(realpath[0], 5, ft_strlen(realpath[0]) - 4);
	if (realpath[0] == NULL)
	{
		ft_free(realpath);
		// exit(1);
	}
	i = -1;
	while (realpath[++i])
	{
		string = ft_strjoinslesh(realpath[i], cmd);
		if (string == NULL)
		{
			ft_free(realpath);
			// exit(1);
		}
		if (access(string, X_OK) == 0)
			break ;
		free(string);
	}
	ft_free(realpath);
	return (string);
}

char	*get_path(char **env, char *cmd)
{
	int		i;
	char	*string;

	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5) != NULL)
		{
			break ;
		}
		i++;
	}
	if (env[i] == NULL)
	{
		string = cmd;
		if (access(string, X_OK) != 0)
		{
			free(string);
			// exit(1);
		}
	}
	else
		string = altpath(env, cmd, i);
	return (string);
}

