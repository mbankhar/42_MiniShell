/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:00:00 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 17:25:42 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**expand_env(char **env, int new_size)
{
	char	**new_env;

	new_env = allocate_new_env(new_size);
	copy_env(new_env, env);
	return (new_env);
}

void	construct_env_var(char *dest, const char *var, const char *value)
{
	strcpy(dest, var);
	strcat(dest, "=");
	strcat(dest, value);
}

int	is_alphanumeric(const char *str)
{
	while (*str)
	{
		if (!isalnum(*str) && *str != '_')
		{
			return (0);
		}
		str++;
	}
	return (1);
}

void	print_envs(char **env)
{
	int		i;
	char	*equal_sign;

	i = 0;
	while (env[i] != NULL)
	{
		equal_sign = strchr(env[i], '=');
		if (equal_sign != NULL)
		{
			*equal_sign = '\0';
			printf("declare -x %s=\"%s\"\n", env[i], equal_sign + 1);
			*equal_sign = '=';
		}
		else
		{
			printf("declare -x %s\n", env[i]);
		}
		i++;
	}
}
