/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:48:20 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/02 12:00:51 by amohame2         ###   ########.fr       */
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
		exit(1);
	}
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
			break ;
		free(string);
	}
	ft_free(realpath);
	return (string);
}

char *get_path(char **env, char *cmd) {
    int i;
    char *string;

    i = 0;
    if (ft_strchr(cmd, '/') != NULL)
        return cmd;
    while (env[i]) {
        if (ft_strnstr(env[i], "PATH=", 5) != NULL)
            break;
        i++;
    }
    if (env[i] == NULL) {
        char *default_paths[] = {"/usr/bin", "/bin", NULL};
        for (i = 0; default_paths[i] != NULL; i++) {
            string = ft_strjoinslesh(default_paths[i], cmd);
            if (access(string, X_OK) == 0)
                return string;
            free(string);
        }
        return NULL;
    }
    else
        return altpath(env, cmd, i);
}

