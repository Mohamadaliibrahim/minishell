#include "../inc/minishell.h"

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("Minishell> ");
		if (input == NULL)
			break ;
		if (*input)
			add_history(input);
		printf("%s\n", input);
		free(input);
	}
	return (0);
}