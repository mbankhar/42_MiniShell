/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 12:01:13 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/11 11:23:05 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_the_token(char **commands)
{
	int		i;
	int		cmd_count;
	char	**filtered_commands;
	int		skip_next;
	int		j;
	int		k;

	i = 0;
	cmd_count = 0;
	filtered_commands = NULL;
	skip_next = 0;
	while (commands[i])
	{
		if (skip_next)
			skip_next = 0;
		else if (ft_strcmp(commands[i], "<") == 0 || ft_strcmp(commands[i], ">") == 0)
			skip_next = 1;
		else if (ft_strcmp(commands[i], "|") != 0)
			cmd_count++;
		i++;
	}
	filtered_commands = (char **)malloc((cmd_count + 1) * sizeof(char *));
	if (!filtered_commands)
		return (NULL);
	i = 0;
	j = 0;
	skip_next = 0;
	while (commands[i])
	{
		if (skip_next)
			skip_next = 0;
		else if (ft_strcmp(commands[i], "<") == 0 || ft_strcmp(commands[i], ">") == 0)
			skip_next = 1;
		else if (ft_strcmp(commands[i], "|") != 0)
		{
			filtered_commands[j] = ft_strdup(commands[i]);
			if (!filtered_commands[j])
			{
				k = -1;
				while (++k < j)
				{
					free(filtered_commands[k]);
				}
				free(filtered_commands);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	filtered_commands[j] = NULL;
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

