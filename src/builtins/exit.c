/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 14:27:48 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/10 02:29:13 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	its_long(char *arg)
{
	if (ft_strlen(arg) >= 20 || (ft_strlen(arg) == 19
			&& !(arg[0] == '+' || arg[0] == '-')))
		return (0);
	return (1);
}

long	read_num_exit(char *arg, int i)
{
	long	result;

	result = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (-1);
		result = result * 10 + (arg[i] - '0');
		i ++;
	}
	return (result);
}

static int	is_numeric_arg(char *arg, t_data *data)
{
	int		i;
	bool	neg;
	long	result;

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
		return (0);
	}
	result = read_num_exit(arg, i);
	if (result == -1)
		return (0);
	data->status = (int)(result % 256);
	if (neg)
		data->status = 256 - data->status;
	return (1);
}

/*********************/
/* Exit Builtin Command */
/*********************/
void	ft_exit(t_data *data, char **cmd_args)
{
	if (data->piped == 0)
		printf("exit\n");
	if (!cmd_args[1])
		ft_godark(data, cmd_args);
	else if (cmd_args[2] && is_numeric_arg(cmd_args[1], data))
	{
		printf("exit: too many arguments\n");
		data->status = 1;
		return ;
	}
	else if ((!its_long(cmd_args[1])) ||!is_numeric_arg(cmd_args[1], data))
	{
		printf("exit: too many arguments\n");
		data->status = 2;
		return ;
	}
	ft_godark(data, cmd_args);
}
