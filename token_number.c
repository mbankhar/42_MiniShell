/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_number.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 10:51:09 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/18 12:50:05 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Function to count the number of valid commands in the tokens array
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
