#include "minishell.h"

char *get_path(char **env, char *cmd) {
    int i;
    char *string;

    i = 0;
    if (ft_strchr(cmd, '/') != NULL)
        return cmd;
    while (env[i]) {
        if (ft_strnstr(env[i], "PATH=", 5) != NULL)
            break;
        i++;
    }
    if (env[i] == NULL) {
        char *default_paths[] = {"/usr/bin", "/bin", NULL};
        for (i = 0; default_paths[i] != NULL; i++) {
            string = ft_strjoinslesh(default_paths[i], cmd);
            if (access(string, X_OK) == 0)
                return string;
            free(string);
        }
        return NULL;
    }
    else
        return altpath(env, cmd, i);
}

char *altpath(char **env, char *cmd, int i) {
    char **paths;
    char *string;
    int j;

    paths = ft_split(env[i] + 5, ':');
    j = 0;
    while (paths[j]) {
        string = ft_strjoinslesh(paths[j], cmd);
        if (access(string, X_OK) == 0) {
            ft_free(paths);
            return string;
        }
        free(string);
        j++;
    }
    ft_free(paths);
    return NULL;
}
