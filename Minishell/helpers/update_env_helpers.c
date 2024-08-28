/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:31:58 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/22 17:34:15 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*create_new_var1(char *var, char *value)
{
	size_t	var_len;
	size_t	value_len;
	char	*new_var;

	var_len = strlen(var);
	value_len = strlen(value);
	new_var = malloc(var_len + value_len + 2);
	if (new_var == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	ft_strcpy(new_var, var);
	ft_strcat_helper(new_var, "=");
	ft_strcat_helper(new_var, value);
	return (new_var);
}

void	update_existing_var1(char ***env, int var_index, char *new_var)
{
	(*env)[var_index] = new_var;
}

void	add_new_var1(char ***env, char *new_var)
{
	int		env_len;
	char	**new_env;

	env_len = -1;
	while ((*env)[++env_len] != NULL)
		;
	new_env = realloc(*env, (env_len + 2) * sizeof(char *));
	if (new_env == NULL)
	{
		perror("realloc");
		free(new_var);
		return ;
	}
	*env = new_env;
	(*env)[env_len] = new_var;
	(*env)[env_len + 1] = NULL;
}
