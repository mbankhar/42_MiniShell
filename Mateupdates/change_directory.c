/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_directory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 10:32:43 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/24 21:45:47 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strcat(char *dest, const char *src)
{
	char *dest_start = dest;
	while (*dest != '\0')
		dest++;
	while (*src != '\0')
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return dest_start;
}

// Function to copy a string


// Function to find the index of an environment variable
int find_env_index(char **env, const char *var)
{
	int i = 0;
	size_t var_len = strlen(var);
	while (env[i] != NULL)
	{
		if (strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
		{
			return i;
		}
		i++;
	}
	return -1;
}

// Function to update an environment variable
void	update_env_var(char ***env, char *var, char *value)
{
	int var_index = find_env_index(*env, var);
	size_t var_len = strlen(var);
	size_t value_len = strlen(value);
	char *new_var = malloc(var_len + value_len + 2);
	if (new_var == NULL)
	{
		perror("malloc");
		return;
	}
	ft_strcpy(new_var, var);
	ft_strcat(new_var, "=");
	ft_strcat(new_var, value);
	if (var_index != -1)
		(*env)[var_index] = new_var;
	else
	{
		int env_len = -1;
		while ((*env)[++env_len] != NULL)
			;
		char **new_env = realloc(*env, (env_len + 2) * sizeof(char *));
		if (new_env == NULL)
		{
			perror("realloc");
			free(new_var);
			return;
		}
		*env = new_env;
		(*env)[env_len] = new_var;
		(*env)[env_len + 1] = NULL;
	}
}

// Helper function to get the value of OLDPWD
char *find_oldpwd(char ***env)
{
	int oldpwd_index = find_env_index(*env, "OLDPWD");
	if (oldpwd_index == -1)
	{
		fprintf(stderr, "OLDPWD not set\n");
		return NULL;
	}
	return ((*env)[oldpwd_index] + 7);
}

// Helper function to get the value of HOME
char *get_home_dir(char **env)
{
	int home_index = find_env_index(env, "HOME");
	if (home_index == -1)
	{
		fprintf(stderr, "HOME not set\n");
		return NULL;
	}
	return (env[home_index] + 5);
}

// void change_directory(char *path, char ***env)
// {
// 	char	buffer[PATH_MAX];

// 	if (path == NULL || (path[0] == '\0'))
// 	{
// 		path = get_home_dir(*env);
// 		if (path == NULL)
// 			return ;
// 	}
// 	else if (path[0] == '-' && path[1] == '\0')
// 	{
// 		path = find_oldpwd(env);
// 		if (path == NULL)
// 			return ;
// 		printf("%s\n", path);
// 	}
// 	if (getcwd(buffer, sizeof(buffer)) == NULL)
// 	{
// 		perror("getcwd");
// 		return ;
// 	}
// 	update_env_var(env, "OLDPWD", buffer);
// 	if (chdir(path) == -1)
// 	{
// 		perror("chdir");
// 		return ;
// 	}
// 	if (getcwd(buffer, sizeof(buffer)) == NULL)
// 	{
// 		perror("getcwd");
// 		return ;
// 	}
// 	update_env_var(env, "PWD", buffer);
// }


// int change_directory(char *path, char ***env, t_shell *shell)
// {
//     char buffer[PATH_MAX];

//     if (path == NULL || (path[0] == '\0'))
//     {
//         path = get_home_dir(*env);
//         if (path == NULL)
//         {
//             ft_putendl_fd("minishell: cd: HOME not set", 2);
//             shell->exit_status = 1;
//             return 1;
//         }
//     }
//     else if (path[0] == '-' && path[1] == '\0')
//     {
//         path = find_oldpwd(env);
//         if (path == NULL)
//         {
//             ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
//             shell->exit_status = 1;
//             return 1;
//         }
//         printf("%s\n", path);
//     }
//     if (getcwd(buffer, sizeof(buffer)) == NULL)
//     {
//         perror("getcwd");
//         shell->exit_status = 1;
//         return 1;
//     }
//     if (chdir(path) == -1)
//     {
//         perror("chdir");
//         shell->exit_status = 1;
//         return 1;
//     }
//     update_env_var(env, "OLDPWD", buffer);
//     if (getcwd(buffer, sizeof(buffer)) == NULL)
//     {
//         perror("getcwd");
//         shell->exit_status = 1;
//         return 1;
//     }
//     update_env_var(env, "PWD", buffer);
//     shell->exit_status = 0;
//     return 0;
// }



int change_directory(char *path, char ***env, t_shell *shell)
{
    char buffer[PATH_MAX];
    char *expanded_path = NULL;

    if (path == NULL || (path[0] == '\0'))
    {
        path = get_home_dir(*env);
        if (path == NULL)
        {
            ft_putendl_fd("minishell: cd: HOME not set", 2);
            shell->exit_status = 1;
            return 1;
        }
    }
    else if (path[0] == '-' && path[1] == '\0')
    {
        path = find_oldpwd(env);
        if (path == NULL)
        {
            ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
            shell->exit_status = 1;
            return 1;
        }
        printf("%s\n", path);
    }
    else
    {
        expanded_path = expand_env_variables(path, *env);
        path = expanded_path;
    }

    if (getcwd(buffer, sizeof(buffer)) == NULL)
    {
        perror("getcwd");
        shell->exit_status = 1;
        if (expanded_path)
            free(expanded_path);
        return 1;
    }
    if (chdir(path) == -1)
    {
        perror("chdir");
        shell->exit_status = 1;
        if (expanded_path)
            free(expanded_path);
        return 1;
    }
    update_env_var(env, "OLDPWD", buffer);
    if (getcwd(buffer, sizeof(buffer)) == NULL)
    {
        perror("getcwd");
        shell->exit_status = 1;
        if (expanded_path)
            free(expanded_path);
        return 1;
    }
    update_env_var(env, "PWD", buffer);
    shell->exit_status = 0;
    if (expanded_path)
        free(expanded_path);
    return 0;
}