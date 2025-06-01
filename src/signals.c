/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pda-silv <pda-silv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:37:51 by pda-silv          #+#    #+#             */
/*   Updated: 2025/06/01 22:01:31 by pda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_sighandler(int signum, siginfo_t *info, void *context)
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
    if (signum == SIGQUIT)
    {
        if (rl_line_buffer && rl_line_buffer[0] != '\0')
        {
            write(1, "Quit (core dumped)\n", 19);
            exit(131);
        }
        rl_on_new_line();
        rl_redisplay();
    }
}

void	ft_setup_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);
	sigaddset(&sa.sa_mask, SIGQUIT);
	sigaddset(&sa.sa_mask, SIGPIPE);
	sa.sa_sigaction = &ft_sighandler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGPIPE, &sa, NULL);
}

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
