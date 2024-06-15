#include "parsing/minishell.h"

// This function tokenizes the input string
Token *tokenize_input(char *input) {
    Token *tokens = NULL;
    while (*input) {
        if (*input == ' ' || *input == '\t') {
            input++;  // Skip spaces and tabs
        } else if (*input == '|') {
            parse_pipe(&input, &tokens);
        } else if (*input == ';') {
            parse_sep(&input, &tokens);
        } else if (*input == '<' || *input == '>') {
            parse_redir(&input, &tokens);
        } else if (*input == '-') {
            parse_options(input, &tokens);
            input += strlen(tokens->value); // Skip the option
        } else if (*input == '"') {
            parse_d_quotes(&input, &tokens);
        } else if (*input == '$') {
            parse_varenv(&input, &tokens);
        } else {
            parse_args(&input, &tokens);
        }
    }
    return tokens;
}
