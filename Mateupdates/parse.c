/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 09:35:59 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/10 11:38:47 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_shit(char **args, t_exec *exec, t_cmds **cmds_ptr, char **env)
{
	int		i;
	int		j;
	int		k;
	t_cmds	*cmds;

	i = -1;
	*cmds_ptr = malloc((exec->number_of_pipes + 1) * sizeof(t_cmds));
	cmds = *cmds_ptr;
	i = -1;
	k = 0;
	while (args[++i])
	{
		j = 0;
		while (args[i] && args[i][0] != '|')
		{
			if (args[i][0] == '<' || args[i][0] == '>')
			{
				i = i + 2;
				continue ;
			}
			else
			{
				i++;
				j++;
			}
		}
		cmds[k].cmd_args = malloc(sizeof(char *) * (j + 1));
		k++;
		if (!args[i])
			break ;
	}
	i = -1;
	k = 0;
	while (args[++i])
	{
		j = 0;
		cmds[k].fd_in = -2;
		cmds[k].fd_out = -2;
		while (args[i] && args[i][0] != '|')
		{
			if (args[i][0] == '<' || args[i][0] == '>')
			{
				look_for_redirect(args, i, &cmds[k], env);
				i = i + 2;
				continue ;
			}
			else
				cmds[k].cmd_args[j++] = args[i++];
		}
		cmds[k].cmd_args[j++] = NULL;
		k++;
		if (!args[i])
		{
			break ;
		}
	}
	cmds->size = k;
}


// Function to remove quotes from a string
void	remove_quotes(char **args)
{
	int		i;
	int		j;
	int		k;
	int		length;
	char	*temp;

	if (args == NULL)
		return ;
	i = -1;
	while (args[++i] != NULL)
	{
		length = strlen(args[i]);
		temp = malloc(sizeof(char) * (length + 1));
		if (temp == NULL)
			exit(EXIT_FAILURE);
		k = 0;
		j = -1;
		while (++j < length)
		{
			if (args[i][j] != '\'' && args[i][j] != '\"')
				temp[k++] = args[i][j];
		}
		temp[k] = '\0';
		free(args[i]);
		args[i] = temp;
	}
}
