/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pwd_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 07:45:50 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/18 19:08:13 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	freeing_cd(t_cd cd, t_env_cpy *env_cpy, int x)
{
	if (cd.pwd_env)
		free(cd.pwd_env);
	if (cd.oldpwd_env)
		free(cd.oldpwd_env);
	if (cd.hello)
		free(cd.hello);
	if (cd.should_free && cd.path)
		free(cd.path);
	if (cd.old_pwd)
		free(cd.old_pwd);
	if (cd.new_pwd)
		free(cd.new_pwd);
	env_cpy->last_exit_status = x;
}

void	too_long_error_cd(void)
{
	ft_putstr_fd("cd: error retrieving "
		"current directory: getcwd: cannot access"
		" parent directories: No such file or directory\n",
		2);
}

void	free_at_the_end(t_cd cd)
{
	free(cd.old_pwd);
	if (cd.should_free)
		free(cd.path);
	free(cd.pwd_env);
	free(cd.oldpwd_env);
}

void	error_mes_after_checking(t_token *token, t_env_cpy *env)
{
	env->last_exit_status = 2;
	write(2, "Minishell: pwd: ", 16);
	write(2, token->next->tokens, ft_strlen(token->next->tokens));
	write(2, ": invalid option\n", 17);
}
