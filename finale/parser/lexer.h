// lexer.h
#ifndef LEXER_H
#define LEXER_H

#include "commands.h"
#include <stdbool.h>  // Add this line

int check_error(const char *line);
ElementLine parse_pipe(ParsedHead *tokens, size_t *i, ElementLine *last_add, int *error);
ElementLine parse_redirection(ParsedHead *tokens, const char *line, size_t *i, ElementLine *last_add, int *error);
ElementLine parse_word(ParsedHead *tokens, const char *line, size_t *i, ElementLine *last_add, int *error);
size_t validade_quote(const char *line, size_t index, bool *error);

ParsedHead* parser(const char *line, int *error);

#endif // LEXER_H
