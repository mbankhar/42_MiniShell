#include "minishell.h"

void parse_args(char **input, Token **tokens, bool is_command) {
    char *start = *input;
    while (**input && **input != ' ' && **input != '\t' && **input != '|' && **input != ';' && **input != '<' && **input != '>' && **input != '"') {
        (*input)++;
    }
    char *arg = strndup(start, *input - start);
    if (is_command) {
        add_token(tokens, TOKEN_COMMAND, arg);
    } else {
        add_token(tokens, TOKEN_LITERAL, arg);
    }
    free(arg);
}