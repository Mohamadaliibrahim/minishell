#include "../inc/minishell.h"

char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;

	// Find the end of the destination string
	while (dest[i] != '\0')
		i++;

	// Append the source string to the destination string
	while (src[j] != '\0')
	{
		dest[i] = src[j];
		i++;
		j++;
	}

	// Null-terminate the concatenated string
	dest[i] = '\0';

	return (dest);
}

