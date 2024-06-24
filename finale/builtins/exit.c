// exit.c
#include "exit.h"
#include <stdio.h>
#include <stdlib.h>
#include "../minishell.h"

// Function to implement the built-in 'exit' command
int built_exit(Shell *shell, char **tokens, int tokens_len) {
    if (tokens_len > 2) {
        fprintf(stderr, "minishell: exit: too many arguments\n");
        return 1;
    }
    printf("exit\n");
    if (tokens_len == 2) {
        exit(atoi(tokens[1]));
    }
    exit(shell->error);
}
