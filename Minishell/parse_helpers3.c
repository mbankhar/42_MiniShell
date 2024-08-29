/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 18:54:22 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 20:56:02 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_command_fields(t_cmds *cmd)
{
	cmd->fd_in = -2;
	cmd->fd_out = -2;
	cmd->heredoc_content = NULL;
	cmd->heredoc_line_count = 0;
}

void	process_single_argument(t_do_shit_data *data, t_cmds *cmd, int *i,
		int *j)
{
	char	*delimiter;

	if (strcmp(data->args[*i], "<<") == 0 && data->args[*i + 1])
	{
		delimiter = data->args[*i + 1];
		if (strlen(delimiter) >= MAX_DELIMITER_LENGTH)
		{
			fprintf(stderr, "Error: Delimiter too long\n");
		}
		else
		{
			handle_heredoc(delimiter, cmd, data->env);
		}
		*i += 2;
	}
	else
	{
		process_funny_argument1(data, cmd, i, j);
	}
}

void	process_funny_argument1(t_do_shit_data *data, t_cmds *cmd, int *i,
		int *j)
{
	t_redirect_data	redirect_data;

	if (data->args[*i][0] == '<' || data->args[*i][0] == '>')
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
		cmd->cmd_args[(*j)++] = ft_strdup(data->args[(*i)++]);
	}
}

void	process_arguments(t_do_shit_data *data, t_cmds *cmds)
{
	int	i;
	int	j;
	int	k;

	i = -1;
	k = 0;
	while (data->args[++i])
	{
		j = 0;
		initialize_command_fields(&cmds[k]);
		while (data->args[i] && data->args[i][0] != '|')
		{
			process_single_argument(data, &cmds[k], &i, &j);
		}
		cmds[k].cmd_args[j] = NULL;
		k++;
		if (!data->args[i])
			break ;
	}
	cmds->size = k;
}
