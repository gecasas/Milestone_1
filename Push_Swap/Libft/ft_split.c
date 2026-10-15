/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_split.c                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: abarba-m <abarba-m@student.42malaga.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/23 22:02:49 by abarba-m         #+#    #+#              */
/*   Updated: 2026/09/25 01:34:22 by abarba-m        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	countwords(char const *s, char c)
{
	int	i;
	int	words;
	int	inword;

	words = 0;
	inword = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c && inword == 1)
		{
			inword = 0;
		}
		if (s[i] != c && inword == 0)
		{
			words++;
			inword = 1;
		}
		i++;
	}
	return (words);
}

static int	wordlen(char const *s, char c, int start)
{
	int	len;

	len = 0;
	while (s[start + len] && s[start + len] != c)
		len++;
	return (len);
}

static void	free_split(char **arr, int word)
{
	while (word > 0)
	{
		word--;
		free(arr[word]);
	}
	free(arr);
}

static int	start_split(char const *s, char c, int start)
{
	while (s[start] == c)
		start++;
	if (!s[start])
		return (-1);
	return (start);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		word;
	int		start;
	int		len;

	arr = malloc(sizeof(char *) * (countwords(s, c) + 1));
	if (!arr)
		return (NULL);
	word = 0;
	start = 0;
	while (s[start])
	{
		start = start_split(s, c, start);
		if (start == -1)
			break ;
		len = wordlen(s, c, start);
		arr[word] = malloc(sizeof(char) * (len + 1));
		if (!arr[word])
			return (free_split(arr, word), NULL);
		ft_memcpy(arr[word], s + start, len);
		arr[word++][len] = '\0';
		start += len;
	}
	arr[word] = NULL;
	return (arr);
}
