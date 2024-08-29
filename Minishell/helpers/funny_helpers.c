/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funny_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:51:17 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 18:16:15 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*allocate_new_value4(const char *existing_value, const char *new_value,
		int append)
{
	char	*result;

	if (append)
	{
		result = malloc(strlen(existing_value) + strlen(new_value) + 1);
		if (result != NULL)
		{
			strcpy(result, existing_value);
			strcat(result, new_value);
		}
	}
	else
	{
		result = malloc(strlen(new_value) + 1);
		if (result != NULL)
		{
			strcpy(result, new_value);
		}
	}
	return (result);
}

void	update_env_var(char ***env, int idx, const char *var,
		const char *new_value)
{
	free((*env)[idx]);
	(*env)[idx] = malloc(strlen(var) + strlen(new_value) + 2);
	if ((*env)[idx] != NULL)
	{
		construct_env_var((*env)[idx], var, new_value);
	}
}
