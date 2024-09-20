/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/20 12:34:45 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_env_cpy	*history(void)
{
	char	**dest;
	char	*pwd;

	dest = malloc(sizeof(char *) * 4);
	if (!dest)
		return (NULL);
	pwd = getcwd(NULL, 0);
	dest[0] = ft_strjoin("PWD=", pwd);
	dest[1] = ft_strdup("SHELVL=1");
	dest[2] = ft_strdup("_=/usr/bin/env");
	dest[3] = NULL;
	free(pwd);
	return (cpy_env(dest));
}

char	**define_env(char **env)
{
	int		i;
	int		env_count;
	char	**dest;

	i = 0;
	env_count = 0;
	while (env[env_count])
		env_count++;
	dest = malloc((env_count + 3) * sizeof(char *));
	if (!dest)
		return (NULL);
	while (env[i])
	{
		dest[i] = ft_strdup(env[i]);
		if (!dest[i])
		{
			while (i-- > 0)
				free(dest[i]);
			free(dest);
			return (NULL);
		}
		i++;
	}
	dest[i++] = ft_strdup("SHLVL=1");
	dest[i++] = ft_strdup("_=/usr/bin/env");
	dest[i] = NULL;
	return (dest);
}

char	**list_to_2d(t_env_cpy *env)
{
	t_env_cpy	*head;
	char		**dest;
	char		*combine;
	int			i;

	i = 0;
	head = env;
	while (env)
	{
		i++;
		env = env->next;
	}
	dest = malloc(sizeof(char *) * (i + 1));
	if (!dest)
		return (NULL);
	i = 0;
	env = head;
	while (env)
	{
		if (env->equal)
			combine = ft_strjoin(env->type, "=");
		else
			combine = ft_strjoin(env->type, "");
		if (!combine)
		{
			while (i > 0)
				free(dest[--i]);
			free(dest);
			return (NULL);
		}
		dest[i] = ft_strjoin(combine, env->env);
		free(combine);
		if (!dest[i])
		{
			while (i > 0)
				free(dest[--i]);
			free(dest);
			return (NULL);
		}
		i++;
		env = env->next;
	}
	dest[i] = NULL;
	return (dest);
}


int	main(int ac, char **av, char **env)
{
	char		*input;
	t_env_cpy	*env_cpy;

	(void)ac;
	(void)av;
	if (env[0] == NULL)
		env_cpy = history();
	else
		env_cpy = cpy_env(env);
	while (1)
	{
		setup_signal_handlers();
		input = readline("\033[0;36m$>\033[0m ");
		if (input == NULL)
		{
			printf("exit\n");
			exit(0);
		}
		if (*input)
			add_history(input);
		check(input, env_cpy);
		free(input);
	}
	free_env_list(env_cpy);
	clear_history();
	rl_clear_history();
	rl_free_line_state();
	return (0);
}
