/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:08:51 by mohamibr          #+#    #+#             */
/*   Updated: 2024/10/21 09:09:53 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	checking(t_cd cd, t_env_cpy *env_cpy)
{
	if (!cd.path)
	{
		freeing_cd(cd, env_cpy, env_cpy->last_exit_status);
		return (1);
	}
	if (!cd.old_pwd)
		old_pwd_is_null(cd, env_cpy);
	if (chdir(cd.path) == -1)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n", cd.path, strerror(errno));
		freeing_cd(cd, env_cpy, 1);
		return (1);
	}
	return (0);
}

void	pwd_or_oldpwd_got_unseted(t_cd cd, t_env_cpy *env_cpy)
{
	if (env_cpy->internal_oldpwd)
		free(env_cpy->internal_oldpwd);
	if (cd.pwd_env)
		env_cpy->internal_oldpwd = ft_strdup(cd.pwd_env);
	else
	{
		env_cpy->internal_oldpwd = ft_strdup(cd.old_pwd);
		remove_env("OLDPWD", &env_cpy);
		if (env_cpy->flag == 0)
		{
			fill_token(env_cpy, "OLDPWD");
			env_cpy->flag++;
		}
		else if (env_cpy->flag > 0)
		{
			cd.hello = ft_strjoin("OLDPWD=", env_cpy->internal_oldpwd);
			fill_token(env_cpy, cd.hello);
			free(cd.hello);
		}
	}
}

void	ft_cd_helper2(t_cd cd, t_env_cpy *env_cpy)
{
	too_long_error_cd();
	env_cpy = add_env_pwd(env_cpy);
	if (cd.pwd_env && cd.oldpwd_env)
		update_env_var(env_cpy, "OLDPWD", cd.pwd_env);
	else
	{
		if (env_cpy->internal_oldpwd)
			free(env_cpy->internal_oldpwd);
		env_cpy->internal_oldpwd = cd.pwd_env;
	}
}

void	ft_cd_helper1(t_cd cd, t_env_cpy *env_cpy, t_token *token)
{
	if (cd.pwd_env)
		update_env_var(env_cpy, "PWD", cd.new_pwd);
	else
	{
		if (env_cpy->internal_pwd)
			free(env_cpy->internal_pwd);
		env_cpy->internal_pwd = ft_strdup(cd.new_pwd);
	}
	if (cd.pwd_env && cd.oldpwd_env)
		update_env_var(env_cpy, "OLDPWD", cd.pwd_env);
	else
		pwd_or_oldpwd_got_unseted(cd, env_cpy);
	free(cd.new_pwd);
	if (token->next && ft_strcmp(token->next->tokens, "-") == 0)
		ft_pwd(token, env_cpy);
}

// hyde el function bt3ml she8el el cd
void	ft_cd(t_token *token, t_env_cpy *env_cpy)
{
	t_cd	cd;

	update_env(env_cpy);
	cd = init_cd(token, env_cpy);
	if (token->next && token->next->next)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		freeing_cd(cd, env_cpy, 1);
		return ;
	}
	if (checking(cd, env_cpy))
		return ;
	cd.new_pwd = getcwd(NULL, 0);
	if (cd.new_pwd)
		ft_cd_helper1(cd, env_cpy, token);
	else
		ft_cd_helper2(cd, env_cpy);
	free_at_the_end(cd);
}
// stop!!
