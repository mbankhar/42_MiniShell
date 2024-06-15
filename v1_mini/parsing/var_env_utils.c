#include "minishell.h"

// This function retrieves the value of an environment variable.
char *get_env_value(char *name, char **envp) {
    while (*envp) {
        char *env_var = *envp;
        char *eq_pos = strchr(env_var, '=');
        if (eq_pos) {
            int name_len = eq_pos - env_var;
            if (strncmp(env_var, name, name_len) == 0 && name[name_len] == '\0') {
                return eq_pos + 1;
            }
        }
        envp++;
    }
    return NULL;
}
