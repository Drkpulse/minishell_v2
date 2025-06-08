/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mangioni.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pda-silv <pda-silv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 13:30:00 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/03 20:38:37 by pda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*   Handles redirection of heredoc output to a file                         */
/*   Takes heredoc content from command and writes to the specified file     */
/*   Preserves existing file descriptors and ensures proper cleanup          */
/* ************************************************************************** */
void	ft_redirect_heredoc_to_file(t_command *command)
{
	int	write_result;

	if (!command->redir.delim_buf || !command->redir.out_fd)
		return ;
	if (command->redir.out_fd > 1)
	{
		write_result = write(command->redir.out_fd,
				command->redir.delim_buf,
				ft_strlen(command->redir.delim_buf));
		if (write_result == -1)
			perror("write to output file");
	}
}

/* ************************************************************************** */
/*                                                                            */
/*   Sets up a pipe for heredoc input                                        */
/*   Creates a pipe, writes heredoc content to it, and redirects stdin       */
/*   Ensures proper cleanup of resources                                     */
/* ************************************************************************** */
void	ft_setup_heredoc_pipe(t_command *cmd)
{
	int	heredoc_pipe[2];
	int	write_result;

	if (pipe(heredoc_pipe) == -1)
	{
		perror("pipe");
		free(cmd->redir.delim_buf);
		return ;
	}
	write_result = write(heredoc_pipe[1], cmd->redir.delim_buf,
			ft_strlen(cmd->redir.delim_buf));
	if (write_result == -1)
		perror("write to heredoc pipe");
	close(heredoc_pipe[1]);
	if (dup2(heredoc_pipe[0], STDIN_FILENO) == -1)
		perror("dup2 in heredoc");
	close(heredoc_pipe[0]);
	free(cmd->redir.delim_buf);
}

/* ************************************************************************** */
/*                                                                            */
/*   Manages pipe and redirection setup for command execution                */
/*   Handles input from heredocs, files, or previous pipes                   */
/*   Sets up output redirections to files or pipes                           */
/*   Ensures proper synchronization of heredoc processing                    */
/* ************************************************************************** */
void	ft_handle_heredoc(t_data *data, t_command cmd, int cmd_index)
{
	ft_get_delim_buf(&cmd, cmd.redir.delim);
	if (cmd.redir.out_fd != STDOUT_FILENO && cmd.redir.delim_buf)
	{
		ft_redirect_heredoc_to_file(&cmd);
		if (cmd_index < data->cmd_count)
		{
			if (write(data->heredoc_sync[cmd_index][1], "", 1) == -1)
				perror("write to heredoc sync");
			close(data->heredoc_sync[cmd_index][1]);
		}
		free(cmd.redir.delim_buf);
		return ;
	}
	if (cmd_index < data->cmd_count)
	{
		if (write(data->heredoc_sync[cmd_index][1], "", 1) == -1)
			perror("write to heredoc sync");
		close(data->heredoc_sync[cmd_index][1]);
	}
	if (cmd.redir.delim_buf)
		ft_setup_heredoc_pipe(&cmd);
}

/* ************************************************************************** */
/*                                                                            */
/*   Executes commands in the pipeline                                       */
/*   Sets up pipes and heredoc synchronization                               */
/*   Handles execution flow for builtins and external commands               */
/*   Manages command redirection and cleanup                                 */
/* ************************************************************************** */
void	ft_setup_heredoc_sync(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_count)
	{
		pipe(data->heredoc_sync[i]);
		i++;
	}
}
