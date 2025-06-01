/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luigi.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 13:30:00 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/01 18:30:33 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*   Handles input redirection setup for commands                            */
/*   Processes input from files or previous pipes                            */
/*   Ensures correct file descriptor duplication                             */
/*   Properly closes original file descriptors after duplication             */
/* ************************************************************************** */
static void	ft_handle_input(t_data *data, t_command cmd, int cmd_index)
{
	if (cmd.redir.in_fd != STDIN_FILENO)
	{
		dup2(cmd.redir.in_fd, STDIN_FILENO);
		close(cmd.redir.in_fd);
	}
	else if (cmd_index > 0)
	{
		dup2(data->prev_pipe, STDIN_FILENO);
		close(data->prev_pipe);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*   Handles output redirection setup for commands                           */
/*   Manages output to files or next pipe in the pipeline                    */
/*   Ensures correct file descriptor duplication                             */
/*   Properly closes original file descriptors after duplication             */
/* ************************************************************************** */
static void	ft_handle_output(t_data *data, t_command cmd,
	int pipefd[2], int cmd_index)
{
	if (cmd.redir.out_fd != STDOUT_FILENO)
	{
		dup2(cmd.redir.out_fd, STDOUT_FILENO);
		close(cmd.redir.out_fd);
	}
	else if (cmd_index < data->cmd_count)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*   Coordinates pipe and redirection handling                               */
/*   Sets up inheritance of file descriptors between commands                */
/*   Processes heredoc input when needed                                     */
/*   Ensures proper synchronization in the pipeline                          */
/* ************************************************************************** */
void	ft_handle_pipes(t_data *data, int pipefd[2],
	t_command command, int cmd_index)
{
	int	i;

	i = 0;
	while (i < data->cmd_count)
	{
		if (i != cmd_index && (data->heredoc_sync[i][0] >= 0))
			close(data->heredoc_sync[i][0]);
		if (data->heredoc_sync[i][1] >= 0)
			close(data->heredoc_sync[i][1]);
		i++;
	}
	if (cmd_index > 0 && data->prev_pipe != -1)
		ft_handle_input(data, command, cmd_index);
	if (command.redir.delim)
		ft_handle_heredoc(data, command, cmd_index);
	ft_handle_output(data, command, pipefd, cmd_index);
	signal(SIGPIPE, SIG_DFL);
	if (cmd_index < data->cmd_count)
		close(pipefd[0]);
}
