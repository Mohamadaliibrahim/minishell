/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mustafa-machlouch <mustafa-machlouch@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:24:57 by mustafa-mac       #+#    #+#             */
/*   Updated: 2024/09/19 12:52:51 by mustafa-mac      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n\033[0;36m$>\033[0m ", 15);
}

static void handle_sigquit(int sig)
{
    (void)sig;
    // You can ignore SIGQUIT entirely if you don't want it to do anything:
    signal(SIGQUIT, SIG_IGN);
}

void setup_signal_handlers(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);
}
