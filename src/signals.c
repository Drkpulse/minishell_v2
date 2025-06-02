/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pda-silv <pda-silv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:37:51 by pda-silv          #+#    #+#             */
/*   Updated: 2025/06/02 22:15:09 by pda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Handles SIGINT and SIGQUIT at the prompt.
*/
static void	ft_prompt_signal(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
	{
		if (rl_line_buffer && rl_line_buffer[0] != '\0')
		{
			write(1, "\nQuit (core dumped)\n", 20);
			exit(131);
		}
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/*
** Signal handler for prompt state.
*/
static void	ft_prompt_handler(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	ft_prompt_signal(signum);
}

/*
** Signal handler for execution state.
*/
static void	ft_exec_handler(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (signum == SIGQUIT)
		write(1, "\nQuit (core dumped)\n", 20);
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
	sigaction(SIGQUIT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGPIPE, &sa, NULL);
}

/*
** Sets signal handlers for execution state.
*/
void	ft_set_exec_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = &ft_exec_handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGPIPE, &sa, NULL);
}
