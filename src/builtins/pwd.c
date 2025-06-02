/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 14:27:48 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/02 21:50:01 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*********************/
/*PWD Builtin Command*/
/*********************/
void	ft_pwd(t_data *data, char **cmd_args)
{
	if (!cmd_args[1])
		ft_printf("%s\n", data->cwd);
	else
	{
		ft_putstr_fd("pwd: too many arguments\n", STDERR_FILENO);
		data->status = 1;
	}
}
