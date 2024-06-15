#include "minishell.h"

void parse_pipe(char **input, Token **tokens) {
    add_token(tokens, TOKEN_PIPELINE, "|");
    (*input)++;
}