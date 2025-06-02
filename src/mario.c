/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mario.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 13:30:00 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/02 22:05:39 by joseferr         ###   ########.fr       */
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
				status = 0;  /* Initialize status before using it */
				waitpid(pids[i], &status, 0);
				if (i == data->cmd_count) // Last command's status
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
/*   Manages pipe and redirection setup for command execution                */
/*   Handles input from heredocs, files, or previous pipes                   */
/*   Sets up output redirections to files or pipes                           */
/*   Ensures proper synchronization of heredoc processing                    */
/* ************************************************************************** */
void	ft_handle_heredoc(t_data *data, t_command cmd, int cmd_index)
{
	int	heredoc_pipe[2];
	int	write_result;

	ft_get_delim_buf(&cmd, cmd.redir.delim);
	if (cmd_index < data->cmd_count)
	{
		write_result = write(data->heredoc_sync[cmd_index][1], "", 1);
		if (write_result == -1)
			perror("write to heredoc sync");
		close(data->heredoc_sync[cmd_index][1]);
	}
	if (cmd.redir.delim_buf)
	{
		if (pipe(heredoc_pipe) == -1)
		{
			perror("pipe");
			free(cmd.redir.delim_buf);
			return ;
		}
		write_result = write(heredoc_pipe[1], cmd.redir.delim_buf,
				ft_strlen(cmd.redir.delim_buf));
		if (write_result == -1)
			perror("write to heredoc pipe");
		close(heredoc_pipe[1]);
		if (dup2(heredoc_pipe[0], STDIN_FILENO) == -1)
			perror("dup2 in heredoc");
		close(heredoc_pipe[0]);
		free(cmd.redir.delim_buf);
	}
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
