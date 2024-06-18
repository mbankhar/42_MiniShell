/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 11:03:12 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/18 16:54:20 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
char** parse_commands(const char* input, int* count);
int is_redirection(const char* token);

int main() {
    const char* input = "< out ls | grep \"hello world\" | wc -l < out";
    int count = 0;
    char** commands = parse_commands(input, &count);

    // Print the result
    for (int i = 0; i < count; i++) {
        printf("Command %d: %s\n", i + 1, commands[i]);
        free(commands[i]);
    }
    free(commands);

    return 0;
}

char** parse_commands(const char* input, int* count) {
    char* input_copy = strdup(input); // Make a modifiable copy of the input
    char* token;
    char** result = NULL;
    int num_commands = 0;
    int within_quotes = 0;
    int is_last_token_redirection = 0;
    size_t command_length = 0;

    // Allocate initial memory for result array
    result = malloc(sizeof(char*));
    result[0] = malloc(1);
    result[0][0] = '\0';

    // Tokenize based on spaces to handle each element
    token = strtok(input_copy, " ");
    while (token != NULL) {
        // Check if token contains quotes
        if (strchr(token, '\"') != NULL) {
            within_quotes = !within_quotes;
        }

        // If the previous token was a redirection operator, skip this token
        if (is_last_token_redirection) {
            is_last_token_redirection = 0;
            token = strtok(NULL, " ");
            continue;
        }

        // If the token is a redirection operator, skip it and the next token
        if (is_redirection(token)) {
            is_last_token_redirection = 1;
            token = strtok(NULL, " ");
            continue;
        }

        // Calculate the required length for the command
        command_length = strlen(result[num_commands]) + strlen(token) + 2;

        // Reallocate memory for the current command
        result[num_commands] = realloc(result[num_commands], command_length);

        // Append token to the last command in result array
        if (within_quotes || strchr(token, '|') == NULL) {
            if (result[num_commands][0] != '\0') {
                strcat(result[num_commands], " ");
            }
            strcat(result[num_commands], token);
        } else {
            // Split by pipe and start a new command
            num_commands++;
            result = realloc(result, (num_commands + 1) * sizeof(char*));
            result[num_commands] = malloc(strlen(token) + 1);
            strcpy(result[num_commands], token + 1); // Skip the pipe character
        }

        token = strtok(NULL, " ");
    }

    // Trim leading spaces from each command
    for (int i = 0; i <= num_commands; i++) {
        char* cmd = result[i];
        while (*cmd == ' ') cmd++;
        memmove(result[i], cmd, strlen(cmd) + 1);
    }

    free(input_copy);
    *count = num_commands + 1;
    return result;
}

int is_redirection(const char* token) {
    return (strcmp(token, "<") == 0 || strcmp(token, ">") == 0 || 
            strcmp(token, "<<") == 0 || strcmp(token, ">>") == 0);
}
