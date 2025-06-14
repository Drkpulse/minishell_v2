/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 20:11:45 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/05 20:38:04 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*   Clean up command resources before shutdown                               */
/*   Safely frees redirection and command-related memory                      */
/*   Follows the pattern of other memory management functions                 */
/*   Ensures no memory leaks or double frees occur                            */
/* ************************************************************************** */
void	ft_cleanup_command_resources(t_data *data)
{
	int	i;

	i = 0;
	while (i <= data->cmd_count)
	{
		if (data->commands[i].redir.delim)
		{
			ft_free((void **)&data->commands[i].redir.delim);
		}
		if (data->commands[i].redir.delim_buf)
		{
			ft_free((void **)&data->commands[i].redir.delim_buf);
		}
		i++;
	}
}

void	ft_command_not_found(t_data *data, char **cmd_args)
{
	ft_printf(C_RED"%s: Command not found\n"RESET_ALL, cmd_args[0]);
	ft_free((void **)&data->cmd_path);
	ft_free_array((void **)cmd_args);
	ft_free_env_array(data);
	ft_cleanup_command_resources(data);
	ft_shutdown(&data, 127);
}

/* ************************************************************************** */
/*                                                                            */
/*   Executes a command with the given arguments                              */
/*   For builtins, runs them directly                                         */
/*   For external commands, uses execve with proper path                      */
/*   Handles cleanup and exits the child process when done                    */
/* ************************************************************************** */
void	ft_execute_command(t_data *data, char **cmd_args, t_token_type type)
{
	if (data->commands[0].redir.in_fd < 0 || data->commands[0].redir.out_fd < 0
		|| !cmd_args[0])
		exit(data->status);
	if (type == BUILTIN)
		ft_execute_builtin(data, cmd_args);
	else
	{
		if (data->cmd_path == NULL)
			ft_command_not_found(data, cmd_args);
		execve(data->cmd_path, cmd_args, data->env);
		perror("execve");
		data->status = EXIT_FAILURE;
	}
	ft_free((void **)&data->cmd_path);
	ft_free_array((void **)cmd_args);
	exit(data->status);
}

static void	ft_prepare_command(t_data *data, int cmd_index, char ***cmd_args)
{
	*cmd_args = ft_tokens_to_args(&data->commands[cmd_index]);
	ft_getpath(data, *cmd_args[0]);
}

/* ************************************************************************** */
/*                                                                            */
/*   Main command execution controller                                        */
/*   Allocates memory for process IDs                                         */
/*   Processes commands one by one                                            */
/*   Handles special case for lone builtins                                   */
/*   Coordinates cleanup after execution                                      */
/* ************************************************************************** */
void	ft_execute(t_data *data)
{
	int		pipefd[2];
	int		cmd_index;
	char	**cmd_args;

	ft_setup_heredoc_sync(data);
	cmd_index = -1;
	data->status = 0;
	data->pids = malloc((data->cmd_count + 1) * sizeof(pid_t));
	if (!data->pids)
		return ;
	while (++cmd_index <= data->cmd_count)
		data->pids[cmd_index] = -1;
	cmd_index = -1;
	data->prev_pipe = -1;
	while (++cmd_index < data->cmd_count + 1)
	{
		ft_prepare_command(data, cmd_index, &cmd_args);
		if (data->cmd_count == 0 && data->commands[cmd_index]
			.tokens->type == BUILTIN)
			ft_execute_lone_builtin(data, cmd_index, cmd_args);
		else
			ft_handle_command(data, pipefd, cmd_index, cmd_args);
		ft_free_cmd(data, cmd_args);
	}
	ft_cleanup_execution(data);
}
