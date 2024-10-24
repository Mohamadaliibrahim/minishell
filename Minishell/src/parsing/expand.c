/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:24:44 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/20 12:45:10 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*append_char(char *result, char c)
{
	char	temp_str[2];
	char	*temp;

	temp_str[0] = c;
	temp_str[1] = '\0';
	temp = ft_strjoin(result, temp_str);
	free(result);
	return (temp);
}

char	*get_env_value(char *var_name, t_env_cpy *env_list)
{
	t_env_cpy	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->type, var_name) == 0)
			return (current->env);
		current = current->next;
	}
	return (NULL);
}

char	*handle_special_expansion(char *result, int *i,
	t_env_cpy *env_list, int is_uid)
{
	char	*value_str;
	char	*temp;

	if (is_uid)
		value_str = ft_itoa(getuid());
	else
		value_str = ft_itoa(env_list->last_exit_status);
	temp = ft_strjoin(result, value_str);
	free(value_str);
	free(result);
	if (is_uid)
		*i += 4;
	else
		*i += 2;
	return (temp);
}

char	*handle_invalid_variable_expansion(char *result, char *var_name, int *i)
{
	char	*temp;

	temp = ft_strjoin(result, "$");
	free(result);
	if (var_name[0] != '\0')
	{
		result = ft_strjoin(temp, (char []){var_name[0], '\0'});
		free(temp);
		*i += 2;
	}
	else
	{
		result = temp;
		*i += 1;
	}
	return (result);
}

char	*handle_digit_variable_expansion(char *result, char *var_name, int *i)
{
	char	*temp;
	char	*substr;
	int		var_len;

	var_name += 1;
	*i += 2;
	var_len = 0;
	while (var_name[var_len] && (ft_isalnum(var_name[var_len])
			|| var_name[var_len] == '_'))
		var_len++;
	substr = ft_substr(var_name, 0, var_len);
	if (substr)
	{
		temp = ft_strjoin(result, substr);
		free(substr);
		free(result);
		result = temp;
	}
	*i += var_len;
	return (result);
}
