// commands.c
#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <regex.h>
#include "../builtins/cd.h"
#include "../builtins/env.h"
#include "../builtins/exit.h"
#include "../builtins/export.h"
#include "../builtins/pwd.h"
#include "../builtins/unset.h"
#include "../executor/execute.h"
#include "../minishell.h"
#include "../parser/lexer.h"
#include "../redirections/heredoc.h"

// Function to remove environment variable references from a string
char* remove_dollar_by_env(char *word, Shell *shell) {
    regex_t regex;
    regcomp(&regex, "[!#%^&*()+={}[\\]|:;\\<>,?/]", 0);
    char *result = strdup(word);
    char *dollar_pos = strchr(result, '$');

    while (dollar_pos != NULL) {
        int has_special = 0;
        char *var = NULL;
        char *space_pos = strchr(dollar_pos, ' ');
        if (space_pos != NULL) {
            var = strndup(dollar_pos, space_pos - dollar_pos);
        } else {
            if (!regexec(&regex, dollar_pos + 1, 0, NULL, 0)) {
                has_special = 1;
            } else {
                var = strdup(dollar_pos + 1);
            }
        }
        if (has_special && strstr(dollar_pos, "$?")) {
            snprintf(dollar_pos, 3, "%d", shell->last_error);
        } else {
            const char *value = getenv(var);
            if (value) {
                memmove(dollar_pos + strlen(value), dollar_pos + strlen(var), strlen(dollar_pos) - strlen(var) + 1);
                memcpy(dollar_pos, value, strlen(value));
            } else {
                memmove(dollar_pos, dollar_pos + strlen(var), strlen(dollar_pos) - strlen(var) + 1);
            }
        }
        free(var);
        dollar_pos = strchr(dollar_pos + 1, '$');
    }
    regfree(&regex);
    return result;
}

// Functions for ElementLine struct
ElementLine new_element_line() {
    ElementLine element;
    element.parse_type = END;
    element.value = NULL;
    return element;
}

void select_type(ElementLine *element, const char *value) {
    if (strcmp(value, "|") == 0) {
        element->parse_type = PIPE;
    } else if (strcmp(value, ">") == 0 || strcmp(value, "<") == 0 || strcmp(value, ">>") == 0 || strcmp(value, "<<") == 0) {
        element->parse_type = REDIRECTION;
    } else {
        element->parse_type = WORD;
    }
}

void add_value(ElementLine *element, const char *value) {
    if (element->value == NULL) {
        element->value = strdup(value);
    } else {
        char *new_value = malloc(strlen(element->value) + strlen(value) + 1);
        strcpy(new_value, element->value);
        strcat(new_value, value);
        free(element->value);
        element->value = new_value;
    }
}

const char* get_value(const ElementLine *element) {
    return element->value;
}

const ParseTypes* get_type(const ElementLine *element) {
    return &element->parse_type;
}

int is_builtin(Shell *shell, const char *cmd, char **splitted) {
    int splitted_len = 0;
    while (splitted[splitted_len] != NULL) {
        splitted_len++;
    }

    if (strcmp(cmd, "cd") == 0) {
        shell->error = built_cd(shell, splitted, splitted_len);
        return 1;
    } else if (strcmp(cmd, "env") == 0) {
        shell->error = built_env(shell, splitted, splitted_len);
        return shell->error;
    } else if (strcmp(cmd, "export") == 0) {
        shell->error = built_export(shell, splitted, splitted_len);
        return 1;
    } else if (strcmp(cmd, "pwd") == 0) {
        shell->error = built_pwd(shell, splitted, splitted_len);
        return 1;
    } else if (strcmp(cmd, "unset") == 0) {
        shell->error = built_unset(shell, splitted, splitted_len);
        return 1;
    } else if (strcmp(cmd, "exit") == 0) {
        shell->error = built_exit(shell, splitted, splitted_len);
        return 1;
    } else {
        return 0;
    }
}

char** split_string(ElementLine *element, Shell *shell) {
    char **splitted = malloc(64 * sizeof(char*));
    size_t i = 0;  // Change int to size_t
    int j = 0;
    int in_quotes = 0;
    char *word = malloc(strlen(element->value) + 1);
    strcpy(word, "");
    
    while (i < strlen(element->value)) {
        if (element->value[i] == '\"' || element->value[i] == '\'') {
            in_quotes = !in_quotes;
        } else if (element->value[i] == ' ' && !in_quotes && strlen(word) > 0) {
            word = remove_dollar_by_env(word, shell);
            splitted[j++] = strdup(word);
            strcpy(word, "");
        } else {
            strncat(word, &element->value[i], 1);
        }
        i++;
    }
    if (strlen(word) > 0) {
        word = remove_dollar_by_env(word, shell);
        splitted[j++] = strdup(word);
    }
    free(word);
    splitted[j] = NULL;
    return splitted;
}

ParsedHead* new_parsed_head() {
    ParsedHead *head = malloc(sizeof(ParsedHead));
    head->n_cmds = 0;
    head->tokens = malloc(64 * sizeof(ElementLine));
    return head;
}

void add_token(ParsedHead *head, ElementLine cmd) {
    if (head->tokens[head->n_cmds - 1].parse_type == WORD && cmd.parse_type != REDIRECTION) {
        head->n_cmds++;
    }
    if (head->tokens[head->n_cmds].parse_type == END && cmd.parse_type == WORD) {
        head->n_cmds++;
    }
    head->tokens[head->n_cmds] = cmd;
}
