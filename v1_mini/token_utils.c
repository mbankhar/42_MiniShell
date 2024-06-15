#include "minishell.h"

// Function to create a new token
Token *create_token(TokenType type, const char *value) {
    Token *new_token = (Token *)malloc(sizeof(Token));
    if (!new_token) {
        perror("minishell: malloc");
        exit(1);
    }
    new_token->type = type;
    new_token->value = strdup(value);
    new_token->next = NULL;
    return new_token;
}

// Function to add a token to the list
void add_token(Token **tokens, TokenType type, const char *value) {
    Token *new_token = create_token(type, value);
    if (*tokens == NULL) {
        *tokens = new_token;
    } else {
        Token *temp = *tokens;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_token;
    }
}

// Function to free the list of tokens
void free_tokens(Token *tokens) {
    Token *temp;
    while (tokens) {
        temp = tokens;
        tokens = tokens->next;
        free(temp->value);
        free(temp);
    }
}
