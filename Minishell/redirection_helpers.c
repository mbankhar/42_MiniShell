/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 22:26:13 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 14:51:04 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_custom_env_value(const char *name, char **env)
{
	size_t	name_len;
	int		i;

	name_len = ft_strlen(name);
	i = -1;
	while (env[++i] != NULL)
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
		{
			return (env[i] + name_len + 1);
		}
	}
	return (NULL);
}

char	*extract_var_name(char *str, int *index)
{
	char	var_name[128];
	int		j;

	j = 0;
	while (str[*index] && (isalnum(str[*index]) || str[*index] == '_'))
		var_name[j++] = str[(*index)++];
	var_name[j] = '\0';
	return (strdup(var_name));
}

char	*expand_variable1(char *str, char **env)
{
	char	*var_name;
	char	*value;
	char	*result;
	int		i;

	i = 0;
	result = malloc(strlen(str) + 1);
	if (result == NULL)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			var_name = extract_var_name(str, &i);
			value = get_custom_env_value(var_name, env);
			free(var_name);
			if (value)
				strcat(result, value);
		}
		else
			strncat(result, &str[i++], 1);
	}
	return (result);
}

char	*expand_env_variables(const char *str, char **env)
{
	char	*result;
	char	*tmp;
	int		i;

	result = strdup(str);
	i = 0;
	while (result[i])
	{
		if (result[i] == '$')
		{
			tmp = expand_variable1(&result[i], env);
			result = ft_strjoin_free(result, tmp);
			free(tmp);
			i = 0;
		}
		else
			i++;
	}
	return (result);
}
