#include "../inc/minishell.h"

void	skip_echo(char **input)
{
	while (**input && (**input == ' ' || **input == '\t'))
		(*input)++;
	if (ft_strncmp(*input, "echo", 4) == 0
		&& ((*input)[4] == ' ' || (*input)[4] == '\0'))
	{
		*input += 4;
		while (**input == ' ')
			(*input)++;
	}
	else
	{
		fprintf(stderr, "Error: expected 'echo' command.\n");
		*input = NULL;
	}
}

int	handle_echo_n(char **input)
{
	int	has_n_flag;
	int	first_flag_found;
	int	i;

	has_n_flag = 0;
	first_flag_found = 0;
	while (**input == '-' && (*input)[1] == 'n')
	{
		i = 1;
		while ((*input)[i] == 'n')
			i++;
		if ((*input)[i] == ' ' || (*input)[i] == '\0')
		{
			if (!first_flag_found)
			{
				has_n_flag = 1;
				first_flag_found = 1;
				*input += i;
				while (**input == ' ')
					(*input)++;
			}
			else
				break ;
		}
		else
			break ;
	}
	return (has_n_flag);
}

char	*extract_quoted_token(char **input, char quote_type)
{
	char	*end;
	char	*start;
	char	*token;

	end = ++(*input);
	start = end;
	while (*end && *end != quote_type)
		end++;
	if (*end == quote_type)
	{
		token = ft_strndup(start, end - start);
		*input = end + 1;
		return (token);
	}
	return (NULL);
}

void	check_echo(char *input, char *start, char *quoted_token)
{
	t_echo	*current;
	t_echo	*new_node;
	t_echo	*next;
	t_echo	*input_tokens;
	int		has_n_flag;
	int		space_exists;
	int		first_token_printed;

	current = NULL;
	new_node = NULL;
	next = NULL;
	input_tokens = NULL;
	skip_echo(&input);
	if (!input)
		return ;
	has_n_flag = handle_echo_n(&input);
	if (has_n_flag && *input == '\0')
		return ;
	if (!has_n_flag && *input == '\0')
	{
		printf("\n");
		return ;
	}
	space_exists = 0;
	first_token_printed = 0;
	while (*input)
	{
		space_exists = 0;
		while (*input == ' ')
		{
			space_exists = 1;
			input++;
		}
		if (*input == '"' || *input == '\'')
		{
			quoted_token = extract_quoted_token(&input, *input);
			if (quoted_token)
			{
				new_node = malloc(sizeof(t_echo));
				new_node->token = quoted_token;
				new_node->next = NULL;
				if (!input_tokens)
					input_tokens = new_node;
				else
					current->next = new_node;
				current = new_node;
				first_token_printed = 1;
			}
		}
		else if (*input)
		{
			start = input;
			while (*input && *input != ' ' && *input != '"' && *input != '\'')
				input++;
			new_node = malloc(sizeof(t_echo));
			new_node->token = ft_strndup(start, input - start);
			new_node->next = NULL;
			if (!input_tokens)
				input_tokens = new_node;
			else
				current->next = new_node;
			current = new_node;
			first_token_printed = 1;
		}
	}
	current = input_tokens;
	while (current)
	{
		printf("%s", current->token);
		current = current->next;
		if (current && space_exists && first_token_printed)
			printf(" ");
	}
	if (!has_n_flag)
		printf("\n");
	current = input_tokens;
	while (current)
	{
		next = current->next;
		free(current->token);
		free(current);
		current = next;
	}
}

