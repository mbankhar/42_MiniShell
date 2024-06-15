#include "minishell.h"

void parse_error(char *error_msg, Token **tokens) {
    add_token(tokens, TOKEN_ERROR, error_msg);
}
