#include "minishell.h"

int main(void) {
    char *line;
    Command *cmd;

    while (1) {
        line = readline("minishell> ");
        if (!line) {
            break; // EOF
        }
        if (strlen(line) > 0) {
            add_history(line);
            cmd = parse_input(line);
            if (cmd->num_commands > 0) {
                execute_command(cmd);
            }
            free_command(cmd);
        }
        free(line);
    }

    return 0;
}
