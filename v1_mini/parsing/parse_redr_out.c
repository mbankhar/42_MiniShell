#include "minishell.h"

void parse_redir_out(char **input, Token **tokens) {
    if (**input == '>') {
        if (*(*input + 1) == '>') {
            add_token(tokens, TOKEN_DOUBLE_REDIRECT_RIGHT, ">>");
            (*input)++;
        } else {
            add_token(tokens, TOKEN_REDIRECT_RIGHT, ">");
        }
        (*input)++;
    }
}
