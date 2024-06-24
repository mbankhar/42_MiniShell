// lexer.c
#include "lexer.h"
#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int check_error(const char *line) {
    if (strlen(line) >= 2) {
        if (strstr(line, "||") != NULL || strstr(line, "&&") != NULL) {
            fprintf(stderr, "minishell: parse error near `%c%c'\n", line[0], line[1]);
            return 2;
        }
    } else if (strlen(line) > 1 && line[0] == '|') {
        fprintf(stderr, "minishell: parse error near `|'\n");
        return 2;
    }
    return 0;
}

ElementLine parse_pipe(ParsedHead *tokens, size_t *i, ElementLine *last_add, int *error) {
    ElementLine element = new_element_line();
    if (*get_type(last_add) != WORD) {
        fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", get_value(last_add));
        *error = 2;
        return element;
    }
    select_type(&element, "|");
    add_value(&element, "|");
    add_token(tokens, element);
    (*i)++;
    *error = 0;
    return tokens->tokens[tokens->n_cmds - 1];
}

ElementLine parse_redirection(ParsedHead *tokens, const char *line, size_t *i, ElementLine *last_add, int *error) {
    ElementLine element = new_element_line();
    char word[3] = {0};
    if (*get_type(last_add) != WORD) {
        fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", get_value(last_add));
        *error = 2;
        return element;
    }
    if (*i + 1 < strlen(line) && line[*i] == line[*i + 1]) {
        strncpy(word, &line[*i], 2);
        *i += 2;
    } else {
        strncpy(word, &line[*i], 1);
        *i += 1;
    }
    select_type(&element, word);
    add_value(&element, word);
    add_token(tokens, element);
    *error = 0;
    return tokens->tokens[tokens->n_cmds - 1];
}

size_t validade_quote(const char *line, size_t index, bool *error) {
    const char *quote = strchr(line + index + 1, line[index]);
    if (quote) {
        *error = false;
        return quote - line - index - 1;
    } else {
        fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
        *error = true;
        return 0;
    }
}

ls
    ElementLine element = new_element_line();
    char word[1024] = {0};

    while (*i < strlen(line)) {
        if (line[*i] == '\"' || line[*i] == '\'') {
            bool quote_error;
            size_t quote_pos = validade_quote(line, *i, &quote_error);
            strncat(word, &line[*i], quote_pos + 2);
            *i += quote_pos + 2;
            if (quote_error) {
                *error = 2;
                return element;
            }
        } else if (strchr("|><&", line[*i]) != NULL) {
            (*i)--;
            break;
        } else {
            strncat(word, &line[*i], 1);
            (*i)++;
        }
    }

    select_type(&element, word);
    add_value(&element, word);

    if (*get_type(last_type) == PIPE || *get_type(last_type) == END || *get_type(last_type) == REDIRECTION) {
        add_token(tokens, element);
        *error = 0;
        return tokens->tokens[tokens->n_cmds - 1];
    } else {
        fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
        *error = 2;
        return element;
    }
}

ParsedHead* parser(const char *line, int *error) {
    ParsedHead *tokens = new_parsed_head();
    char *trimmed = strdup(line);
    if (!trimmed) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    trimmed = strtok(trimmed, "\n");
    size_t i = 0;
    ElementLine last_type = new_element_line();
    *error = check_error(trimmed);

    while (i < strlen(trimmed) && *error == 0) {
        switch (trimmed[i]) {
            case '|':
                last_type = parse_pipe(tokens, &i, &last_type, error);
                break;
            case '>':
            case '<':
                last_type = parse_redirection(tokens, trimmed, &i, &last_type, error);
                break;
            case ' ':
                break;
            default:
                last_type = parse_word(tokens, trimmed, &i, &last_type, error);
                break;
        }
        i++;
    }
    free(trimmed);
    return tokens;
}
