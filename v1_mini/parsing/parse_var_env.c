#include "minishell.h"

// This function parses environment variables from the input and tokenizes them.
void parse_varenv(char **input, Token **tokens) {
    char *start = ++(*input); // Skip the '$' sign
    while (**input && (isalnum(**input) || **input == '_')) {
        (*input)++;
    }
    char *var_name = strndup(start, *input - start);
    add_token(tokens, TOKEN_VARIABLE, var_name);
    free(var_name);
}