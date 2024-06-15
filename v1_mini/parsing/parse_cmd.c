#include "minishell.h"

void parse_cmd(char **input, Token **tokens) {
    if (**input == '|') {
        add_token(tokens, TOKEN_PIPELINE, "|");
        (*input)++;
    } else if (**input == ';') {
        add_token(tokens, TOKEN_SEMICOLON, ";");
        (*input)++;
    } else {
        parse_args(input, tokens, true); // First token is the command
    }
}