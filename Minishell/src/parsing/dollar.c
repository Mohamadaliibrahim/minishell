/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:24:44 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/18 12:40:21 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Append normal characters to the result
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

// Retrieve environment value
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

// Expand environment variable
char	*expand_variable(char *token, int *i, t_env_cpy *env_list, char *result)
{
	char	*var_name;
	int		var_len;
	char	*env_value;
	char	*temp;

	var_name = token + (*i) + 1;
	var_len = 0;
	while (ft_isalnum(var_name[var_len]) || var_name[var_len] == '_')
		var_len++;
	if (ft_strncmp(var_name, "UID", var_len) == 0 && var_len == 3)
	{
		result = ft_strjoin(result, "1000");
		(*i) += var_len + 1;
		return (result);
	}
	env_value = get_env_value(ft_substr(var_name, 0, var_len), env_list);
	if (!env_value)
		env_value = "";
	temp = ft_strjoin(result, env_value);
	free(result);
	*i += var_len + 1;
	return (temp);
}

// Handle $$ expansion (PID)
char	*handle_double_dollar(char *result)
{
	char	*pid_str;
	char	*temp;

	pid_str = ft_itoa(getpid());
	temp = ft_strjoin(result, pid_str);
	free(result);
	free(pid_str);
	return (temp);
}

// Handle $"VARIABLE" expansion
char	*handle_double_quote(char *token, char *result, int *i)
{
	(*i) += 2;
	while (token[*i] && token[*i] != '"')
	{
		result = append_char(result, token[*i]);
		(*i)++;
	}
	if (token[*i] == '"')
		(*i)++;
	return (result);
}

// Main function to expand tokens
char	*expand_token_if_variable(char *token, t_env_cpy *env_list)
{
	char	*result;
	int		i;

	i = 0;
	result = ft_strdup("");
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1] == '\0')
			printf("$");
		if (token[i] == '"' && token[i + 1] == '$')
			printf("$");
		if (token[i] == '$' && token[i + 1] == '$')
			result = handle_double_dollar(result), i += 2;
		else if (token[i] == '$' && token[i + 1] == '"')
			result = handle_double_quote(token, result, &i);
		else if (token[i] == '$')
			result = expand_variable(token, &i, env_list, result);
		else
			result = append_char(result, token[i]), i++;
	}
	return (result);
}
