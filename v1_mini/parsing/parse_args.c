#include "minishell.h"

void parse_args(char **input, Token **tokens) {
    char *start = *input;
    while (**input && **input != ' ' && **input != '\t' && **input != '|' && **input != ';' && **input != '<' && **input != '>' && **input != '"') {
        (*input)++;
    }
    char *arg = strndup(start, *input - start);
    add_token(tokens, TOKEN_LITERAL, arg);
    free(arg);
}