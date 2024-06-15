#include "parsing/minishell.h"

// This function tokenizes the input string
Token *tokenize_input(char *input) {
    Token *tokens = NULL;
    bool is_command = true;

    while (*input) {
        if (*input == ' ' || *input == '\t') {
            input++;  // Skip spaces and tabs
        } else if (*input == '|') {
            parse_pipe(&input, &tokens);
            is_command = true; // Next token after a pipe is a command
        } else if (*input == ';') {
            parse_sep(&input, &tokens);
            is_command = true; // Next token after a semicolon is a command
        } else if (*input == '<' || *input == '>') {
            parse_redir(&input, &tokens);
            is_command = false; // Redirection does not affect command/argument distinction
        } else if (*input == '-') {
            parse_options(input, &tokens);
            input += strlen(tokens->value); // Skip the option
            is_command = false;
        } else if (*input == '"') {
            parse_d_quotes(&input, &tokens);
            is_command = false;
        } else if (*input == '$') {
            parse_varenv(&input, &tokens);
            is_command = false;
        } else {
            parse_args(&input, &tokens, is_command);
            is_command = false; // Subsequent tokens are arguments
        }
    }
    return tokens;
}