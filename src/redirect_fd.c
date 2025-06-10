/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:57:22 by pda-silv          #+#    #+#             */
/*   Updated: 2025/06/10 02:26:24 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_open_redirect_fds(t_redir *redir, const char *in, const char *out)
{
	if (in && *in)
	{
		if (redir->in_fd != STDIN_FILENO && redir->in_fd > 0)
			close(redir->in_fd);
		if (redir->in_fd != -2)
			redir->in_fd = open(in, O_RDONLY);
		if (redir->in_fd == -1 && redir->in_fd != -2)
		{
			printf(C_RED"minishell: %s: %s\n"RESET_ALL, in, strerror(errno));
			redir->in_fd = -2;
		}
	}
	if (out && *out)
	{
		if (redir->out_fd != STDOUT_FILENO && redir->out_fd > 1)
			close(redir->out_fd);
		if (redir->append)
			redir->out_fd = open(out, O_CREAT | O_APPEND | O_WRONLY, FILE_PERM);
		else
			redir->out_fd = open(out, O_CREAT | O_TRUNC | O_WRONLY, FILE_PERM);
		if (redir->out_fd < 0)
			printf(C_RED"minishell: %s: %s\n"RESET_ALL, out,
				strerror(errno));
		redir->append = false;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*   Closes any open redirection file descriptors                            */
/*   Uses safe closing to prevent double-close errors                        */
/*   Handles both input and output file descriptors                          */
/* ************************************************************************** */
void	ft_close_redirect_fds(t_redir *redir)
{
	if (redir->in_fd != STDIN_FILENO)
		ft_safe_close(&redir->in_fd);
	if (redir->out_fd != STDOUT_FILENO)
		ft_safe_close(&redir->out_fd);
}
