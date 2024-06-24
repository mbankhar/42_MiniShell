// commands.h
#ifndef COMMANDS_H
#define COMMANDS_H

#include "../minishell.h"

struct Shell;  // Forward declaration
struct Pipe;   // Forward declaration

typedef enum {
    WORD,
    PIPE,
    REDIRECTION,
    END
} ParseTypes;

typedef struct {
    ParseTypes parse_type;
    char *value;
} ElementLine;

typedef struct ParsedHead {
    int n_cmds;
    ElementLine *tokens;
} ParsedHead;

ElementLine new_element_line();
void select_type(ElementLine *element, const char *value);
void add_value(ElementLine *element, const char *value);
const char* get_value(const ElementLine *element);
const ParseTypes* get_type(const ElementLine *element);
int is_builtin(struct Shell *shell, const char *cmd, char **splitted);
char** split_string(ElementLine *element, struct Shell *shell);

ParsedHead* new_parsed_head();  // Updated to return a pointer
void add_token(ParsedHead *head, ElementLine cmd);

ParsedHead* parser(const char *line, int *error);  // Updated to return a pointer

#endif // COMMANDS_H
