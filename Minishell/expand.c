/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 13:16:57 by mbankhar          #+#    #+#             */
/*   Updated: 2024/08/27 20:29:52 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_values(char **env, const char *var)
{
	size_t	var_len;
	int		i;
	char	*env_entry;
	char	*equals_sign;

	var_len = ft_strlen(var);
	i = -1;
	while (env[++i] != NULL)
	{
		env_entry = env[i];
		equals_sign = strchr(env_entry, '=');
		if (equals_sign != NULL && strncmp(env_entry, var, var_len) == 0
			&& env_entry[var_len] == '=')
		{
			return (env_entry + var_len + 1);
		}
	}
	return (NULL);
}

static void	extract_var_name(char *dollar_pos, char *var_name)
{
	int	j;

	j = 0;
	while (*dollar_pos != '\0' && *dollar_pos != ' ' && *dollar_pos != '$'
		&& *dollar_pos != '\"')
	{
		var_name[j++] = *dollar_pos++;
	}
	var_name[j] = '\0';
}

static void	replace_var_with_value(char *str, char *dollar_pos, char *env_value,
	char *new_str)
{
	int		k;
	char	*p;

	k = 0;
	p = str;
	while (*p != '\0' && p != dollar_pos - strlen(env_value) - 1)
	{
		new_str[k++] = *p++;
	}
	strcpy(new_str + k, env_value);
	k += strlen(env_value);
	while (*dollar_pos != '\0')
	{
		new_str[k++] = *dollar_pos++;
	}
	new_str[k] = '\0';
}

void	expand_in_2darray(char ***cmd, char **env)
{
	t_expand_vars	vars;
	int				i;

	i = 0;
	while ((*cmd)[i] != NULL)
	{
		vars.str = (*cmd)[i];
		vars.dollar_pos = strchr(vars.str, '$');
		if (vars.dollar_pos != NULL)
		{
			vars.dollar_pos++;
			extract_var_name(vars.dollar_pos, vars.var_name);
			vars.env_value = get_env_values(env, vars.var_name);
			if (vars.env_value != NULL)
			{
				replace_var_with_value(vars.str, vars.dollar_pos,
					vars.env_value, vars.new_str);
				free((*cmd)[i]);
				(*cmd)[i] = strdup(vars.new_str);
			}
		}
		i++;
	}
}
