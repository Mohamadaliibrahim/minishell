/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tokenizeing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:03:44 by mohamibr          #+#    #+#             */
/*   Updated: 2024/09/20 12:08:03 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*return_path(char *env)
{
	int		i;
	int		start;
	char	*dest;

	i = 0;
	while (env[i] != '=')
		i++;
	dest = malloc(sizeof(char) * (ft_strlen(env) - i + 1));
	if (!dest)
		return (NULL);
	i++;
	start = i;
	i = 0;
	while (env[start])
	{
		dest[i] = env[start];
		i++;
		start++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*return_type(char *env)
{
	int		i;
	char	*dest;

	i = 0;
	while (env[i] != '\0' && env[i] != '=')
		i++;
	dest = malloc(sizeof(char) * (i + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (env[i] != '\0' && env[i] != '=')
	{
		dest[i] = env[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}


// char	*return_type(char *env)
// {
// 	int		i;
// 	char	*dest;

// 	i = 0;
// 	while (env[i] != '=')
// 		i++;
// 	dest = malloc(sizeof(char) * (i + 1));
// 	i = 0;
// 	while (env[i] != '=')
// 	{
// 		dest[i] = env[i];
// 		i++;
// 	}
// 	dest[i] = '\0';
// 	return (dest);
// }

bool	check_for_equal(char *env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (env[i] == '=')
			return (true);
		i++;
	}
	return (false);
}

t_env_cpy	*add_shell(t_env_cpy *env_cpy)
{
	int			x;

	while (env_cpy)
	{
		if (ft_strcmp(env_cpy->type, "SHLVL") == 0)
		{
			x = ft_atoi(env_cpy->env);
			if (x <= 0)
				x = 1;
			else
				x++;
			if (x > INT_MAX)
				x = INT_MAX;
			free(env_cpy->env);
			env_cpy->env = ft_itoa((int)x);
			if (!env_cpy->env)
			{
				perror("MEmory allocation error");
				exit(EXIT_FAILURE);
			}
			return (env_cpy);
		}
		env_cpy = env_cpy->next;
	}
	return (env_cpy);
}

t_env_cpy	*cpy_env_helper(char *env)
{
	t_env_cpy	*cpy;

	cpy = malloc(sizeof(t_env_cpy));
	if (!cpy)
		return (NULL);
	cpy->env = return_path(env);
	cpy->type = return_type(env);
	cpy->equal = check_for_equal(env);
	cpy->next = NULL;
	cpy->previous = NULL;
	return (cpy);
}

t_env_cpy	*cpy_env(char **env)
{
	int			i;
	t_env_cpy	*head;
	t_env_cpy	*current;
	t_env_cpy	*new_node;

	i = -1;
	new_node = NULL;
	head = NULL;
	current = NULL;
	while (env[++i])
	{
		new_node = cpy_env_helper(env[i]);
		if (!new_node)
			return (NULL);
		if (!head)
		{
			head = new_node;
			current = head;
		}
		else
		{
			current->next = new_node;
			new_node->previous = current;
			current = new_node;
		}
	}
	return (head);
}


// t_env_cpy *add_shell(t_env_cpy *env_cpy)
// {
//     int     shlvl;
//     char    *new_shlvl;
//     t_env_cpy *head = env_cpy;

//     while (env_cpy)
//     {
//         if (ft_strcmp(env_cpy->type, "SHLVL") == 0)
//         {
//             // Convert SHLVL to integer
//             shlvl = ft_atoi(env_cpy->env);

//             // Handle cases where SHLVL is negative or too large
//             if (shlvl < 0)
//                 shlvl = 0;
//             else if (shlvl >= 999)
//                 shlvl = 999;
//             else
//                 shlvl++;

//             // Convert back to string
//             new_shlvl = ft_itoa(shlvl);
//             if (!new_shlvl)
//             {
//                 perror("Memory allocation error");
//                 exit(EXIT_FAILURE);
//             }

//             // Free the old SHLVL value and update it with the new one
//             free(env_cpy->env);
//             env_cpy->env = new_shlvl;
//             return head;
//         }
//         env_cpy = env_cpy->next;
//     }

//     // If SHLVL does not exist, create it and set to 1
//     a_env(&head, ft_strdup("SHLVL"), ft_strdup("1"), true);

//     return head;
// }
