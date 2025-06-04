/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 14:27:48 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/03 20:51:44 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/***********************************/
/*Check and Execute Builtin Command*/
/***********************************/
void	ft_execute_builtin(t_data *data, char **cmd_args)
{
	char	*cmd;
	size_t	len;

	cmd = cmd_args[0];
	len = ft_strlen(cmd) + 1;
	data->status = 0;
	if (!ft_strncmp(OP_EXT, cmd, len))
		ft_exit(data, cmd_args);
	if (!ft_strncmp(OP_PWD, cmd, len))
		ft_pwd(data, cmd_args);
	if (!ft_strncmp(OP_ECHO, cmd, len))
		ft_echo(data, cmd_args);
	if (!ft_strncmp(OP_ENV, cmd, len))
		ft_env(data);
	if (!ft_strncmp(OP_EXP, cmd, len))
		ft_export(data, cmd_args);
	if (!ft_strncmp(OP_UNS, cmd, len))
		ft_unset(data, cmd_args);
	if (!ft_strncmp(OP_CD, cmd, len))
		ft_cd(data, cmd_args);
}
