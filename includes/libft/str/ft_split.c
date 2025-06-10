/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 11:24:38 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/10 02:01:43 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static size_t	ft_count(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			++s;
		if (*s)
			++count;
		while (*s && *s != c)
			++s;
	}
	return (count);
}

static void	ft_sfree(char **arr, int i)
{
	while (i >= 0)
	{
		free(arr[i]);
		i--;
	}
	free(arr);
}

static char	**ft_allocate(char **frs, char const *s, char c, size_t size)
{
	size_t		i;
	const char	*start;

	i = 0;
	while (i < size)
	{
		while (*s == c)
			s++;
		if (*s != '\0')
		{
			start = s;
			while (*s && *s != c)
				s++;
			frs[i] = ft_substr(start, 0, s - start);
			if (!frs[i])
			{
				ft_sfree(frs, i);
				return (NULL);
			}
		}
		i++;
	}
	frs[i] = NULL;
	return (frs);
}

char	**ft_split(char const *s, char c)
{
	size_t	size;
	char	**new;

	if (!s)
		return (NULL);
	size = ft_count(s, c);
	new = (char **)malloc(sizeof(char *) * (size + 1));
	if (!new)
		return (NULL);
	new = ft_allocate(new, s, c, size);
	return (new);
}
