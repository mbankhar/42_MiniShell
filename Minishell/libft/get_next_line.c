/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbankhar <mbankhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 08:11:41 by mbankhar          #+#    #+#             */
/*   Updated: 2024/04/30 16:50:55 by mbankhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	buffer_clean(char *buffer)
{
	int		i;
	int		y;
	char	temp[BUFFER_SIZE + 1];

	y = 0;
	i = 0;
	if (my_strchr(buffer) != -1)
	{
		while (buffer[i] != '\n')
			i++;
		i++;
		while (buffer[i] != '\0')
			temp[y++] = buffer[i++];
		temp[y] = '\0';
		i = 0;
		while (temp[i])
		{
			buffer[i] = temp[i];
			i++;
		}
		buffer[i] = '\0';
	}
	else
		buffer[0] = '\0';
}

char	*my_strjoin(char *str, char *buffer)
{
	int		len_str;
	int		len_buffer;
	char	*new_string;

	len_str = my_strlen((char *)str);
	len_buffer = my_strlen((char *)buffer);
	new_string = (char *)malloc((len_str + len_buffer + 1) * sizeof(char));
	if (new_string == NULL)
	{
		free(str);
		return (NULL);
	}
	ft_strcpy(new_string, str);
	ft_strcpynl(&new_string[len_str], buffer);
	free(str);
	buffer_clean(buffer);
	return (new_string);
}

char	*my_strjoinend(char *str, char *buffer)
{
	int		len_str;
	int		len_buffer;
	char	*new_string;

	len_str = my_strlen((char *)str);
	len_buffer = my_strlen((char *)buffer);
	new_string = (char *)malloc((len_str + len_buffer + 1) * sizeof(char));
	if (new_string == NULL)
	{
		free(str);
		return (NULL);
	}
	ft_strcpy(new_string, str);
	ft_strcpynl(&new_string[len_str], buffer);
	free(str);
	buffer_clean(buffer);
	return (new_string);
}

char	*get_next_line(int fd)
{
	char		*str;
	static char	buffer[BUFFER_SIZE + 1];

	str = NULL;
	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	str = my_strjoin(str, buffer);
	if (str == 0)
		return (NULL);
	return (ft_read(str, buffer, fd, 1));
}

// int	main(void)
// {
// 	int fd;
// 	char *line;
// 	char *filename;

// 	filename = "test.txt";
// 	fd = open(filename, O_RDONLY);
// 	if (fd == -1)
// 	{
// 		perror("Error opening file");
// 		return (1);
// 	}
// 	// while (line && line[0] != '\0')
// 	// {
// 	// 	puts(line);
// 	// 	line = get_next_line(fd);
// 	// }
// 	line = get_next_line(fd);
// 	printf("First line: %s\n", line);
// 	// line = get_next_line(fd);
// 	// printf("2nd line: %s\n", line);
// 	// line = get_next_line(fd);
// 	//  printf("3nd line: %s\n", line);
// 	// line = get_next_line(fd);
// 	//  printf("4nd line: %s\n", line);
// 	// line = get_next_line(fd);
// 	//  printf("5nd line: %s\n", line);
// 	// line = get_next_line(fd);
// 	//  printf("6nd line: %s\n", line);

// 	close(fd);
// 	return (0);
// }
