#include "minishell.h"

void free_parsing_memory(Token **tokens) {
    free_tokens(*tokens);
    *tokens = NULL;
}
