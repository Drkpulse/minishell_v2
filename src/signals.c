/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:37:51 by pda-silv          #+#    #+#             */
/*   Updated: 2025/06/08 19:06:22 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sigint_received;

/*
** Handles SIGINT and SIGQUIT at the prompt.
*/
static void ft_prompt_signal(int signum)
{
	if (signum == SIGINT)
	{
		g_sigint_received = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		data->status = 130;
	}
}

/*
** Signal handler for prompt state.
*/
static void	ft_prompt_handler(int signum, siginfo_t *info, void *context, t_data *data)
{
	(void)info;
	(void)context;
	ft_prompt_signal(signum, data);
}

/*
** Sets signal handlers for prompt state.
*/
void	ft_set_prompt_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = &ft_prompt_handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGPIPE, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*   Sets signal handlers for child processes                                */
/*   Restores default behavior for signals in child processes                */
/*   Ensures signal handlers work properly for command execution             */
/* ************************************************************************** */
void	ft_set_child_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGPIPE, &sa, NULL);
}
