/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/22 15:12:22 by mohamibr         ###   ########.fr       */
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
	dest[1] = ft_strdup("SHLVL=1");
	dest[2] = ft_strdup("_=/usr/bin/env");
	dest[3] = NULL;
	free(pwd);
	return (cpy_env(dest));
}

void	fill_dest(char ***dest, int *i)
{
	(*dest)[*i++] = ft_strdup("SHLVL=1");
	(*dest)[*i++] = ft_strdup("_=/usr/bin/env");
	(*dest)[*i] = NULL;
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
	fill_dest(&dest, &i);
	return (dest);
}

void	free_dest(char ***dest, int *i)
{
	while ((*i) > 0)
		free((*dest)[--(*i)]);
	free((*dest));
}

int	fill_combine(char **combine, t_env_cpy *env, char ***dest, int *i)
{
	if (env->equal)
		(*combine) = ft_strjoin(env->type, "=");
	else
		(*combine) = ft_strjoin(env->type, "");
	if (!(*combine))
	{
		free_dest(dest, i);
		return (1);
	}
	return (0);
}

int	env_loop_to_2d(char **combine, t_env_cpy *env, char ***dest, int *i)
{
	char	*env_value;

	if (fill_combine(combine, env, dest, i))
		return (1);
	if (env->env)
		env_value = ft_strdup(env->env);
	else
		env_value = ft_strdup("");
	if (!env_value)
	{
		free((*combine));
		free_dest(dest, i);
		return (1);
	}
	(*dest)[(*i)] = ft_strjoin((*combine), env_value);
	free((*combine));
	free(env_value);
	if (!(*dest)[(*i)])
	{
		free_dest(dest, i);
		return (1);
	}
	(*i)++;
	return (0);
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
		if (env_loop_to_2d(&combine, env, &dest, &i))
			return (NULL);
		env = env->next;
	}
	dest[i] = NULL;
	return (dest);
}

void	creating_env(t_env_cpy **env_cpy, char **env)
{
	if (env[0] == NULL)
		(*env_cpy) = history();
	else
		(*env_cpy) = cpy_env(env);
	(*env_cpy) = add_shell((*env_cpy));
}

void	check_ac_av(int ac, char **av)
{
	(void)(av);
	if (ac > 1)
	{
		ft_putstr_fd("ERROR: too many arguments\n", 2);
		exit(127);
	}
}

void	freez(t_env_cpy **env_cpy)
{
	free_env_list(*env_cpy);
	clear_history();
	rl_clear_history();
	rl_free_line_state();
}

void	preparing(int ac, char **av, t_env_cpy **env_cpy, char **env)
{
	check_ac_av(ac, av);
	setup_signal_handlers();
	creating_env(&(*env_cpy), env);
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_env_cpy	*env_cpy;

	preparing(ac, av, &env_cpy, env);
	while (1)
	{
		input = readline("\033[0;31mHELL>\033[0m ");
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		if (g_last_signal != 0)
		{
			env_cpy->last_exit_status = 128 + g_last_signal;
			g_last_signal = 0;
		}
		check(input, env_cpy);
		free(input);
	}
	freez(&env_cpy);
	return (0);
}
