/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:26:18 by mbankhar          #+#    #+#             */
/*   Updated: 2024/04/30 16:42:31 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_read(char *str, char *buffer, int fd, int chars_read)
{
	while (chars_read)
	{
		if (my_strchr(str) != -1)
			return (str);
		if (!str)
			return (NULL);
		chars_read = read(fd, buffer, BUFFER_SIZE);
		if (chars_read == 0 || chars_read == -1)
		{
			if (str[0] == '\0' || chars_read == -1)
			{
				if (str)
					free(str);
				return (NULL);
			}
			else
				return (str);
		}
		buffer[chars_read] = '\0';
		str = my_strjoin(str, &buffer[0]);
		if (!str)
			return (NULL);
	}
	return (my_strjoin(str, &buffer[0]));
}

int	my_strchr(char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (-1);
	while (s[i] != '\0')
	{
		if (s[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

void	ft_strcpynl(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s2[i] != '\0' && s2[i] != '\n')
	{
		s1[i] = s2[i];
		i++;
	}
	if (s2[i] == '\n')
	{
		s1[i] = '\n';
		s1[i + 1] = '\0';
	}
	else
		s1[i] = '\0';
}

void	ft_strcpy(char *dest, char *src)
{
	int	i;

	if (src == NULL)
		return ;
	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

int	my_strlen(char *c)
{
	int	i;

	i = 0;
	if (c == NULL)
		return (0);
	while (c[i] != '\0' && c[i] != '\n')
	{
		i++;
	}
	if (c[i] == '\n')
		i++;
	return (i);
}
