/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/14 11:33:12 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	do_comand(t_token *token, char **env)
{
	int		pid;
	int		status;
	char	*av[2];
	char	*cmd_path;

	(void)cmd_path;
	av[0] = token->tokens;
	av[1] = NULL;
	if (ft_strncmp(token->tokens, "/", 1) == 0
		|| ft_strncmp(token->tokens, "./", 2) == 0
		|| ft_strncmp(token->tokens, "../", 3) == 0)
		cmd_path = ft_strdup(token->tokens);
	else
		cmd_path = find_in_path(token->tokens);
	if (!cmd_path)
	{
		printf("Error");
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd_path, av, env) == -1)
			perror("Error");
		exit(EXIT_FAILURE);
	}
	else if (pid)
		waitpid(pid, &status, 0);
	free(cmd_path);
}

int	check_for_quotations(char *input)
{
	int	i;
	int	single_quote_open;
	int	double_quote_open;

	i = 0;
	single_quote_open = 0;
	double_quote_open = 0;
	while (input[i])
	{
		if (input[i] == '"' && !single_quote_open)
			double_quote_open = !double_quote_open;
		else if (input[i] == '\'' && !double_quote_open)
			single_quote_open = !single_quote_open;
		i++;
	}
	if (single_quote_open || double_quote_open)
		return (0);
	return (1);

}

void	check_cmnd(char *input, t_token *token, char **env)
{
	(void)input;
	if ((ft_strcmp(token->tokens, "ls") == 0)
		|| (ft_strcmp(token->tokens, "clear") == 0))
		do_comand(token, env);
	else if ((ft_strcmp(token->tokens, "echo") == 0))
		check_echo(token);
	else if ((ft_strcmp(token->tokens, "pwd") == 0))
		ft_pwd(token);
}


void	check(char *input, char **env)
{
	t_token	*token;

	token = NULL;
	(void)env;
	tokenize_input(input, &token);
	if (!check_for_quotations(input))
	{
		printf("Syntax error\n");
		free(token);
	}
	else
	{
		// while(token)
		// {
		// 	printf("%s\n", token->tokens);
		// 	token = token->next;
		// }
		if (token)
		{
			if (token->token_type == CMND)
				check_cmnd(input, token, env);
			else if ((ft_strcmp(token->tokens, "cd") == 0))
				ft_cd(token);
			else if (token->token_type == UNKNOWN)
				printf("%s : Command not found\n", token->tokens);
		}
		free_token_list(token);
	}
}

int	main(int ac, char **av, char **env)
{
	char	*input;

	(void)ac;
	(void)av;
	while (1)
	{
		input = readline("Minishell> ");
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
