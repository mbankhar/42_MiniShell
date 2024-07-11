/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 13:16:57 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/11 10:08:08 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_env_values(char **env, const char *var)
{
	size_t var_len;
	int i;
	char *env_entry;
	char *equals_sign;

	var_len = ft_strlen(var);
	i = -1;
	while (env[++i] != NULL)
	{
		env_entry = env[i];
		equals_sign = strchr(env_entry, '=');
		if (equals_sign != NULL && strncmp(env_entry, var, var_len) == 0 && env_entry[var_len] == '=')
		{
			return env_entry + var_len + 1;
		}
	}
	return NULL;
}

void expand_in_2darray(char ***cmd, char **env)
{
	int i, j, k;
	char *str;
	char *dollar_pos;
	char var_name[256];
	char new_str[1024];
	char *env_value;
	char *p;

	for (i = 0; (*cmd)[i] != NULL; i++) {
		str = (*cmd)[i];
		dollar_pos = strchr(str, '$');
		if (dollar_pos != NULL) {
			dollar_pos++;
			j = 0;
			while (*dollar_pos != '\0' && *dollar_pos != ' ' && *dollar_pos != '$' && *dollar_pos != '\"')
			{
				var_name[j++] = *dollar_pos++;
			}
			var_name[j] = '\0';
			env_value = get_env_values(env, var_name);
			if (env_value != NULL)
			{
				k = 0;
				p = str;
				while (*p != '\0' && p != dollar_pos - j - 1)
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
				free((*cmd)[i]);
				(*cmd)[i] = strdup(new_str);
			}
		}
	}
}
