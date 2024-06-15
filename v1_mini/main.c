#include "parsing/minishell.h"

// Function to print tokens for testing
void print_tokens(Token *tokens) {
    while (tokens) {
        printf("Type: %d, Value: %s\n", tokens->type, tokens->value);
        tokens = tokens->next;
    }
}

int main(int argc, char **argv, char **envp) {
    (void)argc;
    (void)argv;
    (void)envp;

    char *line;
    Token *tokens;

    while (1) {
        line = readline("minishell> ");
        if (!line) {
            break; // EOF
        }
        if (strlen(line) > 0) {
            add_history(line);
            tokens = tokenize_input(line);
            print_tokens(tokens);
            free_tokens(tokens);
        }
        free(line);
    }

    return 0;
}
