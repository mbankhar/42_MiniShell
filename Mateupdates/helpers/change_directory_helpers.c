/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_directory_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 12:52:22 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/22 17:37:34 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strcat_helper(char *dest, const char *src)
{
	char	*dest_start;

	dest_start = dest;
	while (*dest != '\0')
		dest++;
	while (*src != '\0')
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (dest_start);
}

int	find_env_index_helper(char **env, const char *var)
{
	int		i;
	size_t	var_len;

	i = 0;
	var_len = strlen(var);
	while (env[i] != NULL)
	{
		if (strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

void	update_env_var_helper(char ***env, char *var, char *value)
{
	int		var_index;
	char	*new_var;

	var_index = find_env_index_helper(*env, var);
	new_var = create_new_var1(var, value);
	if (new_var == NULL)
		return ;
	if (var_index != -1)
		update_existing_var1(env, var_index, new_var);
	else
		add_new_var1(env, new_var);
}

char	*find_oldpwd_helper(char ***env)
{
	int	oldpwd_index;

	oldpwd_index = find_env_index_helper(*env, "OLDPWD");
	if (oldpwd_index == -1)
	{
		fprintf(stderr, "OLDPWD not set\n");
		return (NULL);
	}
	return ((*env)[oldpwd_index] + 7);
}

char	*get_home_dir_helper(char **env)
{
	int	home_index;

	home_index = find_env_index_helper(env, "HOME");
	if (home_index == -1)
	{
		fprintf(stderr, "HOME not set\n");
		return (NULL);
	}
	return (env[home_index] + 5);
}
