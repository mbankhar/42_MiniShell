/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:00:00 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 15:03:05 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

int	handle_exit(char **args)
{
	int			arg_count;
	int			sign;
	long long	exit_code;

	arg_count = count_args(args);
	sign = 1;
	printf("exit\n");
	if (arg_count == 1)
	{
		exit(0);
	}
	else if (arg_count >= 2)
	{
		if (parse_and_validate_exit_arg(args[1], &sign, &exit_code))
			return (1);
		if (arg_count > 2)
		{
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			ft_putchar_fd('\n', 2);
			return (1);
		}
		exit((exit_code & 0xFF));
	}
	return (0);
}
