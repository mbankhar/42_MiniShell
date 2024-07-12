/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 11:11:08 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/11 11:16:34 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Function prototypes for shared helper functions
int count_env_size(char **env);
int find_env_var(char **env, const char *var);
char **allocate_new_env(int new_size);
void copy_env(char **new_env, char **env);
char **expand_env(char **env, int new_size);
void construct_env_var(char *dest, const char *var, const char *value);
int is_alphanumeric(const char *str);
void print_envs(char **env);
void extract_var_value(char *arg, char *var, char **value, int *append);
void update_existing_var(char ***env, int idx, const char *var, const char *value, int append);
void add_new_var(char ***env, const char *var, const char *value);
void execute_export(t_cmds *cmds, char ***env);

// Count the size of the environment array
int count_env_size(char **env)
{
    int count = 0;
    while (env[count] != NULL)
    {
        count++;
    }
    return count;
}

// Find an environment variable by name
int find_env_var(char **env, const char *var)
{
    size_t var_len = strlen(var);
    int i = 0;
    while (env[i] != NULL) {
        if (strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=') {
            return i;
        }
        i++;
    }
    return -1;
}

// Allocate new environment array
char **allocate_new_env(int new_size)
{
    char **new_env = (char **)malloc((new_size + 1) * sizeof(char *));
    if (new_env == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return new_env;
}

// Copy environment array
void copy_env(char **new_env, char **env)
{
    int i = 0;
    while (env[i] != NULL) {
        new_env[i] = env[i];
        i++;
    }
    new_env[i] = NULL;
}

// Expand environment array
char **expand_env(char **env, int new_size)
{
    char **new_env = allocate_new_env(new_size);
    copy_env(new_env, env);
    return new_env;
}

// Construct environment variable string
void construct_env_var(char *dest, const char *var, const char *value)
{
    strcpy(dest, var);
    strcat(dest, "=");
    strcat(dest, value);
}

// Check if the string is alphanumeric
int is_alphanumeric(const char *str)
{
    while (*str) {
        if (!isalnum(*str) && *str != '_') {
            return 0;
        }
        str++;
    }
    return 1;
}

// Print the environment with values in double quotes and declare -x prefix
void print_envs(char **env)
{
    int i = 0;
    while (env[i] != NULL) {
        char *equal_sign = strchr(env[i], '=');
        if (equal_sign != NULL) {
            *equal_sign = '\0';
            printf("declare -x %s=\"%s\"\n", env[i], equal_sign + 1);
            *equal_sign = '=';
        } else {
            printf("declare -x %s\n", env[i]);
        }
        i++;
    }
}

void extract_var_value(char *arg, char *var, char **value, int *append)
{
    char *plus_equal_sign = strstr(arg, "+=");
    char *equal_sign = strchr(arg, '=');

    if (plus_equal_sign != NULL && (equal_sign == NULL || plus_equal_sign < equal_sign))
    {
        size_t var_len = plus_equal_sign - arg;
        strncpy(var, arg, var_len);
        var[var_len] = '\0';
        *value = plus_equal_sign + 2;
        *append = 1;
    }
    else if (equal_sign != NULL)
    {
        size_t var_len = equal_sign - arg;
        strncpy(var, arg, var_len);
        var[var_len] = '\0';
        *value = equal_sign + 1;
        *append = 0;
    }
    else
    {
        strcpy(var, arg);
        *value = NULL;
        *append = 0;
    }
}


void update_existing_var(char ***env, int idx, const char *var, const char *value, int append)
{
    char *new_value;
    char *existing_value = strchr((*env)[idx], '=');

    if (append)
    {
        new_value = malloc(strlen(existing_value + 1) + strlen(value) + 1);
        if (new_value != NULL)
        {
            strcpy(new_value, existing_value + 1);
            strcat(new_value, value);
        }
    }
    else
    {
        new_value = malloc(strlen(var) + strlen(value) + 2);
        if (new_value != NULL)
        {
            construct_env_var(new_value, var, value);
        }
    }

    free((*env)[idx]);
    (*env)[idx] = malloc(strlen(var) + strlen(new_value) + 2);
    if ((*env)[idx] != NULL)
    {
        construct_env_var((*env)[idx], var, new_value);
    }

    free(new_value);
}


void add_new_var(char ***env, const char *var, const char *value)
{
    int env_size = count_env_size(*env);
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


void execute_export(t_cmds *cmds, char ***env)
{
    int i;
    int idx;
    char var[256];
    char *value;
    int append;

    i = 1;
    if (cmds->cmd_args[1] == NULL)
    {
        print_envs(*env);
        return;
    }
    while (cmds->cmd_args[i] != NULL)
    {
        extract_var_value(cmds->cmd_args[i], var, &value, &append);
        if (is_alphanumeric(var))
        {
            if (value != NULL || append)
            {
                idx = find_env_var(*env, var);
                if (idx != -1)
                {
                    update_existing_var(env, idx, var, value, append);
                }
                else
                {
                    add_new_var(env, var, value);
                }
            }
            else
            {
                idx = find_env_var(*env, var);
                if (idx == -1)
                {
                    add_new_var(env, var, NULL);
                }
            }
        }
        else
        {
            fprintf(stderr, "export: invalid variable name: %s\n", var);
        }
        i++;
    }
}



// Execute the unset command
void execute_unset(t_cmds *cmds, char ***env)
{
	int		i;
	char	*var;
	int		idx;

	i = 1;
	while (cmds->cmd_args[i] != NULL)
	{
		var = cmds->cmd_args[i];
		if (is_alphanumeric(var))
		{
			idx = find_env_var(*env, var);
			if (idx != -1)
			{
				while ((*env)[idx + 1] != NULL)
				{
					(*env)[idx] = (*env)[idx + 1];
					idx++;
				}
				(*env)[idx] = NULL;
			}
			else
				printf("unset: variable not found: %s\n", var);
		}
		else
			printf("unset: invalid variable name: %s\n", var);
		i++;
	}
}
