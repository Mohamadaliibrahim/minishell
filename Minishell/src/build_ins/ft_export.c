/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 15:13:03 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/29 00:53:11 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_export(t_env_cpy *env_cpy)
{
	printf("declare -x %s", env_cpy->type);
	if (env_cpy->equal == true)
		printf("=\"%s\"\n", env_cpy->env);
	else
		printf("\n");
	env_cpy = env_cpy->next;
}

t_env_cpy	*fill_token(t_env_cpy *env_cpy, char *str)
{
	char	*equal_sign_pos;
	char	*type;
	char	*env;
	bool	equal;

	env = NULL;
	equal = false;
	if (!str || !*str || str[0] == '{' || str[0] == '}')
		return (env_cpy);
	equal_sign_pos = ft_strchr(str, '=');
	if (equal_sign_pos)
	{
		type = ft_substr(str, 0, equal_sign_pos - str);
		env = equal_sign_pos + 1;
		equal = true;
	}
	else
		type = ft_strdup(str);
	if (!type)
		return (env_cpy);
	env_cpy = a_env(&env_cpy, type, env, equal);
	return (env_cpy);
}

int	check_ex(char *str)
{
	int		i;
	char	*before;

	if (!str || str[0] == '\0')
		return (0);
	if (!((str[0] >= 'A' && str[0] <= 'Z') || (str[0] >= 'a' && str[0] <= 'z')
			|| str[0] == '_'))
		return (0);
	before = return_type(str);
	if (!before)
		return (0);
	i = 0;
	while (before[++i])
	{
		if (!((before[i] >= 'A' && before[i] <= 'Z') || (before[i] >= 'a'
					&& before[i] <= 'z') || (before[i] >= '0'
					&& before[i] <= '9') || before[i] == '='
				|| before[i] == '_' || before[i] == '$'))
		{
			free(before);
			return (0);
		}
	}
	free(before);
	return (1);
}

void	ft_export(t_token *token, t_env_cpy *env_cpy)
{
	int	flag;

	flag = 0;
	if (!token->next)
	{
		env_cpy->last_exit_status = 0;
		print_sorted(env_cpy);
	}
	else
	{
		token = token->next;
		while (token)
		{
			if (token->tokens[0] != '{' && token->tokens[0] != '}')
			{
				if (check_ex(token->tokens))
					env_cpy = fill_token(env_cpy, token->tokens);
				else
				{
					flag = 1;
					printf("bash: export: `%s': not a valid identifier\n",
						token->tokens);
					env_cpy->last_exit_status = 127;
				}
			}
			token = token->next;
		}
	}
	if (flag == 0)
		env_cpy->last_exit_status = 0;
}
