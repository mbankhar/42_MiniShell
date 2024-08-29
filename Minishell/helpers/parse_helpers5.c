/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 16:53:58 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 18:13:35 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	allocate_cmds(t_do_shit_data *data, t_cmds **cmds)
{
	*cmds = malloc((data->exec->number_of_pipes + 1) * sizeof(t_cmds));
	if (*cmds == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
}

void	allocate_cmd_args(t_do_shit_data *data, t_cmds *cmds)
{
	int	i;
	int	k;
	int	j;

	i = -1;
	k = 0;
	while (data->args[++i])
	{
		j = count_cmd_args(data->args, &i);
		cmds[k].cmd_args = malloc(sizeof(char *) * (j + 1));
		if (cmds[k].cmd_args == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		k++;
		if (!data->args[i])
			break ;
	}
}

void	handle_heredoc_wrapper1(char *delimiter, t_cmds *cmd, char **env)
{
	if (strlen(delimiter) >= MAX_DELIMITER_LENGTH)
		fprintf(stderr, "Error: Delimiter too long\n");
	else
		handle_heredoc(delimiter, cmd, env);
}
