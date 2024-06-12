// main.c
#include "minishell.h"

int main(void) {
    char *line;

    while (1) {
        line = readline("minishell> ");
        if (!line) {
            break; // Handle Ctrl+D (EOF)
        }
        add_history(line);

        Command *cmd = parse_input(line);
        execute_command(cmd); // Call to execution function

        // Free memory
        free_command(cmd);
        free(line);
    }

    return 0;
}
