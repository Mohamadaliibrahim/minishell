#include "../inc/minishell.h"

static void	skip_echo(char **input)
{
	while (**input && (**input == ' ' || **input == '\t'))
		(*input)++;
	if (ft_strncmp(*input, "echo ", 5) == 0)
		*input += 4;
}

static char	*extract_quoted_token(char **input, char quote_type)
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

void check_echo(char *input, char *start, char	*quoted_token)
{
	t_echo	*current;
	t_echo	*new_node;
	t_echo	*next;
	t_echo	*input_tokens;

	current = NULL;
	input_tokens = NULL;
	skip_echo(&input);
	while (*input)
	{
		while (*input == ' ')
			input++;

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
			new_node->prev = current;

			if (!input_tokens)
				input_tokens = new_node;
			else
				current->next = new_node;
			current = new_node;
		}
	}
	current = input_tokens;
	if ((ft_strcmp(current->token, "echo") == 0) && !(current->next))
		printf("\n");
	else
	{
		while (current)
		{
			printf("%s ", current->token);
			current = current->next;
		}
		printf("\n");
	}
	current = input_tokens;
	while (current)
	{
		next = current->next;
		free(current->token);
		free(current);
		current = next;
	}
}
