/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 12:01:13 by mbankhar          #+#    #+#             */
/*   Updated: 2024/08/21 14:42:20 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_the_token(char **commands)
{
	int		cmd_count;
	char	**filtered_commands;

	cmd_count = count_commands1(commands);
	filtered_commands = filter_commands(commands, cmd_count);
	return (filtered_commands);
}

char	*get_env_value(char **environ, const char *var)
{
	int		index;
	size_t	len;

	index = 0;
	len = ft_strlen(var);
	while (environ[index])
	{
		if (ft_strncmp(environ[index], var, len) == 0
			&& environ[index][len] == '=')
		{
			return (environ[index] + len + 1);
		}
		index++;
	}
	return (NULL);
}

int	get_token_number(char **tokens, char **env)
{
	int		count;
	char	*path;
	int		i;

	i = -1;
	count = 0;
	while (tokens[++i] != NULL)
	{
		path = get_path(env, tokens[i]);
		if (path != NULL)
		{
			count++;
			free(path);
		}
	}
	return (count);
}
