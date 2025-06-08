/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 14:27:48 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/03 20:42:15 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long read_num_exit(char *arg, t_data *data, int i, bool neg)
{
	long result;

	result = 0;
	while (arg[i])
	{
		if(!ft_isdigit(arg[i]))
		{

			data->status = 2;
			return(-1);
		}
		result = result * 10 + (arg[i] - '0');
		if ((!neg && result > LONG_MAX) || (neg && -result < LONG_MIN))
		{
			data->status = 2;
			return (-1);
		}
		i ++;
	}
	return (result);
}

static int	is_numeric_arg(char *arg, t_data *data)
{
	int		i;
	bool	neg;
	long 	result;

	i = 0;
	neg = 0;
	if (arg[i] == '-' || arg[i] == '+')
	{
		if (arg[i] == '-')
			neg = 1;
		i++;
	}
	if (!arg[i])
	{
		data->status = 2;
		return(0);
	}
	result = read_num_exit(arg, data, i, neg);
	if (result == -1)
		return (0);
	data->status = (int)(result % 256);
	if (neg)
		data->status = 256 - data->status;
	return(1);
}

static void	print_exit_msg(char *arg)
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
		ft_printf("exit, %s\n", cmd_args[0]);
	if (!cmd_args[1])
		ft_godark(data, cmd_args);
	ft_printf("a");
	if (!is_numeric_arg(cmd_args[1], data))
	{
		print_exit_msg(cmd_args[1]);
		return ;
	}
	ft_printf("a");
	if (cmd_args[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		data->status = 1;
		return ;
	}
	ft_printf("a");
	printf("%d", data->status);
	ft_godark(data, cmd_args);
}