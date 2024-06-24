    // unset.c
    #include "unset.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../minishell.h"
    #include "../env/new_env.h"

    // Function to implement the built-in 'unset' command
    int built_unset(Shell *shell, char **tokens, int tokens_len) {
        for (int i = 1; i < tokens_len; i++) {
            remove_env(&shell->env, tokens[i]);
        }
        return 0;
    }
