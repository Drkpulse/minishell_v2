/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 20:11:45 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/02 22:01:08 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*   Creates a child process to execute a command                            */
/*   Sets up pipes for communication between processes                       */
/*   Configures signal handling for child processes                          */
/*   Handles execution of commands in the child process                      */
/* ************************************************************************** */
static void	ft_create_child_process(t_data *data, int pipefd[2],
	int cmd_index, char **cmd_args)
{
	struct sigaction	sa;

	if (cmd_index < data->cmd_count)
		ft_setup_pipes(pipefd);
	data->pids[cmd_index] = fork();
	if (data->pids[cmd_index] == -1)
		ft_pipe_error(data, cmd_args);
	else if (data->pids[cmd_index] == 0)
	{
		ft_set_child_signals();
		ft_handle_pipes(data, pipefd, data->commands[cmd_index], cmd_index);
		ft_execute_command(data, cmd_args,
			data->commands[cmd_index].tokens->type);
		exit(EXIT_FAILURE);
	}
	else
	{
		sigemptyset(&sa.sa_mask);
		sa.sa_handler = SIG_IGN;
		sa.sa_flags = 0;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*   Safely closes a file descriptor if it's valid                           */
/*   Updates the descriptor to -1 after closing to prevent double closing    */
/*   Ignores descriptors that are already set to -1                          */
/* ************************************************************************** */
void	ft_safe_close(int *fd)
{
	if (fd && *fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}

static void	ft_handle_parent(t_data *data, int pipefd[2], int cmd_index)
{
	if (cmd_index < data->cmd_count)
		ft_safe_close(&pipefd[1]);
	if (data->prev_pipe != -1)
		ft_safe_close(&data->prev_pipe);
	if (cmd_index < data->cmd_count)
		data->prev_pipe = pipefd[0];
}

/* ************************************************************************** */
/*                                                                            */
/*   Handles execution of commands in a pipeline                             */
/*   Waits for certain processes when needed                                 */
/*   Creates child processes for command execution                           */
/*   Manages parent process handling of pipes                                */
/* ************************************************************************** */
void	ft_handle_command(t_data *data, int *pipefd, int cmd_index,
	char **cmd_args)
{
	if (cmd_index > 0 && data->commands[cmd_index - 1].redir.out_fd
		!= STDOUT_FILENO && data->commands[cmd_index].redir.in_fd
		!= STDIN_FILENO)
	{
		if (data->pids[cmd_index - 1] > 0)
			waitpid(data->pids[cmd_index - 1], NULL, 0);
	}
	ft_create_child_process(data, pipefd, cmd_index, cmd_args);
	if (data->pids[cmd_index] > 0)
		ft_handle_parent(data, pipefd, cmd_index);
}

/* ************************************************************************** */
/*                                                                            */
/*   Executes a lone builtin command without forking                         */
/*   Sets up file redirection for stdin and stdout                           */
/*   Preserves original file descriptors                                     */
/*   Restores original stdin and stdout after execution                      */
/* ************************************************************************** */
void	ft_execute_lone_builtin(t_data *data, int cmd_index, char **cmd_args)
{
	data->original_stdin = dup(STDIN_FILENO);
	data->original_stdout = dup(STDOUT_FILENO);
	if (data->original_stdin == -1 || data->original_stdout == -1)
		return ;
	if (data->commands[cmd_index].redir.in_fd != STDIN_FILENO)
	{
		dup2(data->commands[cmd_index].redir.in_fd, STDIN_FILENO);
		ft_safe_close(&data->commands[cmd_index].redir.in_fd);
	}
	if (data->commands[cmd_index].redir.out_fd != STDOUT_FILENO)
	{
		dup2(data->commands[cmd_index].redir.out_fd, STDOUT_FILENO);
		ft_safe_close(&data->commands[cmd_index].redir.out_fd);
	}
	data->pids[cmd_index] = -1;
	ft_execute_builtin(data, cmd_args);
	dup2(data->original_stdin, STDIN_FILENO);
	dup2(data->original_stdout, STDOUT_FILENO);
	ft_safe_close(&data->original_stdin);
	ft_safe_close(&data->original_stdout);
}
