/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:41:43 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 20:53:45 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_commands(t_do_shit_data *data, t_cmds **cmds_ptr)
{
	*cmds_ptr = malloc((data->exec->number_of_pipes + 1) * sizeof(t_cmds));
	if (*cmds_ptr == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
}

int	count_command_args2(char **args, int *i)
{
	int	j;

	j = 0;
	while (args[*i] && args[*i][0] != '|')
	{
		if (args[*i][0] == '<' || args[*i][0] == '>')
		{
			*i += 2;
			continue ;
		}
		(*i)++;
		j++;
	}
	return (j);
}

void	allocate_command_args(t_do_shit_data *data, t_cmds *cmds)
{
	int	i;
	int	k;
	int	j;

	i = -1;
	k = 0;
	while (data->args[++i])
	{
		j = count_command_args2(data->args, &i);
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
