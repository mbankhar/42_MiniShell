/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 18:23:20 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 18:23:25 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_env_size(char **env)
{
	int	count;

	count = 0;
	while (env[count] != NULL)
	{
		count++;
	}
	return (count);
}

int	find_env_var(char **env, const char *var)
{
	size_t	var_len;
	int		i;

	var_len = strlen(var);
	i = 0;
	while (env[i] != NULL)
	{
		if (strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

char	**allocate_new_env(int new_size)
{
	char	**new_env;

	new_env = (char **)malloc((new_size + 1) * sizeof(char *));
	if (new_env == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (new_env);
}

void	copy_env(char **new_env, char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[i] = NULL;
}
