/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pda-silv <pda-silv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:37:51 by pda-silv          #+#    #+#             */
/*   Updated: 2025/06/02 20:18:07 by pda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_disable_echoctl(void)
{
	struct termios	term;

	if (tcgetattr(0, &term) == -1)
		return (-1);
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(0, TCSANOW, &term) == -1)
		return (-1);
	return (0);
}

/*
** Handler for SIGINT at prompt: clears the current input line.
*/
static void	ft_sigint_prompt(int signum, siginfo_t *info, void *context)
{
    (void)info;
    (void)context;
    if (signum == SIGINT)
    {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

/*
** Handler for SIGQUIT at prompt: prints message and exits.
*/
static void	ft_sigquit_prompt(int signum, siginfo_t *info, void *context)
{
    (void)info;
    (void)context;
    if (signum == SIGQUIT)
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
** Handler for SIGINT during execution: does nothing (default behavior).
*/
static void	ft_sigint_exec(int signum, siginfo_t *info, void *context)
{
    (void)signum;
    (void)info;
    (void)context;
}

/*
** Handler for SIGQUIT during execution: clears line.
*/
static void	ft_sigquit_exec(int signum, siginfo_t *info, void *context)
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
    sa.sa_sigaction = &ft_sigint_prompt;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_sigaction = &ft_sigquit_prompt;
    sa.sa_flags = SA_SIGINFO;
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
    sa.sa_sigaction = &ft_sigint_exec;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_sigaction = &ft_sigquit_exec;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGQUIT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigaction(SIGPIPE, &sa, NULL);
}
