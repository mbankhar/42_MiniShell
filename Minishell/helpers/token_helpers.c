/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:23:06 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 13:39:35 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_commands1(char **commands)
{
	int	i;
	int	cmd_count;
	int	skip_next;

	i = 0;
	cmd_count = 0;
	skip_next = 0;
	while (commands[i])
	{
		if (skip_next)
			skip_next = 0;
		else if (ft_strcmp(commands[i], "<") == 0 || ft_strcmp(commands[i],
				">") == 0)
			skip_next = 1;
		else if (ft_strcmp(commands[i], "|") != 0)
			cmd_count++;
		i++;
	}
	return (cmd_count);
}

void	free_filtered_commands(char **filtered_commands, int j)
{
	int	k;

	k = -1;
	while (++k < j)
		free(filtered_commands[k]);
	free(filtered_commands);
}

static int	should_skip_command(char *command, int *skip_next)
{
	if (*skip_next)
	{
		*skip_next = 0;
		return (1);
	}
	else if (ft_strcmp(command, "<") == 0 || ft_strcmp(command, ">") == 0)
	{
		*skip_next = 1;
		return (1);
	}
	else if (ft_strcmp(command, "|") == 0)
		return (1);
	return (0);
}

static int	add_filtered_command(char **filtered_commands, char *command, int j)
{
	filtered_commands[j] = ft_strdup(command);
	if (!filtered_commands[j])
	{
		free_filtered_commands(filtered_commands, j);
		return (0);
	}
	return (1);
}

char	**filter_commands(char **commands, int cmd_count)
{
	char	**filtered_commands;
	int		i;
	int		j;
	int		skip_next;

	filtered_commands = (char **)malloc((cmd_count + 1) * sizeof(char *));
	if (!filtered_commands)
		return (NULL);
	i = 0;
	j = 0;
	skip_next = 0;
	while (commands[i])
	{
		if (should_skip_command(commands[i], &skip_next))
			;
		else if (!add_filtered_command(filtered_commands, commands[i], j))
			return (NULL);
		else
			j++;
		i++;
	}
	filtered_commands[j] = NULL;
	return (filtered_commands);
}
