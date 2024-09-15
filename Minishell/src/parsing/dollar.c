/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 10:14:18 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/15 11:40:26 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*get_special_var(char c, t_shell *shell)
{
	if (c == '?')
		return (ft_itoa(shell->last_exit_status));
	else if (c == '$')
		return (ft_itoa(getpid()));
	return (ft_strdup(""));
}

static char	*handle_dollar(char *str, int *i, t_shell *shell)
{
    char	*var_name;
    char	*var_value;
    int		j;

    (*i)++;
    if (str[*i] == '?' || str[*i] == '$')
    {
        var_value = get_special_var(str[*i], shell);
        (*i)++;
    }
    else if (!ft_isalnum(str[*i]) && str[*i] != '_')
    {
        if (str[*i] == 0 || ft_strchr(":= \t", str[*i]))
        {
            var_value = ft_strdup("$");
        }
        else
        {
            var_value = ft_strdup("");
        }
    }
    else
    {
        j = *i;
        while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
            j++;
        var_name = ft_substr(str, *i, j - *i);
        var_value = getenv(var_name);
        free(var_name);
        if (!var_value)
            var_value = ft_strdup("");
        else
            var_value = ft_strdup(var_value);
        *i = j;
    }
    return (var_value);
}



static char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined_str;
	size_t	len1;
	size_t	len2;

	if (!s1 && !s2)
		return (NULL);
	len1 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	len2 = 0;
	if (s2)
		len2 = ft_strlen(s2);
	joined_str = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!joined_str)
	{
		free(s1);
		return (NULL);
	}
	if (s1)
		strcpy(joined_str, s1);
	if (s2)
		strcpy(joined_str + len1, s2);
	joined_str[len1 + len2] = '\0';
	free(s1);
	return (joined_str);
}

static char	*ft_charjoin_free(char *s, char c)
{
	char	*new_str;
	size_t	len;

	if (!s)
		len = 0;
	else
		len = ft_strlen(s);
	new_str = (char *)malloc(sizeof(char) * (len + 2));
	if (!new_str)
	{
		free(s);
		return (NULL);
	}
	if (s)
		strcpy(new_str, s);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(s);
	return (new_str);
}

char	*expand_env_var(char *str, t_shell *shell)
{
	char	*expanded_str;
	char	*var_value;
	int		i;

	i = 0;
	expanded_str = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			var_value = handle_dollar(str, &i, shell);
			expanded_str = ft_strjoin_free(expanded_str, var_value);
			free(var_value);
		}
		else
		{
			expanded_str = ft_charjoin_free(expanded_str, str[i]);
			i++;
		}
	}
	return (expanded_str);
}
