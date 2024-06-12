// parsing.c
#include "minishell.h"

Command *parse_input(char *line) {
    Command *cmd = malloc(sizeof(Command));
    cmd->tokens = malloc(MAX_TOKENS * sizeof(char *));
    cmd->input_fd = 0; // Default to stdin
    cmd->output_fd = 1; // Default to stdout

    int i = 0;
    int in_quotes = 0;
    char *start = line;
    int escape_next = 0; // Flag to indicate if the next character is escaped

    for (char *c = line; *c != '\0'; c++) {
        if (escape_next) {
            escape_next = 0; // Reset the escape flag
            continue; // Skip the escaped character
        }

        if (*c == '\\') {
            escape_next = 1; // Set the escape flag for the next character
            continue;
        }

        if (*c == '\'' || *c == '"') {
            if (in_quotes == 0) {
                in_quotes = (*c == '\'') ? 1 : 2;
                start = c + 1;
            } else if ((in_quotes == 1 && *c == '\'') || (in_quotes == 2 && *c == '"')) {
                in_quotes = 0;
                *c = '\0';
                cmd->tokens[i++] = start;
                start = c + 1;
            }
        } else if (*c == ' ' && in_quotes == 0) {
            *c = '\0';
            if (start != c) { // Avoid empty tokens
                cmd->tokens[i++] = start;
            }
            start = c + 1;
        } else if (*c == '<' && in_quotes == 0) { // Input redirection
            *c = '\0';
            if (start != c) {
                cmd->tokens[i++] = start;
            }
            start = c + 1;
            while (*start == ' ') start++;
            cmd->input_fd = open(start, O_RDONLY);
            if (cmd->input_fd == -1) {
                perror("minishell: open");
                exit(1);
            }
            break;
        } else if (*c == '>' && in_quotes == 0) { // Output redirection
            *c = '\0';
            if (start != c) {
                cmd->tokens[i++] = start;
            }
            start = c + 1;
            int flags = O_WRONLY | O_CREAT | O_TRUNC;
            if (*start == '>') { // Append mode
                flags = O_WRONLY | O_CREAT | O_APPEND;
                start++;
            }
            while (*start == ' ') start++;
            cmd->output_fd = open(start, flags, 0644);
            if (cmd->output_fd == -1) {
                perror("minishell: open");
                exit(1);
            }
            break;
        }
    }

    if (start != line + strlen(line)) { // Add the last token
        cmd->tokens[i++] = start;
    }
    cmd->tokens[i] = NULL;

    // Environment variable expansion
    for (int j = 0; cmd->tokens[j] != NULL; j++) {
        if (cmd->tokens[j][0] == '$') {
            char *var_name = cmd->tokens[j] + 1;
            char *var_value = getenv(var_name);
            if (var_value != NULL) {
                cmd->tokens[j] = var_value;
            }
        }
    }

    return cmd;
}

void free_command(Command *cmd) {
    free(cmd->tokens);
    free(cmd);
}
