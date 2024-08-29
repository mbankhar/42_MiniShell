/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:00:00 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 20:47:30 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_plus_equal(t_var_params *params, char *plus_equal_sign)
{
	size_t	var_len;

	var_len = plus_equal_sign - params->arg;
	strncpy(params->var, params->arg, var_len);
	params->var[var_len] = '\0';
	*(params->value) = plus_equal_sign + 2;
	*(params->append) = 1;
}

void	handle_equal(t_var_params *params, char *equal_sign)
{
	size_t	var_len;

	var_len = equal_sign - params->arg;
	strncpy(params->var, params->arg, var_len);
	params->var[var_len] = '\0';
	*(params->value) = equal_sign + 1;
	*(params->append) = 0;
}

void	extract_var_value(char *arg, char *var, char **value, int *append)
{
	t_var_params	params;
	char			*plus_equal_sign;
	char			*equal_sign;

	params.arg = arg;
	params.var = var;
	params.value = value;
	params.append = append;
	plus_equal_sign = strstr(arg, "+=");
	equal_sign = strchr(arg, '=');
	if (plus_equal_sign != NULL && (equal_sign == NULL
			|| plus_equal_sign < equal_sign))
	{
		handle_plus_equal(&params, plus_equal_sign);
	}
	else if (equal_sign != NULL)
	{
		handle_equal(&params, equal_sign);
	}
	else
	{
		strcpy(var, arg);
		*value = NULL;
		*append = 0;
	}
}

void	update_existing_var(t_update_var_params *params)
{
	char	*new_value;
	char	*existing_value;

	existing_value = strchr((*params->env)[params->idx], '=');
	if (existing_value != NULL)
	{
		existing_value++;
	}
	else
	{
		existing_value = "";
	}
	new_value = allocate_new_value4(existing_value, params->value,
			params->append);
	if (new_value == NULL)
	{
		perror("malloc");
		return ;
	}
	update_env_var(params->env, params->idx, params->var, new_value);
	free(new_value);
}

void	add_new_var(char ***env, const char *var, const char *value)
{
	int	env_size;

	env_size = count_env_size(*env);
	*env = expand_env(*env, env_size + 1);
	if (value == NULL)
	{
		(*env)[env_size] = malloc(strlen(var) + 1);
		if ((*env)[env_size] != NULL)
		{
			strcpy((*env)[env_size], var);
		}
	}
	else
	{
		(*env)[env_size] = malloc(strlen(var) + strlen(value) + 2);
		if ((*env)[env_size] != NULL)
		{
			construct_env_var((*env)[env_size], var, value);
		}
	}
	(*env)[env_size + 1] = NULL;
}
