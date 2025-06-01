/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 14:27:48 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/01 21:34:14 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric_arg(char *arg, int *exit_value)
{
	int	i;
	int	neg;

	i = 0;
	neg = 0;
	if (arg[i] == '-' || arg[i] == '+')
	{
		if (arg[i] == '-')
			neg = 1;
		i++;
	}
	*exit_value = 0;
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (0);
		*exit_value = *exit_value * 10 + (arg[i] - '0');
		i++;
	}
	if (neg)
		*exit_value = -*exit_value;
	return (1);
}

static void	handle_exit_error(char *arg)
{
	write(2, "minishell: exit: ", 17);
	write(2, arg, ft_strlen(arg));
	write(2, ": numeric argument required\n", 28);
}

/*********************/
/* Exit Builtin Command */
/*********************/
void	ft_exit(t_data *data, char **cmd_args)
{
	int	exit_status;

	exit_status = 0;
	if (data->piped == 0)
		ft_printf("exit\n");
	if (!cmd_args[1])
	{
		ft_free_cmd(data, cmd_args);
		ft_cleanup_execution(data);
		ft_safe_close(&data->original_stdin);
		ft_safe_close(&data->original_stdout);
		ft_shutdown(&data, (unsigned char)data->status);
	}
	if (!is_numeric_arg(cmd_args[1], &exit_status))
	{
		handle_exit_error( cmd_args[1]);
		data->status = 1;
		return ;
	}
	if (cmd_args[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		data->status = 1;
		return ;
	}
	ft_free_cmd(data, cmd_args);
	ft_cleanup_execution(data);
	ft_safe_close(&data->original_stdin);
	ft_safe_close(&data->original_stdout);
	ft_shutdown(&data, (unsigned char)(exit_status % 256));
}
