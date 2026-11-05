/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 08:37:56 by gecasas           #+#    #+#             */
/*   Updated: 2026/11/05 12:09:27 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_fill_buffer(int fd, char *to_read, char *buffer);
static char	*ft_search_nl(char **to_read);

char	*get_next_line(int fd)
{
	static char	*to_read;
	char		*buffer;
	char		*line;

	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(to_read);
		free(buffer);
		to_read = NULL;
		buffer = NULL;
		return (NULL);
	}
	if (!buffer)
		return (NULL);
	to_read = ft_fill_buffer(fd, to_read, buffer);
	line = ft_search_nl(&to_read);
	free(buffer);
	return (line);
}

static char	*ft_fill_buffer(int fd, char *to_read, char *buffer)
{
	ssize_t	readed;
	char	*tmp;

	readed = 1;
	while (readed > 0 && !ft_strchr(to_read, '\n'))
	{
		readed = read(fd, buffer, BUFFER_SIZE);
		if (readed == -1)
		{
			free (to_read);
			return (NULL);
		}
		if (readed == 0)
			break ;
		buffer[readed] = '\0';
		tmp = to_read;
		if (!tmp)
			to_read = ft_strdup(buffer);
		else
			to_read = ft_strjoin(tmp, buffer);
		free(tmp);
		if (!to_read)
			return (NULL);
	}
	return (to_read);
}

static char	*ft_search_nl(char **to_read)
{
	char	*new_line;
	char	*rest;
	size_t	i;

	if (!*to_read)
		return (NULL);
	i = 0;
	while ((*to_read)[i] && (*to_read)[i] != '\n')
		i++;
	if ((*to_read)[i] == '\n')
		i++;
	new_line = ft_substr(*to_read, 0, i);
	rest = NULL;
	if ((*to_read)[i])
		rest = ft_substr(*to_read, i, ft_strlen(*to_read) - i);
	if (!new_line || ((*to_read)[i] && !rest))
	{
		free(new_line);
		free(rest);
		new_line = NULL;
		rest = NULL;
	}
	free(*to_read);
	*to_read = rest;
	return (new_line);
}

void	*ft_calloc(size_t count, size_t size)
{
	char	*str;
	size_t	total;

	total = count * size;
	if (count != 0 && (total / count) != size)
		return (NULL);
	str = malloc(total);
	if (!str)
		return (NULL);
	ft_memzero(str, '\0', size * count);
	return (str);
}

void	*ft_memzero(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;

	str = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		str[i] = (unsigned char)c;
		i++;
	}
	return (s);
}
