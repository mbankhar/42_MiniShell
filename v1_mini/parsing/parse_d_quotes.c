#include "minishell.h"

void parse_d_quotes(char **input, Token **tokens) {
    char *start = ++(*input); // Skip the opening double quote
    while (**input && **input != '"') {
        (*input)++;
    }
    if (**input == '"') {
        char *quoted_str = strndup(start, *input - start);
        add_token(tokens, TOKEN_DOUBLE_QUOTE, quoted_str);
        free(quoted_str);
        (*input)++; // Skip the closing double quote
    } else {
        // Handle error for unclosed double quote
        add_token(tokens, TOKEN_ERROR, "Unclosed double quote");
    }
}