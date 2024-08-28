/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 11:17:31 by mbankhar          #+#    #+#             */
/*   Updated: 2024/08/28 16:47:44 by amohame2         ###   ########.fr       */
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

void	handle_redirection(char *file, int type, t_cmds *cmds, t_shell *shell)
{
	int	fd;

	if (type == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (type == 2)
		fd = open(file, O_RDONLY);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(file);
		shell->exit_status = 1;
	}
	else
	{
		if (type == 2 && cmds->fd_in != -1)
			close(cmds->fd_in);
		else if (cmds->fd_out != -1)
			close(cmds->fd_out);
		if (type == 2)
			cmds->fd_in = fd;
		else
			cmds->fd_out = fd;
	}
}

void	look_for_redirect(t_redirect_data *data)
{
	char	*file;
	char	*expanded_file;

	file = remove_quotess(data->commands[data->index + 1]);
	if (file == NULL)
	{
		data->shell->exit_status = 1;
		return ;
	}
	expanded_file = expand_env_variables(file, data->env);
	free(file);
	if (expanded_file == NULL)
	{
		data->shell->exit_status = 1;
		return ;
	}
	if (data->commands[data->index][0] == '>'
		&& data->commands[data->index][1] == '>')
		handle_redirection(expanded_file, 1, data->cmds, data->shell);
	else if (data->commands[data->index][0] == '<')
		handle_redirection(expanded_file, 2, data->cmds, data->shell);
	else if (data->commands[data->index][0] == '>')
		handle_redirection(expanded_file, 3, data->cmds, data->shell);
	free(expanded_file);
}
