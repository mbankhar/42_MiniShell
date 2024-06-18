/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 11:03:12 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/18 11:52:11 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Placeholder functions to be replaced with actual implementations
char **ft_split(const char *str, char delimiter);
char *ft_substr(const char *str, unsigned int start, size_t len);
char *ft_strjoinslesh(const char *s1, const char *s2);
void ft_free(char **strs);

char *altpath(char **env, char *cmd, int i) {
    char **realpath;
    char *string = NULL;

    realpath = ft_split(env[i], ':');
    realpath[0] = ft_substr(realpath[0], 5, strlen(realpath[0]) - 5);
    if (realpath[0] == NULL) {
        ft_free(realpath);
        return NULL;
    }
    i = -1;
    while (realpath[++i]) {
        string = ft_strjoinslesh(realpath[i], cmd);
        if (string == NULL) {
            ft_free(realpath);
            return NULL;
        }
        if (access(string, X_OK) == 0)
            break;
        free(string);
        string = NULL;
    }
    ft_free(realpath);
    return string;
}

char *get_path(char **env, char *cmd) {
    int i = 0;
    char *string;

    while (env[i]) {
        if (strncmp(env[i], "PATH=", 5) == 0) {
            break;
        }
        i++;
    }
    if (env[i] == NULL) {
        string = strdup(cmd);
        if (access(string, X_OK) != 0) {
            free(string);
            return NULL;
        }
    } else {
        string = altpath(env, cmd, i);
    }
    return string;
}

int get_token_number(char **tokens, char **env) {
    int count = 0;
    char *path;

    for (int i = 0; tokens[i] != NULL; i++) {
        path = get_path(env, tokens[i]);
        if (path != NULL) {
            count++;
            free(path);
        }
    }
    return count;
}

int main(int argc, char *argv[], char *envp[]) {
    // Example input tokens
    char *tokens[] = {"null", NULL};
    int count = get_token_number(tokens, envp);
    printf("Number of valid commands: %d\n", count);
    return 0;
}

// Example implementations for custom functions
char **ft_split(const char *str, char delimiter) {
    char **result;
    int count = 1;
    for (const char *temp = str; *temp; temp++) {
        if (*temp == delimiter) count++;
    }
    result = malloc((count + 1) * sizeof(char *));
    int index = 0;
    const char *start = str;
    for (const char *temp = str; *temp; temp++) {
        if (*temp == delimiter) {
            int length = temp - start;
            result[index] = strndup(start, length);
            index++;
            start = temp + 1;
        }
    }
    result[index] = strdup(start);
    result[count] = NULL;
    return result;
}

char *ft_substr(const char *str, unsigned int start, size_t len) {
    char *substr = malloc(len + 1);
    if (!substr) return NULL;
    strncpy(substr, str + start, len);
    substr[len] = '\0';
    return substr;
}

char *ft_strjoinslesh(const char *s1, const char *s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 2);
    if (!result) return NULL;
    strcpy(result, s1);
    result[len1] = '/';
    strcpy(result + len1 + 1, s2);
    return result;
}

void ft_free(char **strs) {
    for (int i = 0; strs[i] != NULL; i++) {
        free(strs[i]);
    }
    free(strs);
}
