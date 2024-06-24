// pwd.c
#include "pwd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../minishell.h"
#include "../env/new_env.h"

// Function to implement the built-in 'pwd' command
int built_pwd(Shell *shell, char **tokens, int tokens_len) {
    (void)tokens_len; // Suppress unused parameter warning
    (void)tokens;     // Suppress unused parameter warning
    const char *pwd = get_env(&shell->env, "PWD");
    if (pwd) {
        printf("%s\n", pwd);
        return 0;
    } else {
        fprintf(stderr, "minishell: pwd: PWD not set\n");
        return 1;
    }
}
