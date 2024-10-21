#include "../../inc/minishell.h"

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

void	remove_red_loop(t_token **current, t_token **prev, t_token **head)
{
	t_token	*to_delete;
	t_token	*next_token;

	to_delete = *current;
	next_token = (*current)->next;
	if (next_token == NULL)
		return ;
	if (*prev)
		(*prev)->next = next_token->next;
	else
		*head = next_token->next;
	free(to_delete->tokens);
	free(to_delete);
	if (next_token)
	{
		free(next_token->tokens);
		free(next_token);
	}
	if (*prev)
		*current = (*prev)->next;
	else
		*current = *head;
}

void	remove_redirection_tokens(t_token **head)
{
	t_token	*current;
	t_token	*prev;

	current = *head;
	prev = NULL;
	while (current)
	{
		if (current->token_type == REDIRECT_OUT || current->token_type == APPEND
			|| current->token_type == REDIRECT_IN)
			remove_red_loop(&current, &prev, head);
		else
		{
			prev = current;
			current = current->next;
		}
	}
}
//stop!