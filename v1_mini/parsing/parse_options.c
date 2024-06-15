#include "minishell.h"

void parse_options(char *input, Token **tokens) {
    if (*input == '-') {
        char *start = input;
        while (*input && *input != ' ' && *input != '\t') {
            input++;
        }
        char *option = strndup(start, input - start);
        add_token(tokens, TOKEN_OPTION, option);
        free(option);
    }
}
