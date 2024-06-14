/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 10:00:00 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/14 11:42:04 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_variable(char *var_name) {
    if (strcmp(var_name, "?") == 0) {
        int status;
        waitpid(-1, &status, WNOHANG);
        char status_str[16];
        snprintf(status_str, sizeof(status_str), "%d", WEXITSTATUS(status));
        return strdup(status_str);
    }
    char *value = getenv(var_name);
    return (value != NULL) ? strdup(value) : strdup("");
}

void expand_variables_in_tokens(char **tokens) {
    for (int i = 0; tokens[i] != NULL; i++) {
        char *token = tokens[i];
        if (token[0] == '$') {
            char *var_name = token + 1;
            char *expanded_value = expand_variable(var_name);
            free(tokens[i]);
            tokens[i] = expanded_value;
        }
    }
}

Command *parse_input(char *line) {
    Command *cmd = malloc(sizeof(Command));
    if (!cmd) {
        perror("minishell: malloc");
        exit(1);
    }
    cmd->num_commands = 0;
    int current_command = 0;
    int i = 0;
    int in_quotes = 0;
    char *start = line;
    int escape_next = 0;

    for (int k = 0; k < MAX_COMMANDS; k++) {
        cmd->commands[k].tokens = malloc(MAX_TOKENS * sizeof(char *));
        if (!cmd->commands[k].tokens) {
            perror("minishell: malloc");
            exit(1);
        }
        cmd->commands[k].flags = malloc(MAX_FLAGS * sizeof(char *));
        if (!cmd->commands[k].flags) {
            perror("minishell: malloc");
            exit(1);
        }
        cmd->commands[k].num_flags = 0;
        cmd->commands[k].input_fd = 0;
        cmd->commands[k].output_fd = 1;
    }

    for (char *c = line; *c != '\0'; c++) {
        if (escape_next) {
            escape_next = 0;
            continue;
        }

        if (*c == '\\') {
            escape_next = 1;
            continue;
        }

        if (*c == '\'' || *c == '"') {
            if (in_quotes == 0) {
                in_quotes = (*c == '\'') ? 1 : 2;
                start = c + 1;
            } else if ((in_quotes == 1 && *c == '\'') || (in_quotes == 2 && *c == '"')) {
                in_quotes = 0;
                *c = '\0';
                cmd->commands[current_command].tokens[i++] = strdup(start);
                start = c + 1;
            }
        } else if (*c == ' ' && in_quotes == 0) {
            *c = '\0';
            if (start != c) {
                cmd->commands[current_command].tokens[i++] = strdup(start);
            }
            start = c + 1;
        } else if (*c == '<' && in_quotes == 0) {
            *c = '\0';
            if (start != c) {
                cmd->commands[current_command].tokens[i++] = strdup(start);
            }
            start = c + 1;
            while (*start == ' ') start++;
            cmd->commands[current_command].redirect_file = strdup(start);
            cmd->commands[current_command].type = CMD_REDIRECT_IN;
            break;
        } else if (*c == '>' && in_quotes == 0) {
            *c = '\0';
            if (start != c) {
                cmd->commands[current_command].tokens[i++] = strdup(start);
            }
            start = c + 1;
            while (*start == ' ') start++;
            if (*start == '>') {
                start++;
                while (*start == ' ') start++;
                cmd->commands[current_command].type = CMD_REDIRECT_APPEND;
            } else {
                cmd->commands[current_command].type = CMD_REDIRECT_OUT;
            }
            cmd->commands[current_command].redirect_file = strdup(start);
            break;
        } else if (*c == '|' && in_quotes == 0) {
            *c = '\0';
            if (start != c) {
                cmd->commands[current_command].tokens[i++] = strdup(start);
            }
            cmd->commands[current_command].tokens[i] = NULL;
            start = c + 1;
            i = 0;
            current_command++;
            cmd->commands[current_command].type = CMD_PIPE;
        }
    }

    if (start != line && *start != '\0') {
        cmd->commands[current_command].tokens[i++] = strdup(start);
    }

    cmd->commands[current_command].tokens[i] = NULL;
    cmd->num_commands = current_command + 1;

    for (int k = 0; k < cmd->num_commands; k++) {
        expand_variables_in_tokens(cmd->commands[k].tokens);
    }

    return cmd;
}

void free_command(Command *cmd) {
    for (int i = 0; i < cmd->num_commands; i++) {
        for (int j = 0; cmd->commands[i].tokens[j] != NULL; j++) {
            free(cmd->commands[i].tokens[j]);
        }
        free(cmd->commands[i].tokens);
        free(cmd->commands[i].flags);
        if (cmd->commands[i].redirect_file) {
            free(cmd->commands[i].redirect_file);
        }
    }
    free(cmd);
}

int count_pipes(const char *line) {
    int count = 0;
    while (*line) {
        if (*line == '|') {
            count++;
        }
        line++;
    }
    return count;
}
