/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:03:48 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 17:26:53 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_cmd(t_cmds *cmd)
{
	cmd->fd_in = -2;
	cmd->fd_out = -2;
	cmd->heredoc_content = NULL;
	cmd->heredoc_line_count = 0;
}

void	handle_argument(t_do_shit_data *data, t_cmds *cmd, int *i, int *j)
{
	char			*delimiter;
	t_redirect_data	redirect_data;

	if (strcmp(data->args[*i], "<<") == 0 && data->args[*i + 1])
	{
		delimiter = data->args[*i + 1];
		handle_heredoc_wrapper1(delimiter, cmd, data->env);
		*i += 2;
	}
	else if (data->args[*i][0] == '<' || data->args[*i][0] == '>')
	{
		redirect_data.commands = data->args;
		redirect_data.index = *i;
		redirect_data.cmds = cmd;
		redirect_data.env = data->env;
		redirect_data.shell = data->shell;
		look_for_redirect(&redirect_data);
		*i += 2;
	}
	else
	{
		cmd->cmd_args[*j] = ft_strdup(data->args[*i]);
		(*i)++;
		(*j)++;
	}
}

void	process_single_cmd(t_do_shit_data *data, t_cmds *cmd, int *i)
{
	int	j;

	j = 0;
	initialize_cmd(cmd);
	while (data->args[*i] && data->args[*i][0] != '|')
	{
		handle_argument(data, cmd, i, &j);
	}
	cmd->cmd_args[j] = NULL;
}
