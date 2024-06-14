/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:01:08 by mbankhar          #+#    #+#             */
/*   Updated: 2024/03/14 11:01:58 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(int n)
{
	int		count;

	count = 0;
	if (n < 0)
	{
		n = -n;
		count++;
	}
	while (n != 0)
	{
		count++;
		n /= 10;
	}
	return (count);
}

static char	*ft_isnull(char *itoastr)
{
	itoastr = (char *)malloc(2 * sizeof(char));
	if (itoastr == NULL)
		return (NULL);
	itoastr[0] = '0';
	itoastr[1] = '\0';
	return (itoastr);
}

static char	*ft_malocandprint(char *itoastr, int count, long x )
{
	int	i;

	itoastr = (char *)malloc((count + 1) * sizeof(char));
	if (itoastr == NULL)
		return (NULL);
	itoastr[count] = '\0';
	i = count - 1;
	while (x != 0)
	{
		itoastr[i] = (x % 10) + '0';
		x /= 10;
		i--;
	}
	return (itoastr);
}

char	*ft_itoa(int n)
{
	char	*itoastr;
	int		count;
	int		neg;
	long	x;

	itoastr = NULL;
	x = n;
	count = ft_count(x);
	neg = 0;
	if (x < 0)
	{
		neg = 1;
		x = -x;
	}
	if (x == 0)
		return (ft_isnull(itoastr));
	else
		itoastr = ft_malocandprint(itoastr, count, x);
	if (itoastr == NULL)
		return (NULL);
	if (neg)
		itoastr[0] = '-';
	return (itoastr);
}
