/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_number.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 10:51:09 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/28 13:05:14 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Function to count the number of valid commands in the tokens array
int	get_token_number(char **tokens, char **env)
{
	int		count;
	char	*path;
	int		i;

	i = -1;
	count = 0;
	while (tokens[++i] != NULL)
	{
		path = get_path(env, tokens[i]);
		if (path != NULL)
		{
			count++;
			free(path);
		}
	}
	return (count);
}
