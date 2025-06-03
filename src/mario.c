/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mario.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 13:30:00 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/03 20:17:16 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_wait_children(t_data *data, pid_t *pids)
{
	int	i;
	int	status;

	i = 0;
	if (pids)
	{
		while (i <= data->cmd_count)
		{
			if (pids[i] > 0)
			{
				status = 0;
				waitpid(pids[i], &status, 0);
				if (i == data->cmd_count)
				{
					if (WIFSIGNALED(status))
						data->status = 128 + WTERMSIG(status);
					else if (WIFEXITED(status))
						data->status = WEXITSTATUS(status);
				}
			}
			i++;
		}
		free(pids);
	}
	if (data->prev_pipe >= 0)
		ft_safe_close(&data->prev_pipe);
}

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
			command->redir.delim_buf, ft_strlen(command->redir.delim_buf));
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

	/* Check if we need to redirect heredoc to a file */
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

	/* Original heredoc handling for pipes continues here */
	if (cmd_index < data->cmd_count)
	{
		if (write(data->heredoc_sync[cmd_index][1], "", 1) == -1)
			perror("write to heredoc sync");
		close(data->heredoc_sync[cmd_index][1]);
	}
	if (cmd.redir.delim_buf)
		ft_setup_heredoc_pipe(&cmd);
}

void	ft_setup_pipes(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

void	ft_safe_pipe(int pipefd[2], t_data *data, char **cmd_args)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		ft_free_cmd(data, cmd_args);
		exit(EXIT_FAILURE);
	}
}

void	ft_pipe_error(t_data *data, char **cmd_args)
{
	perror("fork");
	ft_free_cmd(data, cmd_args);
}
