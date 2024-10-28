/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/10/27 10:36:53 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_env_cpy	*history(void)
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

static void	creating_env(t_env_cpy **env_cpy, char **env)
{
	if (env[0] == NULL)
		(*env_cpy) = history();
	else
		(*env_cpy) = cpy_env(env);
	(*env_cpy) = add_shell((*env_cpy));
}

static void	check_ac_av(int ac, char **av)
{
	(void)(av);
	if (ac > 1)
	{
		ft_putstr_fd("ERROR: too many arguments\n", 2);
		exit(127);
	}
}

void	preparing(int ac, char **av, t_env_cpy **env_cpy, char **env)
{
	check_ac_av(ac, av);
	setup_signal_handlers();
	creating_env(&(*env_cpy), env);
}
