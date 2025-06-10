/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 13:30:00 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/08 20:14:25 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_set_exit_status(t_data *data, int status)
{
	if (WIFSIGNALED(status))
		data->status = 128 + WTERMSIG(status);
	else if (WIFEXITED(status))
		data->status = WEXITSTATUS(status);
}

void	ft_wait_children(t_data *data, pid_t *pids)
{
	int	i;
	int	status;

	i = 0;
	if (!pids)
	{
		if (data->prev_pipe >= 0)
			ft_safe_close(&data->prev_pipe);
		return ;
	}
	while (i <= data->cmd_count)
	{
		if (pids[i] > 0)
		{
			status = 0;
			waitpid(pids[i], &status, 0);
			if (i == data->cmd_count)
				ft_set_exit_status(data, status);
		}
		i++;
	}
	free(pids);
	if (data->prev_pipe >= 0)
		ft_safe_close(&data->prev_pipe);
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
