#include "../../inc/minishell.h"

/* Function to find an existing node with the given type */
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

void	update_env_node(t_env_cpy *node, char *env, bool equal)
{
	if (equal)
	{
		free(node->env);
		if (env)
			node->env = ft_strdup(env);
		else
			node->env = ft_strdup("");
		node->equal = true;
	}
}


/* Function to create a new node */
t_env_cpy	*create_env_node(char *type, char *env, bool equal)
{
	t_env_cpy	*new_node;

	new_node = malloc(sizeof(t_env_cpy));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->env = equal ? ft_strdup(env ? env : "") : NULL;
	new_node->equal = equal;
	new_node->next = NULL;
	return (new_node);
}

/* Function to add a node to the end of the list */
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

/* Main function to add or update environment variable */
t_env_cpy	*a_env(t_env_cpy **head, char *type, char *env, bool equal)
{
	t_env_cpy	*node;

	node = find_env_node(*head, type);
	if (node)
	{
		update_env_node(node, env, equal);
		free(type); // 'type' is no longer needed
		return (node);
	}
	else
	{
		node = create_env_node(type, env, equal);
		if (!node)
		{
			free(type);
			return (NULL);
		}
		add_env_node(head, node);
		return (node);
	}
}
