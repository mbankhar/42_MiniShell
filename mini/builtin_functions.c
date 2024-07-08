#include "minishell.h"

void ft_free(char **array)
{
    int i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

void change_directory(const char *path)
{
    if (chdir(path) == -1)
    {
        perror("minishell: cd");
    }
}

void execute_echo(char *args[])
{
    int i = 0;
    while (args[++i] != NULL)
    {
        printf("%s", args[i]);
        if (args[i + 1] != NULL)
        {
            printf(" ");
        }
    }
    printf("\n");
}

void execute_export(const char *var, const char *value)
{
    extern char **environ;
    set_env_var(var, value, environ);
}

void execute_unset(char *var)
{
    extern char **environ;
    int i = 0, j = 0;
    while (environ[i])
    {
        if (strncmp(environ[i], var, strlen(var)) != 0 || environ[i][strlen(var)] != '=')
        {
            environ[j++] = environ[i];
        }
        else
        {
            free(environ[i]);
        }
        i++;
    }
    environ[j] = NULL;
}

void print_env()
{
    extern char **environ;
    char **env = environ;
    while (*env != 0)
    {
        printf("%s\n", *env);
        env++;
    }
}

void set_env_var(const char *name, const char *value, char **environ)
{
    int i = 0;
    size_t name_len = strlen(name);

    while (environ[i])
    {
        if (strncmp(environ[i], name, name_len) == 0 && environ[i][name_len] == '=')
        {
            free(environ[i]);
            environ[i] = malloc(name_len + strlen(value) + 2);
            if (environ[i] == NULL)
            {
                perror("minishell: malloc error");
                exit(EXIT_FAILURE);
            }
            sprintf(environ[i], "%s=%s", name, value);
            return;
        }
        i++;
    }

    environ = realloc(environ, (i + 2) * sizeof(char *));
    if (environ == NULL)
    {
        perror("minishell: realloc error");
        exit(EXIT_FAILURE);
    }
    environ[i] = malloc(name_len + strlen(value) + 2);
    if (environ[i] == NULL)
    {
        perror("minishell: malloc error");
        exit(EXIT_FAILURE);
    }
    sprintf(environ[i], "%s=%s", name, value);
    environ[i + 1] = NULL;
}

char *create_env_entry(const char *name, const char *value)
{
    char *entry = malloc(strlen(name) + strlen(value) + 2);
    if (!entry)
    {
        perror("minishell: malloc");
        exit(1);
    }
    sprintf(entry, "%s=%s", name, value);
    return entry;
}

void update_oldpwd(char **environ)
{
    char oldpwd[PATH_MAX];
    if (getcwd(oldpwd, sizeof(oldpwd)))
    {   
        set_env_var("OLDPWD", oldpwd, environ);
    }
}

void execute_builtin(t_cmds *cmds, char **environ)

{
    if (!cmds || !cmds->cmd_args || !cmds->cmd_args[0])
        return;

   if (strcmp(cmds->cmd_args[0], "cd") == 0)
    {
        static char prev_dir[PATH_MAX] = ""; // Store previous directory
        char current_dir[PATH_MAX];

        if (getcwd(current_dir, sizeof(current_dir)) == NULL)
        {
            perror("minishell: getcwd");
            return;
        }

        if (cmds->cmd_args[1] == NULL || strcmp(cmds->cmd_args[1], "~") == 0)
        {
            // Change to home directory
            char *home_dir = getenv("HOME");
            if (home_dir)
            {
                if (chdir(home_dir) == 0)
                {
                    set_env_var("OLDPWD", current_dir, environ);
                }
                else
                {
                    perror("minishell: cd");
                }
            }
            else
            {
                fprintf(stderr, "minishell: cd: HOME not set\n");
            }
        }
        else if (strcmp(cmds->cmd_args[1], "-") == 0)
        {
            // Change to previous directory
            if (*prev_dir)
            {
                if (chdir(prev_dir) == 0)
                {
                    printf("%s\n", prev_dir);
                    strcpy(prev_dir, current_dir);
                }
                else
                {
                    perror("minishell: cd");
                }
            }
            else
            {
                fprintf(stderr, "minishell: cd: OLDPWD not set\n");
            }
        }
        else if (strcmp(cmds->cmd_args[1], ";") == 0)
        {
            // Handle invalid argument ';'
            fprintf(stderr, "minishell: cd: ';': invalid argument\n");
        }
        else
        {
            // Change to the specified directory
            if (chdir(cmds->cmd_args[1]) == 0)
            {
                strcpy(prev_dir, current_dir);
                set_env_var("OLDPWD", current_dir, environ);
            }
            else
            {
                perror("minishell: cd");
            }
        }
    }
    else if (strcmp(cmds->cmd_args[0], "echo") == 0)
    {
        execute_echo(cmds->cmd_args);
    }
    else if (strcmp(cmds->cmd_args[0], "export") == 0)
    {
        if (cmds->cmd_args[1] == NULL)
        {
            fprintf(stderr, "export: missing argument\n");
        }
        else
        {
            char *var = strtok(cmds->cmd_args[1], "=");
            char *value = strtok(NULL, "=");
            if (value != NULL)
            {
                execute_export(var, value);
            }
            else if (value == NULL && strchr(cmds->cmd_args[1], '='))
            {
                execute_export(var, "");
            }
        }
    }
    else if (strcmp(cmds->cmd_args[0], "unset") == 0)
    {
        if (cmds->cmd_args[1] == NULL)
        {
            fprintf(stderr, "unset: missing argument\n");
        }
        else
        {
            execute_unset(cmds->cmd_args[1]);
        }
    }
    else if (strcmp(cmds->cmd_args[0], "env") == 0)
    {
        print_env();
    }
}
