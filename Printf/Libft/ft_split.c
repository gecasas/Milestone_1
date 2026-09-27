/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecasas <gecasas@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 17:58:16 by gecasas           #+#    #+#             */
/*   Updated: 2026/09/22 19:46:04 by gecasas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Allocates and returns an array of strings obtained by
splitting 's' using 'c' as delimiter. */

static size_t	ft_countwords(char const *s, char c);
static void		*ft_free(char **str);
static char		**ft_fill_split(char const *s, char c, char **str);

char	**ft_split(char const *s, char c)
{
	char		**str;

	if (!s)
		return (NULL);
	str = ft_calloc(ft_countwords(s, c) + 1, sizeof (char *));
	if (!str)
		return (ft_free(str));
	str = ft_fill_split(s, c, str);
	return (str);
}

static size_t	ft_countwords(char const *s, char c)
{
	size_t	words;
	int		new_word;
	int		i;

	new_word = 1;
	words = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			new_word = 1;
		else
		{
			if (new_word == 1)
			{
				new_word = 0;
				words++;
			}
		}
		i++;
	}
	return (words);
}

static void	*ft_free(char **str)
{
	int	i;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

static char	**ft_fill_split(const char *s, char c, char **str)
{
	size_t		start;
	size_t		len;
	size_t		j;

	start = 0;
	len = 0;
	j = 0;
	while (s[start] != '\0')
	{
		len = 0;
		if (s[start] == c)
			start++;
		else
		{
			while (s[start + len] != c && s[start + len] != '\0')
				len++;
			str[j] = ft_substr(s, start, len);
			if (!str[j])
				return (ft_free(str));
			j++;
			start = start + len;
		}
	}
	return (str);
}
