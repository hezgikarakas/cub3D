/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaraka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:58:26 by hakaraka          #+#    #+#             */
/*   Updated: 2022/10/19 11:58:29 by hakaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_split_count_words(const char *s, char c)
{
	int	at;
	int	words;

	at = 0;
	words = 0;
	while (s[at])
	{
		while (s[at] && s[at] == c)
			at++;
		if (s[at] && s[at] != c)
		{
			words++;
			while (s[at] && s[at] != c)
				at++;
		}
	}
	return (words);
}

static int	ft_len_of_word(const char *s, int i, char c)
{
	int	start;

	start = i;
	while (s[i] && s[i] != c)
		i++;
	return (i - start);
}

static void	ft_split_extract(const char *s, char c, char **result)
{
	int	i;
	int	word;
	int	len;

	i = 0;
	word = 0;
	while (s[i] && result != NULL)
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i])
		{
			len = ft_len_of_word(s, i, c);
			result[word] = ft_substr(s, i, len);
			word++;
			i = i + len;
		}
	}
	result[word] = NULL;
}

static int	ft_find_error_and_free(char **result, int count)
{
	int		found_error;
	int		i;

	found_error = 0;
	i = 0;
	while (i < count && !found_error)
	{
		if (result[i] == NULL)
			found_error = 1;
		i++;
	}
	if (found_error)
	{
		i = 0;
		while (i < count)
		{
			if (result[i] != NULL)
				free(result[i]);
			i++;
		}
		free(result);
	}
	return (found_error);
}

char	**ft_split(char const *s, char c)
{
	int		count;
	char	**result;

	count = ft_split_count_words(s, c);
	result = (char **)malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	ft_split_extract(s, c, result);
	if (ft_find_error_and_free(result, count))
		return (NULL);
	return (result);
}
