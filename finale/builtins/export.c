// export.c
#include "export.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../minishell.h"
#include "../env/new_env.h"

// Function to implement the built-in 'export' command
int built_export(Shell *shell, char **tokens, int tokens_len) {
    for (int i = 1; i < tokens_len; i++) {
        char *token = tokens[i];
        char *equals_sign = strchr(token, '=');
        if (equals_sign != NULL) {
            *equals_sign = '\0'; // Split the string into key and value
            const char *key = token;
            const char *value = equals_sign + 1;
            set_env(&shell->env, key, value);
        } else {
            fprintf(stderr, "minishell: export: %s: not a valid identifier\n", token);
            return 1;
        }
    }
    return 0;
}
