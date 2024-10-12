#include "../../inc/minishell.h"

t_env_cpy	*find_env_node(t_env_cpy *head, char *type)
{
	t_env_cpy	*tmp;

	tmp = head;
	while (tmp)
	{
		if (ft_strcmp(tmp->type, type) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int	find_dollar(char *env)
{
	if (env[0] == '$')
		return (1);
	return (0);
}

char	*remove_dollar(char *env)
{
	char	*dest;
	int		i;
	int		j;

	if (env[0] != '$')
		return (ft_strdup(env));
	dest = malloc(sizeof(char) * (ft_strlen(env)));
	if (!dest)
		return (NULL);
	i = 1;
	j = 0;
	while (env[i])
	{
		dest[j] = env[i];
		i++;
		j++;
	}
	dest[j] = '\0';
	return (dest);
}

char	*test_dollar(char *env, t_env_cpy *head)
{
	t_env_cpy	*env_cpy;
	char		*key;

	if (!env)
		return (ft_strdup(""));

	env_cpy = head;
	key = remove_dollar(env);
	if (!key)
		return (ft_strdup(""));

	while (env_cpy)
	{
		if (ft_strcmp(env_cpy->type, key) == 0)
		{
			free(key);
			if (env_cpy->env)
				return (ft_strdup(env_cpy->env));
			else
				return (ft_strdup(""));
		}
		env_cpy = env_cpy->next;
	}
	free(key);
	return (ft_strdup(""));
}


void	update_env_node(t_env_cpy *node, t_env_cpy *head, char *env, bool equal)
{
	char	*new_env;

	if (equal)
	{
		free(node->env);
		if (env)
		{
			if (find_dollar(env))
			{
				new_env = test_dollar(env, head);
				if (!new_env)
					new_env = ft_strdup("");
				env = new_env;
			}
			node->env = ft_strdup(env);
			if (find_dollar(env))
				free(env);
		}
		else
			node->env = ft_strdup("");
		node->equal = true;
	}
}

/*
 */

t_env_cpy	*create_env_node(char *type, char *env, bool equal)
{
	t_env_cpy	*new_node;

	new_node = malloc(sizeof(t_env_cpy));
	if (!new_node)
		return (NULL);
	new_node->type = ft_strdup(type);
	if (!new_node->type)
	{
		free(new_node);
		return (NULL);
	}
	if (equal)
	{
		if (env)
			new_node->env = ft_strdup(env);
		else
			new_node->env = ft_strdup("");
		if (!new_node->env)
		{
			free(new_node->type);
			free(new_node);
			return (NULL);
		}
	}
	else
		new_node->env = NULL;
	new_node->equal = equal;
	new_node->next = NULL;
	return (new_node);
}


void	add_env_node(t_env_cpy **head, t_env_cpy *new_node)
{
	t_env_cpy	*tmp;

	if (!*head)
		*head = new_node;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

t_env_cpy	*a_env(t_env_cpy **head, char *type, char *env, bool equal)
{
	t_env_cpy	*node;
	char		*expanded_env = NULL;

	if (equal && env && find_dollar(env))
	{
		expanded_env = test_dollar(env, *head);
		if (!expanded_env)
			expanded_env = ft_strdup("");
	}

	node = find_env_node(*head, type);
	if (node)
	{
		update_env_node(node, *head, equal && expanded_env ? expanded_env : env, equal);
		free(type);
		if (expanded_env)
			free(expanded_env);
		return (node);
	}
	else
	{
		if (equal && expanded_env)
			env = expanded_env;
		node = create_env_node(type, env, equal);
		if (!node)
		{
			free(type);
			if (expanded_env)
				free(expanded_env);
			return (NULL);
		}
		add_env_node(head, node);
		free(type);
		if (expanded_env)
			free(expanded_env);
		return (node);
	}
}
