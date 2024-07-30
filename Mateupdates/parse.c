/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 09:35:59 by mbankhar          #+#    #+#             */
/*   Updated: 2024/07/29 14:42:46 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void do_shit(char **args, t_exec *exec, t_cmds **cmds_ptr, char **env, t_shell *shell)
// {
//     int     i;
//     int     j;
//     int     k;
//     t_cmds  *cmds;

//     i = -1;
//     *cmds_ptr = malloc((exec->number_of_pipes + 1) * sizeof(t_cmds));
//     cmds = *cmds_ptr;
//     i = -1;
//     k = 0;
//     while (args[++i])
//     {
//         j = 0;
//         while (args[i] && args[i][0] != '|')
//         {
//             if (args[i][0] == '<' && args[i][1] == '<')
//             {
//                 i = i + 2;
//                 continue;
//             }
//             else if (args[i][0] == '<' || args[i][0] == '>')
//             {
//                 i = i + 2;
//                 continue;
//             }
//             else
//             {
//                 i++;
//                 j++;
//             }
//         }
//         cmds[k].cmd_args = malloc(sizeof(char *) * (j + 1));
//         k++;
//         if (!args[i])
//             break;
//     }
//     i = -1;
//     k = 0;
//     while (args[++i])
//     {
//         j = 0;
//         cmds[k].fd_in = -2;
//         cmds[k].fd_out = -2;
//         cmds[k].heredoc_content = NULL;
//         cmds[k].heredoc_line_count = 0;
//         while (args[i] && args[i][0] != '|')
//         {
//             if (strcmp(args[i], "<<") == 0 && args[i + 1])
//             {
//                 char *delimiter = args[i + 1];
//                 if (strlen(delimiter) >= MAX_DELIMITER_LENGTH)
//                 {
//                     fprintf(stderr, "Error: Delimiter too long\n");
//                 }
//                 else
//                 {
//                     handle_heredoc(delimiter, &cmds[k], env);
//                 }
//                 i += 2;
//             }
//             else if (args[i][0] == '<' || args[i][0] == '>')
//             {
//                 look_for_redirect(args, i, &cmds[k], env, shell);
//                 i += 2;
//             }
//             else
//             {
//                 cmds[k].cmd_args[j++] = ft_strdup(args[i++]);
//             }
//         }
//         cmds[k].cmd_args[j] = NULL;
//         k++;
//         if (!args[i])
//         {
//             break;
//         }
//     }
//     cmds->size = k;
// }
void do_shit(char **args, t_exec *exec, t_cmds **cmds_ptr, char **env, t_shell *shell)
{
    int     i;
    int     j;
    int     k;
    t_cmds  *cmds;

    i = -1;
    *cmds_ptr = malloc((exec->number_of_pipes + 1) * sizeof(t_cmds));
    cmds = *cmds_ptr;
    i = -1;
    k = 0;
    while (args[++i])
    {
        j = 0;
        while (args[i] && args[i][0] != '|')
        {
            if (args[i][0] == '<' && args[i][1] == '<')
            {
                i = i + 2;
                continue;
            }
            else if (args[i][0] == '<' || args[i][0] == '>')
            {
                i = i + 2;
                continue;
            }
            else
            {
                i++;
                j++;
            }
        }
        cmds[k].cmd_args = malloc(sizeof(char *) * (j + 1));
        k++;
        if (!args[i])
            break;
    }
    i = -1;
    k = 0;
    while (args[++i])
    {
        j = 0;
        cmds[k].fd_in = -2;
        cmds[k].fd_out = -2;
        cmds[k].heredoc_content = NULL;
        cmds[k].heredoc_line_count = 0;
        while (args[i] && args[i][0] != '|')
        {
            if (strcmp(args[i], "<<") == 0 && args[i + 1])
            {
                char *delimiter = args[i + 1];
                if (strlen(delimiter) >= MAX_DELIMITER_LENGTH)
                {
                    fprintf(stderr, "Error: Delimiter too long\n");
                }
                else
                {
                    handle_heredoc(delimiter, &cmds[k], env);
                }
                i += 2;
            }
            else if (args[i][0] == '<' || args[i][0] == '>')
            {
                look_for_redirect(args, i, &cmds[k], env, shell);
                i += 2;
            }
            else
            {
                cmds[k].cmd_args[j++] = ft_strdup(args[i++]);
            }
        }
        cmds[k].cmd_args[j] = NULL;
        k++;
        if (!args[i])
        {
            break;
        }
    }
    cmds->size = k;
}
// Function to remove quotes from a string
void remove_quotes(char **args)
{
    int i;
    int j;
    int k;
    int length;
    char *temp;

    if (args == NULL)
        return;
    i = -1;
    while (args[++i] != NULL)
    {
        length = strlen(args[i]);
        temp = malloc(sizeof(char) * (length + 1));
        if (temp == NULL)
            exit(EXIT_FAILURE);
        k = 0;
        j = -1;
        while (++j < length)
        {
            if (args[i][j] != '\'' && args[i][j] != '\"')
                temp[k++] = args[i][j];
        }
        temp[k] = '\0';
        free(args[i]);
        args[i] = temp;
    }
}
