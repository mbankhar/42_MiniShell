/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_directory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 12:52:22 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/27 18:15:37 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

static int	handle_special_paths(char **path, char ***env, t_shell *shell)
{
	if (*path == NULL || (*path)[0] == '\0')
	{
		*path = get_home_dir_helper(*env);
		if (*path == NULL)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			shell->exit_status = 1;
			return (1);
		}
	}
	else if ((*path)[0] == '-' && (*path)[1] == '\0')
	{
		*path = find_oldpwd_helper(env);
		if (*path == NULL)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
			shell->exit_status = 1;
			return (1);
		}
		printf("%s\n", *path);
	}
	return (0);
}

static int	update_env_vars(char ***env, char *buffer, t_shell *shell)
{
	update_env_var_helper(env, "OLDPWD", buffer);
	if (getcwd(buffer, PATH_MAX) == NULL)
	{
		perror("getcwd");
		shell->exit_status = 1;
		return (1);
	}
	update_env_var_helper(env, "PWD", buffer);
	return (0);
}

static int	perform_chdir(char *path, char *expanded_path, t_shell *shell)
{
	if (expanded_path != NULL)
	{
		if (chdir(expanded_path) == -1)
		{
			perror("chdir");
			shell->exit_status = 1;
			free(expanded_path);
			return (1);
		}
	}
	else
	{
		if (chdir(path) == -1)
		{
			perror("chdir");
			shell->exit_status = 1;
			return (1);
		}
	}
	return (0);
}

int	change_directory(char *path, char ***env, t_shell *shell)
{
	char			buffer[PATH_MAX];
	char			*expanded_path;

	expanded_path = NULL;
	if (handle_special_paths(&path, env, shell))
		return (1);
	if (path[0] != '\0')
		expanded_path = expand_env_variables(path, *env);
	if (getcwd(buffer, sizeof(buffer)) == NULL)
	{
		perror("getcwd");
		shell->exit_status = 1;
		free(expanded_path);
		return (1);
	}
	if (perform_chdir(path, expanded_path, shell))
		return (1);
	if (update_env_vars(env, buffer, shell))
	{
		free(expanded_path);
		return (1);
	}
	shell->exit_status = 0;
	free(expanded_path);
	return (0);
}
