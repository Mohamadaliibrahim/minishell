#include "../../inc/minishell.h"

void	ft_trunck(t_token *head, t_token	*token)
{
	int	fd;

	fd = open (token->next->tokens, O_WRONLY | O_CREAT | O_TRUNC | 0777);
	if (fd == -1)
		return ;
	if (dup2(fd, STDOUT_FILENO) == -1)
		return ;
	execute_comand(token)
}

void	ft_append(t_token *head, t_token *token)
{
	int	fd;

	fd = open(token->next->tokens, O_WRONLY | O_CREAT | O_APPEND | 0777);
	if (fd == -1)
		return ;
	if (dup2(fd, STDOUT_FILENO) == -1)
		return ;
	execute_comand(token);
}

void	ft_redirection(t_token *token)
{
	t_token	*head;

	head = token;
	if (ft_strcmp(token->tokens, ">") == 0)
		ft_trunck(head, token);
	else if (ft_strcmp(token->tokens, ">>") == 0)
		ft_append(head, token);
	// else if (ft_strcmp(token->tokens, "<") == 0)
}