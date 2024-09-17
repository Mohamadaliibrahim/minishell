#include "../../inc/minishell.h"

t_env_cpy	*remove_it(t_env_cpy *env_cpy)
{
	t_env_cpy	*before;
	t_env_cpy	*after;

	before = env_cpy->previous;
	after = env_cpy->next;
	if (!before)
	{
		if (after)
			after->previous = NULL;
	}
	else
	{
		before->next = after;
		if (after)
			after->previous = before;
	}
	free_single_env_list(env_cpy);
	if (after)
		return (after);
	else
		return (before);
}

void	remove_env(char *type, t_env_cpy *env_cpy)
{
	t_env_cpy	*current;

	current = env_cpy;
	while (current)
	{
		if (current->type && ft_strcmp(current->type, type) == 0)
			current = remove_it(current);
		else
			current = current->next;
	}
}

void	ft_unset(t_token *token, t_env_cpy *env_cpy)
{
	char	*type;

	if (!token->next)
		return ;
	token = token->next;
	while (token)
	{
		type = return_type(token->tokens);
		if (type)
			remove_env(type, env_cpy);
		token = token->next;
	}
}
