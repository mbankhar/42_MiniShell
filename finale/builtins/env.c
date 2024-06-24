// env.c
#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../minishell.h"
#include "../env/new_env.h"

// Function to implement the built-in 'env' command
int built_env(Shell *shell, char **tokens, int tokens_len) {
    if (tokens_len > 1) {
        fprintf(stderr, "minishell: env: too many arguments\n");
        return 1;
    }

    char *env = NULL;
    size_t env_len = 0;
    for (size_t i = 0; i < shell->env.size; ++i) {
        EnvVar *var = &shell->env.vars[i];
        size_t line_len = strlen(var->key) + strlen(var->value) + 2; // key=value\n
        env = realloc(env, env_len + line_len);
        sprintf(env + env_len, "%s=%s\n", var->key, var->value);
        env_len += line_len;
    }

    tokens = realloc(tokens, 3 * sizeof(char *));
    tokens[0] = strdup("echo");
    tokens[1] = env;
    tokens[2] = NULL;
    return 0;
}
