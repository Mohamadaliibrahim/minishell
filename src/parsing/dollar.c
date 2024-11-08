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

char	*handle_general_variable_expansion(t_var_expansion *expansion,
		t_env_cpy *env_list)
{
	char	*substr;
	char	*env_value;
	char	*temp;

	substr = ft_substr(expansion->var_name, 0, expansion->var_len);
	env_value = get_env_value(substr, env_list);
	free(substr);
	if (!env_value)
		env_value = ft_strdup("");
	else
		env_value = ft_strdup(env_value);
	temp = ft_strjoin(expansion->result, env_value);
	free(expansion->result);
	free(env_value);
	*(expansion->i) += expansion->var_len + 1;
	return (temp);
}

static char	*handle_single_quoted_variable(char *token, int *i, char *result)
{
	(*i)++;
	while (token[*i])
	{
		if (token[*i] != '\'')
			result = append_char(result, token[*i]);
		(*i)++;
	}
	return (result);
}

static	char	*handle_double_quoted_variable(char *token,
		int *i, char *result)
{
	(*i)++;
	while (token[*i] && token[*i] != '"')
	{
		result = append_char(result, token[*i]);
		(*i)++;
	}
	if (token[*i] == '"')
		(*i)++;
	return (result);
}

char	*expand_variable(char *token, int *i, t_env_cpy *env_list, char *result)
{
	t_var_expansion	expansion;
	char			*var_name;
	int				var_len;

	var_name = token + (*i) + 1;
	if (var_name[0] == '\'')
		return (handle_single_quoted_variable(token, i, result));
	if (var_name[0] == '"')
		return (handle_double_quoted_variable(token, i, result));
	if (ft_strncmp(var_name, "UID", 3) == 0 || var_name[0] == '?')
		return (handle_special_expansion(result, i, env_list,
				(var_name[0] == 'U')));
	if (!ft_isalnum(var_name[0]) && var_name[0] != '_')
		return (handle_invalid_variable_expansion(result, var_name, i));
	if (ft_isdigit(var_name[0]))
		return (handle_digit_variable_expansion(result, var_name, i));
	var_len = 0;
	while (ft_isalnum(var_name[var_len]) || var_name[var_len] == '_')
		var_len++;
	expansion.result = result;
	expansion.var_name = var_name;
	expansion.var_len = var_len;
	expansion.i = i;
	return (handle_general_variable_expansion(&expansion, env_list));
}

char	*expand_token_if_variable(char *token, t_env_cpy *env_list)
{
	char	*result;
	int		i;

	i = 0;
	result = ft_strdup("");
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1] == '\\')
		{
			result = append_char(result, '$');
			i += 2;
		}
		else if (token[i] == '$')
		{
			result = expand_variable(token, &i, env_list, result);
		}
		else
		{
			result = append_char(result, token[i]);
			i++;
		}
	}
	return (result);
}
