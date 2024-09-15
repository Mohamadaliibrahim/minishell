/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/15 19:40:31 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**copy_envp(char **envp)
{
	int		i;
	char	**env_copy;

	i = 0;
	while (envp[i])
		i++;
	env_copy = malloc(sizeof(char *) * (i + 1));
	if (!env_copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env_copy[i] = ft_strdup(envp[i]);
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}

void	init_shell(t_shell *shell, char **envp)
{
	shell->env = copy_envp(envp);
	shell->last_exit_status = 0;
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_shell		shell;
	t_env_cpy	*env_cpy;

	(void)ac;
	(void)av;
	env_cpy = cpy_env(env);
	init_shell(&shell, env);
	while (1)
	{
		input = readline("\033[0;36m$>\033[0m ");
		if (input == NULL)
			break ;
		if (*input)
			add_history(input);
		check(input, env, env_cpy);
		free(input);
	}
	free_env_list(env_cpy);
	free_shell(&shell);
	clear_history();
	rl_clear_history();
	rl_free_line_state();
	return (0);
}
	 