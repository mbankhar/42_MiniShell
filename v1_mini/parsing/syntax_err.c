#include "minishell.h"

// This function handles syntax errors by tokenizing an error message.
void handle_syntax_error(char *input, Token **tokens) {
    char *error_msg = malloc(strlen(input) + 50);
    sprintf(error_msg, "Syntax error near unexpected token '%s'", input);
    add_token(tokens, TOKEN_ERROR, error_msg);
    free(error_msg);
}
