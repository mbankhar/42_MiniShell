#include "parsing/minishell.h"



const char *token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_AND: return "TOKEN_AND";
        case TOKEN_OR: return "TOKEN_OR";
        case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
        case TOKEN_REDIRECT_LEFT: return "TOKEN_REDIRECT_LEFT";
        case TOKEN_REDIRECT_RIGHT: return "TOKEN_REDIRECT_RIGHT";
        case TOKEN_DOUBLE_REDIRECT_LEFT: return "TOKEN_DOUBLE_REDIRECT_LEFT";
        case TOKEN_DOUBLE_REDIRECT_RIGHT: return "TOKEN_DOUBLE_REDIRECT_RIGHT";
        case TOKEN_PIPELINE: return "TOKEN_PIPELINE";
        case TOKEN_COMMAND: return "TOKEN_COMMAND";
        case TOKEN_OPTION: return "TOKEN_OPTION";
        case TOKEN_SINGLE_QUOTE: return "TOKEN_SINGLE_QUOTE";
        case TOKEN_DOUBLE_QUOTE: return "TOKEN_DOUBLE_QUOTE";
        case TOKEN_BACKSLASH: return "TOKEN_BACKSLASH";
        case TOKEN_LITERAL: return "TOKEN_LITERAL";
        case TOKEN_VARIABLE: return "TOKEN_VARIABLE";
        case TOKEN_SPACE: return "TOKEN_SPACE";
        case TOKEN_ERROR: return "TOKEN_ERROR";
        default: return "UNKNOWN_TOKEN";
    }
}

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
