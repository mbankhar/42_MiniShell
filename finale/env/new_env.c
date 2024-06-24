#include "new_env.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

// Initialize a new environment
Env new_env() {
    Env env;
    env.vars = NULL;
    env.size = 0;

    extern char **environ;
    for (char **envp = environ; *envp != NULL; envp++) {
        char *entry = strdup(*envp);
        if (!entry) {
            perror("strdup");
            continue;
        }

        char *key = strtok(entry, "=");
        if (!key) {
            free(entry);
            continue;
        }

        char *value = strtok(NULL, "");
        if (!value) {
            free(entry);
            continue;
        }

        printf("Setting environment variable: %s=%s\n", key, value); // Debug output
        set_env(&env, key, value);
        free(entry);
    }
    return env;
}

// Get an environment variable value
const char* get_env(Env *env, const char *key) {
    for (size_t i = 0; i < env->size; ++i) {
        if (strcmp(env->vars[i].key, key) == 0) {
            return env->vars[i].value;
        }
    }
    return NULL;
}

// Set an environment variable
void set_env(Env *env, const char *key, const char *value) {
    for (size_t i = 0; i < env->size; ++i) {
        if (strcmp(env->vars[i].key, key) == 0) {
            free(env->vars[i].value);
            env->vars[i].value = strdup(value);
            if (!env->vars[i].value) {
                perror("strdup");
                exit(EXIT_FAILURE);
            }
            return;
        }
    }
    env->size++;
    EnvVar *new_vars = realloc(env->vars, env->size * sizeof(EnvVar));
    if (!new_vars) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    env->vars = new_vars;
    env->vars[env->size - 1].key = strdup(key);
    env->vars[env->size - 1].value = strdup(value);
    if (!env->vars[env->size - 1].key || !env->vars[env->size - 1].value) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
}

// Remove an environment variable
void remove_env(Env *env, const char *key) {
    for (size_t i = 0; i < env->size; ++i) {
        if (strcmp(env->vars[i].key, key) == 0) {
            free(env->vars[i].key);
            free(env->vars[i].value);
            for (size_t j = i; j < env->size - 1; ++j) {
                env->vars[j] = env->vars[j + 1];
            }
            env->size--;
            if (env->size == 0) {
                free(env->vars);
                env->vars = NULL;
            } else {
                EnvVar *new_vars = realloc(env->vars, env->size * sizeof(EnvVar));
                if (!new_vars) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }
                env->vars = new_vars;
            }
            return;
        }
    }
}

// Get all environment variables
EnvVar* get_all(Env *env) {
    return env->vars;
}

size_t get_env_size(Env *env) {
    return env->size;
}

// Validate if a command exists in the PATH
int path_validation(Env *env, const char *cmd) {
    const char *path_env = get_env(env, "PATH");
    if (path_env == NULL) {
        return 0;
    }
    char *path = strdup(path_env);
    if (!path) {
        perror("strdup");
        return 0;
    }
    char *dir = strtok(path, ":");
    while (dir != NULL) {
        char tmp_path[1024];
        snprintf(tmp_path, sizeof(tmp_path), "%s/%s", dir, cmd);
        struct stat buffer;
        if (stat(tmp_path, &buffer) == 0) {
            free(path);
            return 1;
        }
        dir = strtok(NULL, ":");
    }
    free(path);
    return 0;
}
