/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 08:37:56 by gecasas           #+#    #+#             */
/*   Updated: 2026/10/27 21:27:25 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char    *ft_fill_buffer(int fd, char *to_read, char *buffer)
{
    ssize_t    readed;
    char    *tmp;

    readed = 1;
    while (readed > 0)
    {
        if (ft_strchr(to_read, '\n'))
            break ;
        readed = read(fd, buffer, BUFFER_SIZE);
        if (readed == -1)
        {
            free (to_read);
            return (NULL);
        }
        else if (readed == 0)
            break ;
        buffer[readed] = '\0';
        if (!to_read)
            to_read = ft_strdup("");
        tmp = to_read;
        to_read = ft_strjoin(tmp, buffer);
        free(tmp);
        tmp = NULL;
    }
    return (to_read);
}

char	*ft_search_nl(char **to_read)
{
	char	*new_line;
	char	*tmp;
	int		i;

	i = 0;
	while (*to_read[i] != '\n' && *to_read[i] != '\0')
	{
		new_line[i] = *to_read[i];
		i++;
	}
	tmp = ft_substr(*to_read, i + 1, ft_strlen(*to_read - i));
	*to_read = tmp;
	tmp = NULL;
	return (new_line);
}

char    *get_next_line(int fd)
{
    static char    *to_read;
    char        	*buffer;
    char        	*line;
    
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
	//buscar '\n' y dejarlo en la linea devuelta, despues estatica empieza justo despues
}
