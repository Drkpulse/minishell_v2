/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:39:13 by pda-silv          #+#    #+#             */
/*   Updated: 2025/06/10 02:02:05 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Input first spa treatment                   */
/* Closes the input if it ends with newline    */
/* If there's no input just shows newline      */
/* Adds input to command history               */
/* It now needs to parse and exec the input    */

void	ft_process_input(t_data *data)
{
	if (data->input[0] != '\0')
		add_history(data->input);
	if (!ft_is_quotes_balanced(data->input))
	{
		printf(C_RED"Invalid Input - Unclosed Quotes\n"RESET_ALL);
		data->status = 1;
		if (data->input)
			ft_free((void **)&data->input);
		return ;
	}
	if (!ft_replace_tabs(data->input))
	{
		ft_free((void **) &(data->input));
		return ;
	}
	ft_bzero(data->commands, MAX_PIPE_COUNT * sizeof(t_command));
	data->cmd_count = 0;
	if (!ft_tokenize_input(data, data->input, 0))
		ft_execute(data);
	ft_free((void **) &(data->input));
}

static void	ft_iohandler(t_data *data)
{
	char	prompt[MAX_CWD_SIZE + 20];

	ft_set_prompt_signals();
	if (!getcwd(data->cwd, sizeof(data->cwd)))
	{
		perror("getcwd");
		ft_shutdown(&data, ERR_IO);
	}
	ft_strlcpy(prompt, "", sizeof(prompt));
	ft_strlcat(prompt, C_BLUE, sizeof(prompt));
	ft_strlcat(prompt, data->cwd, sizeof(prompt));
	ft_strlcat(prompt, " > "RESET_COLOR, sizeof(prompt));
	data->input = readline(prompt);
	if (g_sigint_received)
	{
		data->status = g_sigint_received;
		g_sigint_received = 0;
	}
	if (!data->input)
		ft_shutdown(&data, OK);
	ft_process_input(data);
}

/* Adds a tab character when TAB is pressed instead of autocompletion */
static int	ft_tab_handler(int count, int key)
{
	(void)count;
	if (key == '\t')
	{
		rl_insert_text("\t");
		return (0);
	}
	return (1);
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

int	main(int argc, char **argv, char **env)
{
	t_data	*data;

	(void)argc;
	(void)argv;
	if (ft_initilaize(&data, env))
		ft_shutdown(&data, NOK);
	if (ft_disable_echoctl() == -1)
	{
		write(2, "minishell: failed to set terminal attributes\n", 44);
		ft_shutdown(&data, NOK);
	}
	rl_bind_key('\t', &ft_tab_handler);
	while (true)
		ft_iohandler(data);
	ft_shutdown(&data, OK);
}
