/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 16:53:58 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 18:15:03 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_cmd_args(char **args, int *i)
{
	int	j;

	j = 0;
	while (args[*i] && args[*i][0] != '|')
	{
		if ((args[*i][0] == '<' && args[*i][1] == '<') || args[*i][0] == '<'
			|| args[*i][0] == '>')
		{
			*i += 2;
			continue ;
		}
		(*i)++;
		j++;
	}
	return (j);
}

void	process_args(t_do_shit_data *data, t_cmds *cmds)
{
	int	i;
	int	k;

	i = -1;
	k = 0;
	while (data->args[++i])
	{
		process_single_cmd(data, &cmds[k], &i);
		k++;
		if (!data->args[i])
			break ;
	}
	cmds->size = k;
}
