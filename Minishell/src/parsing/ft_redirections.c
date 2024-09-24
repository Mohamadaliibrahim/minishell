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

char	*get_filename(t_token *token)
{
	while (token)
	{
		if ((token->token_type == APPEND || token->token_type == REDIRECT_OUT
				|| token->token_type == REDIRECT_IN))
		{
			if (token->next == NULL)
				return (NULL);
			return (token->next->tokens);
		}
		token = token->next;
	}
	return (NULL);
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
			if (next_token == NULL)
				return ;
			if (prev)
				prev->next = next_token->next;
			else
				*head = next_token->next;
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
		perror("open");
		env->last_exit_status = 1;
		return ;
	}
	if (env->last_output_fd != -1)
		close(env->last_output_fd);
	env->last_output_fd = fd;
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
	if (env->last_output_fd != -1)
		close(env->last_output_fd);
	env->last_output_fd = fd;
}

void	ft_infile(t_token *token, t_env_cpy *env)
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
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		env->last_exit_status = 1;
		return ;
	}
	if (env->last_input_fd != -1)
		close(env->last_input_fd);
	env->last_input_fd = fd;
}

void	ft_redirection(t_token *token, t_env_cpy *env)
{
	t_token	*head;
	int		stdout_backup;
	int		stdin_backup;

	head = token;
	env->last_output_fd = -1;
	env->last_input_fd = -1;
	stdout_backup = dup(STDOUT_FILENO);
	if (stdout_backup == -1 && env->last_exit_status == 0)
	{
		perror("dup");
		env->last_exit_status = 1;
		return ;
	}
	stdin_backup = dup(STDIN_FILENO);
	if (stdin_backup == -1 && env->last_exit_status == 0)
	{
		perror("dup");
		env->last_exit_status = 1;
		return ;
	}
	while (token)
	{
		if (token->token_type == REDIRECT_OUT)
			ft_trunck(token, env);
		else if ((token->token_type == APPEND))
			ft_append(token, env);
		else if (token->token_type == REDIRECT_IN)
			ft_infile(token, env);
		token = token->next;
	}
	remove_redirection_tokens(&head);
	if (env->last_exit_status != 0)
	{
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
		return ;
	}
	if (env->last_input_fd != -1)
	{
		if ((dup2(env->last_input_fd, STDIN_FILENO) == -1)
			&& env->last_exit_status == 0)
		{
			perror("dup2");
			close(stdout_backup);
			close(stdin_backup);
			env->last_exit_status = 1;
			return ;
		}
		close(env->last_input_fd);
	}
	if (env->last_output_fd != -1)
	{
		if ((dup2(env->last_output_fd, STDOUT_FILENO) == -1)
			&& env->last_exit_status == 0)
		{
			perror("dup2");
			close(stdout_backup);
			close(stdin_backup);
			env->last_exit_status = 1;
			return ;
		}
		close(env->last_output_fd);
	}
	if (env->last_exit_status == 0)
		ft_cmd(head, env);
	if (dup2(stdout_backup, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		env->last_exit_status = 1;
	}
	close(stdout_backup);
	if (dup2(stdin_backup, STDIN_FILENO) == -1)
	{
		perror("dup2");
		env->last_exit_status = 1;
	}
	close(stdin_backup);
}

void	check_redirections(t_token *token, t_env_cpy *env)
{
	ft_redirection(token, env);
}
