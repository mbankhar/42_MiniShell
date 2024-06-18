/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_dollars_redirect.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 12:01:13 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/18 12:02:01 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_the_token(char **commands, t_exec *exec)
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
		else if (strcmp(commands[i], "<") == 0 || strcmp(commands[i], ">") == 0)
			skip_next = 1;
		else if (strcmp(commands[i], "|") != 0)
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
		else if (strcmp(commands[i], "<") == 0 || strcmp(commands[i], ">") == 0)
			skip_next = 1;
		else if (strcmp(commands[i], "|") != 0)
		{
			filtered_commands[j] = strdup(commands[i]);
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
	len = strlen(var);
	while (environ[index])
	{
		// Check if the current environment variable starts with var=
		if (strncmp(environ[index], var, len) == 0 && environ[index][len] == '=')
		{
			return (environ[index] + len + 1); // Return the value part
		}
		index++;
	}
	return (NULL); // Return NULL if the variable is not found
}

// Function to handle $LOGNAME in the commands array
void	handle_dollar(char **commands, int i, int y, char **environ)
{
	char	*var_name;
	char	*env_value;
	// Extract the variable name from the command (e.g., LOGNAME from $LOGNAME)
	var_name = commands[i] + y + 1;

	// Get the value of the environment variable
	env_value = get_env_value(environ, var_name);
	ft_strcpy(commands[i], env_value);
	if (env_value)
	{
		// Replace the command string with the environment variable's value
		commands[i] = env_value;
	}
}

void	check_dollar(char **commands, t_exec *exec)
{
	extern char	**environ;
	int			i;
	int			y;

	i = -1;
	while (commands[++i])
	{
		y = -1;
		while (commands[i][++y])
		{
			if (commands[i][y] == '$')
			{
				// check_for_single_or_double(commands, i);
				handle_dollar(commands, i, y, environ);
			}
		}
	}
}

void	look_for_redirect(char **commands, t_exec *exec)
{
	int	i;
	int	y;
	i = -1;
	while (commands[++i])
	{
		y = -1;
		while (commands[i][++y])
		{
			if (commands[i][y] == '>' && commands[i][y + 1] == '>')
				exec->fd_out = open(commands[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			// else if (commands[i][y] == '<' && commands[i][y + 1] == '<')
			// 	here_doc();
			else if (commands[i][y] == '<')
				exec->fd_in = open(commands[i + 1], O_RDONLY);
			else if (commands[i][y] == '>')
				exec->fd_out = open(commands[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
	}
}
