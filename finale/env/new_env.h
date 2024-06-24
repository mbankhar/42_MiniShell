#ifndef NEW_ENV_H
#define NEW_ENV_H

#include <stddef.h>  // Add this line

typedef struct {
    char *key;
    char *value;
} EnvVar;

typedef struct {
    EnvVar *vars;
    size_t size;
} Env;

Env new_env();
const char* get_env(Env *env, const char *key);
void set_env(Env *env, const char *key, const char *value);
EnvVar* get_all(Env *env);
size_t get_env_size(Env *env);
void remove_env(Env *env, const char *key);
int path_validation(Env *env, const char *cmd);

#endif // NEW_ENV_H
