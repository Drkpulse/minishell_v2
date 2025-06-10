/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shutdown.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:37:51 by pda-silv          #+#    #+#             */
/*   Updated: 2025/06/08 20:14:48 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	ft_free_tokens(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i <= data->cmd_count)
	{
		j = 0;
		while (j < data->commands[i].token_count)
		{
			if (data->commands[i].tokens[j].value)
				ft_free((void **)&data->commands[i].tokens[j].value);
			j++;
		}
		i++;
	}
	if (data->cmd_path)
		ft_free((void **)&data->cmd_path);
	if (data->pids)
		ft_free((void **)&data->pids);
}

void	ft_free_env_array(t_data *data)
{
	int	i;

	i = 0;
	if (!data->env)
		return ;
	while (data->env[i])
	{
		if (data->env[i])
			ft_free((void **)&data->env[i]);
		i++;
	}
	ft_free((void **)&data->env);
}
//this function its onlu used for exit()- is aux function
void	ft_shutdown(t_data **data, int retval)
{
	if (*data)
	{
		clear_history();
		if ((*data)->input)
			ft_free((void **)&((*data)->input));
		ft_free_tokens(*data);
		ft_free_env_array(*data);
		ft_free((void **)data);
	}
	exit(retval);
}

void	ft_godark(t_data *data, char **cmd_args)
{
	ft_free_cmd(data, cmd_args);
	ft_cleanup_execution(data);
	ft_safe_close(&data->original_stdin);
	ft_safe_close(&data->original_stdout);
	ft_shutdown(&data, (unsigned char)data->status);
}

/* ************************************************************************** */
/*                                                                            */
/*   Performs cleanup operations after command execution                     */
/*   Closes heredoc synchronization pipes                                    */
/*   Waits for child processes to complete                                   */
/*   Closes any redirected file descriptors                                  */
/* ************************************************************************** */
void	ft_cleanup_execution(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_count)
	{
		ft_safe_close(&data->heredoc_sync[i][0]);
		ft_safe_close(&data->heredoc_sync[i][1]);
		i++;
	}
	ft_wait_children(data, data->pids);
	i = 0;
	while (i <= data->cmd_count)
	{
		ft_close_redirect_fds(&data->commands[i].redir);
		i++;
	}
	ft_cleanup_command_resources(data);
	data->pids = NULL;
	ft_free_tokens(data);
}
