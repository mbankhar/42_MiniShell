/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohame2 <amohame2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 20:54:23 by amohame2          #+#    #+#             */
/*   Updated: 2024/08/28 14:57:41 by amohame2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_exit_arg(const char *arg)
{
	int	i;
	int	digits;

	i = 0;
	digits = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		digits++;
		i++;
	}
	return (digits > 0);
}

long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		else
			sign = 1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

char	*trim_quotes_and_spaces(char *str)
{
	int	len;

	while (*str == ' ')
		str++;
	len = ft_strlen(str);
	while (len > 0 && str[len - 1] == ' ')
		len--;
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\''
				&& str[len - 1] == '\'')))
	{
		str[len - 1] = '\0';
		return (str + 1);
	}
	str[len] = '\0';
	return (str);
}

int	parse_and_validate_exit_arg(char *arg, int *sign, long long *exit_code)
{
	char	*num_start;
	char	*trimmed;

	num_start = arg;
	if (arg[0] == '-' && arg[1] == '"')
	{
		*sign = -1;
		num_start = arg + 1;
	}
	else if (arg[0] == '+' && arg[1] == '"')
	{
		num_start = arg + 1;
	}
	trimmed = trim_quotes_and_spaces(num_start);
	if (!is_valid_exit_arg(trimmed))
	{
		ft_putendl_fd("minishell: exit: numeric argument required", 2);
		exit(255);
	}
	*exit_code = ft_atoll(trimmed) * (*sign);
	return (0);
}

// int	handle_exit(char **args)
// {
// 	int			arg_count;
// 	int			sign;
// 	long long	exit_code;

// 	arg_count = 0;
// 	sign = 1;
// 	while (args[arg_count])
// 		arg_count++;
// 	printf("exit\n");
// 	if (arg_count == 1)
// 	{
// 		exit(0);
// 	}
// 	else if (arg_count >= 2)
// 	{
// 		if (parse_and_validate_exit_arg(args[1], &sign, &exit_code))
// 			return (1);
// 		if (arg_count > 2)
// 		{
// 			ft_putendl_fd("minishell: exit: too many arguments", 2);
// 			ft_putchar_fd('\n', 2);
// 			return (1);
// 		}
// 		exit((exit_code & 0xFF));
// 	}
// 	return (0);
// }
