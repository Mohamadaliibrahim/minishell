#include "../inc/minishell.h"

void	do_comand(t_token *token, char **env)
{
	int		pid;
	int		status;
	char	*av[2];

	av[0] = token->tokens;
	av[1] = NULL;
	pid = fork();
	if (pid == 0)
	{
		if (execve(find_in_path(token->tokens), av, env) == -1)
			perror("Error");
		exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, &status, 0);
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
		if (input[i] == '"')
			double_quote_open = !double_quote_open;
		else if (input[i] == '\'')
			single_quote_open = !single_quote_open;
		i++;
	}
	if (single_quote_open || double_quote_open)
		return (0);
	return (1);
}

void	check(char *input, char **env)
{
	t_token	*token;

	token = NULL;
	tokenize_input(input, &token);
	if (!check_for_quotations(input))
	{
		printf("Error\n");
		free(token);
	}
	else
	{
		while (token)
		{
			if (token->token_type == CMND)
				do_comand(token, env);
			else if (token->token_type == UNKNOWN)
			{
				printf("Error\n");
				break ;
			}
			token = token->next;
		}
		free(token);
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
