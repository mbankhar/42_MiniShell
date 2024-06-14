/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 10:00:00 by mbankhar          #+#    #+#             */
/*   Updated: 2024/06/14 11:42:42 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp) {
    char *line;
    Command *cmd;

    while (1) {
        line = readline("minishell> ");
        if (!line) {
            break; // EOF
        }
        if (strlen(line) > 0) {
            add_history(line);
            cmd = parse_input(line);
            if (cmd->num_commands > 0) {
                execute_command(cmd);
            }
            free_command(cmd);
        }
        free(line);
    }

    return 0;
}
