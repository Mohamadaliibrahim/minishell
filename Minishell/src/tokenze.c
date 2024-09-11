i/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenze.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:07:00 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/11 13:20:48 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token    *tokenze_input(char *input)
{
    t_token    *new_tokens;
    char        **tokens;
    int         i;

    i = 0;
    tokens = ft_split(input, ' ');
    while (tokens[i] != NULL)
    {
        new_tokens = malloc(sizeof(t_token));
        if (!new_tokens)
            return (NULL);
        new_tokens->tokens = strdup(tokens[i]);
        new_tokens->next = NULL;
    }
}