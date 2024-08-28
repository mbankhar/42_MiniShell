/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 09:35:59 by mbankhar          #+#    #+#             */
/*   Updated: 2024/08/28 20:41:47 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_shit(t_do_shit_data *data)
{
	t_cmds	*cmds;

	initialize_commands(data, &cmds);
	allocate_command_args(data, cmds);
	process_arguments(data, cmds);
	*data->cmds_ptr = cmds;
}

char	*allocate_temp_string(int length)
{
	char	*temp;

	temp = malloc(sizeof(char) * (length + 1));
	if (temp == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (temp);
}

void	copy_without_quotes(char *dest, const char *src)
{
	int	j;
	int	k;
	int	length;

	j = 0;
	k = 0;
	length = strlen(src);
	while (j < length)
	{
		if (src[j] != '\'' && src[j] != '\"')
		{
			dest[k++] = src[j];
		}
		j++;
	}
	dest[k] = '\0';
}

void	remove_quotes(char **args)
{
	int		i;
	int		length;
	char	*temp;

	if (args == NULL)
		return ;
	i = -1;
	while (args[++i] != NULL)
	{
		length = strlen(args[i]);
		temp = allocate_temp_string(length);
		copy_without_quotes(temp, args[i]);
		free(args[i]);
		args[i] = temp;
	}
}
