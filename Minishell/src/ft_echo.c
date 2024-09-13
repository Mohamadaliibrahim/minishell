/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:08:35 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/13 23:08:06 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	skip_echo(char **input)
{
	while (**input && (**input == ' ' || **input == '\t'))
		(*input)++;
	if (ft_strncmp(*input, "echo", 4) == 0 && ((*input)[4] == ' '
		|| (*input)[4] == '\0'))
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

char	*remove_qoutes(char *str)
{
	int		i;
	int		j;
	int		x;
	char	*dest;

	x = ft_strlen(str);
	dest = malloc(sizeof(char) * (x + 1));
	if (!dest)
		return (NULL);
	j = 0;
	i = 0;
	while (i < x)
	{
		if (str[i] != '\'' && str[i] != '"')
			dest[j++] = str[i];
		i++;
	}
	dest[j] = '\0';
	return (dest);
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

bool	check_n_helper(bool n, t_token **token)
{
	n = true;
	*token = (*token)->next;
	return (n);
}

bool	check_n(t_token **token)
{
	bool	n;
	int		i;

	n = false;
	while (*token)
	{
		if ((*token)->tokens[0] == '-' && (*token)->tokens[1] == 'n'
			&& (*token)->tokens[2] != '\0')
		{
			i = 2;
			while ((*token)->tokens[i] == 'n')
				i++;
			if ((*token)->tokens[i] != '\0')
				break ;
			n = true;
			*token = (*token)->next;
		}
		else if ((*token)->tokens[0] == '-' && (*token)->tokens[1] == 'n'
			&& (*token)->tokens[2] == '\0')
			n = check_n_helper(n, token);
		else
			break ;
	}
	return (n);
}

void	do_n(t_token *token)
{
	bool	n;
	int		f;

	f = 1;
	n = check_n(&token);
	while (token)
	{
		if (!f)
			printf(" ");
		printf("%s", token->tokens);
		f = 0;
		token = token->next;
	}
	if (!n)
		printf("\n");
}

void	check_echo(t_token *token)
{
	if (!token->next)
	{
		printf("\n");
		return ;
	}
	token = token->next;
	do_n(token);
}
