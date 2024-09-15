/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/15 10:44:41 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


char **copy_envp(char **envp)
{
    int i;
    char **env_copy;

    for (i = 0; envp[i]; i++)
        ;
    env_copy = malloc(sizeof(char *) * (i + 1));
    if (!env_copy)
        return (NULL);
    for (i = 0; envp[i]; i++)
        env_copy[i] = ft_strdup(envp[i]); // Use your own ft_strdup
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
	char	*input;
	t_shell	shell;

	(void)ac;
	(void)av;
	init_shell(&shell, env);
	printf(
		"\033[36m"  // Start of turquoise color
		" __    __     __     __   __     __     ______     __  __     ______     __         __        \n"
		"/\\ \"-./  \\   /\\ \\   /\\ \"-.\\ \\   /\\ \\   /\\  ___\\   /\\ \\_\\ \\   /\\  ___\\   /\\ \\       /\\ \\       \n"
		"\\ \\ \\-./\\ \\  \\ \\ \\  \\ \\ \\-.  \\  \\ \\ \\  \\ \\___  \\  \\ \\  __ \\  \\ \\  __\\   \\ \\ \\____  \\ \\ \\____  \n"
		" \\ \\_\\ \\ \\_\\  \\ \\_\\  \\  \\_\\\"\\_\\  \\ \\_\\  \\/\\_____\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\_____\\  \\ \\_____\\ \n"
		"  \\/_/  \\/_/   \\/_/   \\/_/ \\/_/   \\/_/   \\/_____/   \\/_/\\/_/   \\/_____/   \\/_____/   \\/_____/ \n"
		"\033[0m\n"  // Reset to default color
	);
	while (1)
	{
		input = readline("\033[0;36m$>\033[0m ");
		if (input == NULL)
			break ;
		if (*input)
			add_history(input);
		check(input, env);
		free(input);
	}
	clear_history();
	rl_clear_history();
	rl_free_line_state();
	return (0);
}
