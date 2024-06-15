#include "minishell.h"

void parse_sep(char **input, Token **tokens) {
    if (**input == ';') {
        add_token(tokens, TOKEN_SEMICOLON, ";");
        (*input)++;
    }
}
