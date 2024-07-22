/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 11:17:31 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/19 18:50:43 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Function to remove quotes from a string
char	*remove_quotess(const char *str)
{
	size_t		len;
	char		*result;
	char		*dst;
	const char	*src;

	len = strlen(str);
	result = malloc(len + 1);
	if (result == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	dst = result;
	src = str;
	while (*src != '\0')
	{
		if (*src != '\'' && *src != '\"')
		{
			*dst = *src;
			dst++;
		}
		src++;
	}
	*dst = '\0';
	return (result);
}

// Function to get the value of an environment variable from a custom environment
char *get_custom_env_value(const char *name, char **env)
{
	size_t	name_len;
	int		i;

	name_len = ft_strlen(name);
	i = -1;
	while (env[++i] != NULL)
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
		{
			return (env[i] + name_len + 1);
		}
	}
	return (NULL);
}

// Function to expand environment variables in a string
char	*expand_env_variables(const char *str, char **env)
{
	size_t		len;
	char		*result;
	const char	*start;
	const char	*dollar;
	const char	*end;

	len = strlen(str);
	result = malloc(len + 1);
	if (result == NULL)
		return (NULL);
	result[0] = '\0';
	start = str;
	while ((dollar = strchr(start, '$')) != NULL)
	{
		strncat(result, start, dollar - start);
		dollar++;
		end = dollar;
		while (*end && (isalnum(*end) || *end == '_'))
			end++;
		char var_name[128];
		strncpy(var_name, dollar, end - dollar);
		var_name[end - dollar] = '\0';
		char *value = get_custom_env_value(var_name, env);
		if (value)
			strcat(result, value);
		start = end;
	}
	strcat(result, start);
	return (result);
}

// Function to handle redirection
void look_for_redirect(char **commands, int index, t_cmds *cmds, char **env)
{
	int		fd;
	char	*file;
	char	*expanded_file;

	file = remove_quotess(commands[index + 1]);
	if (file == NULL)
		return ;
	expanded_file = expand_env_variables(file, env);
	free(file);
	if (expanded_file == NULL)
		return ;
	if (commands[index][0] == '>' && commands[index][1] == '>')
	{
		fd = open(expanded_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			perror("Failed to open file for appending");
			g_exit_status = 1;
		}
		else
			cmds->fd_out = fd;
	}
	else if (commands[index][0] == '<')
	{
		fd = open(expanded_file, O_RDONLY);
		if (fd == -1)
		{
			perror("Failed to open file for reading");
			g_exit_status = 1;

		}
		else
			cmds->fd_in = fd;
	}
	else if (commands[index][0] == '>')
	{
		fd = open(expanded_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("Failed to open file for writing");
			g_exit_status = 1;

		}
		else
			cmds->fd_out = fd;
	}
	free(expanded_file);
}
