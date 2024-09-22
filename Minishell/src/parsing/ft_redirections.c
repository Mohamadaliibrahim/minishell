#include "../../inc/minishell.h"

int	search_for_redirection(t_token *token1)
{
	t_token	*token;

	token = token1;
	while (token)
	{
		if (token->token_type == REDIRECT_OUT || token->token_type == APPEND
			|| token->token_type == REDIRECT_IN)
		{
			return (1);
		}
		token = token->next;
	}
	return (0);
}

void	remove_redirection_tokens(t_token **head)
{
	t_token	*current;
	t_token	*prev;
	t_token	*to_delete;
	t_token	*next_token;

	current = *head;
	prev = NULL;
	while (current)
	{
		if (current->token_type == REDIRECT_OUT || current->token_type == APPEND
			|| current->token_type == REDIRECT_IN)
		{
			to_delete = current;
			next_token = current->next;
			// Remove the redirection token
			if (next_token == NULL)
			{
				fprintf(stderr,
					"Syntax error: redirection operator without a filename\n");
				return ;
			}
			if (prev)
				prev->next = next_token->next;
			else
				*head = next_token->next;

			// Free the tokens
			free(to_delete->tokens);
			free(to_delete);
			if (next_token)
			{
				free(next_token->tokens);
				free(next_token);
			}
			current = (prev) ? prev->next : *head;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

char	**tokens_to_args(t_token *token)
{
	int		count;
	t_token	*current;
	char	**args;
	int		i;

	count = 0;
	current = token;
	i = -1;
	while (current)
	{
		if (current->token_type == REDIRECT_OUT || current->token_type == APPEND
			|| current->token_type == REDIRECT_IN)
			break ;
		count++;
		current = current->next;
	}
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	current = token;
	while (++i < count)
	{
		args[i] = strdup(current->tokens);
		current = current->next;
	}
	args[count] = NULL;
	return (args);
}

char	*get_filename(t_token *token)
{
	while (token)
	{
		if ((token->token_type == APPEND || token->token_type == REDIRECT_OUT
				|| token->token_type == REDIRECT_IN) && token->next)
		{
			if (token->next == NULL)
			{
				fprintf(stderr,
					"Syntax error:redirection operator without a filename\n");
				return (NULL);
			}
			return (token->next->tokens);
		}
		token = token->next;
	}
	return (NULL);
}

void	ft_trunck(t_token *token, t_env_cpy *env)
{
	int		fd;
	char	*file_name;

	file_name = get_filename(token);
	if (!file_name)
	{
		perror("filename");
		env->last_exit_status = 2;
		return ;
	}
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("file");
		env->last_exit_status = 1;
		return ;
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		env->last_exit_status = 1;
		return ;
	}
	close(fd);
	env->last_fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (env->last_fd == -1)
	{
		perror("open");
		env->last_exit_status = 1;
		return ;
	}
}

void	ft_append(t_token *token, t_env_cpy *env)
{
	int		fd;
	char	*file_name;

	file_name = get_filename(token);
	if (!file_name)
	{
		perror("filename");
		env->last_exit_status = 2;
		return ;
	}
	fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open");
		env->last_exit_status = 1;
		return ;
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		env->last_exit_status = 1;
		return ;
	}
	close(fd);
	env->last_fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (env->last_fd == -1)
	{
		perror("open");
		env->last_exit_status = 1;
		return ;
	}
}

void	ft_redirection(t_token *token, t_env_cpy *env)
{
	t_token	*head;
	int		stdout_backup;

	head = token;
	env->last_fd = -1;
	stdout_backup = dup(STDOUT_FILENO);
	if (stdout_backup == -1)
	{
		perror("dup");
		env->last_exit_status = 1;
		return ;
	}
	while (token)
	{
		if (token->tokens && token->token_type == REDIRECT_OUT)
			ft_trunck(token, env);
		else if ((token->tokens && token->token_type == APPEND))
			ft_append(token, env);
		// else if (ft_strcmp(token->tokens, "<") == 0)
		// 	ft_inputfile(token, env);
		token = token->next;
	}
	remove_redirection_tokens(&head);
	if (env->last_fd != -1)
	{
		if (dup2(env->last_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(stdout_backup);
			env->last_exit_status = 1;
			return ;
		}
		close(env->last_fd);
	}
	ft_cmd(head, env);
	if (dup2(stdout_backup, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		env->last_exit_status = 1;
	}
	close(stdout_backup);
}

void	check_redirections(t_token *token, t_env_cpy *env)
{
	ft_redirection(token, env);
}
