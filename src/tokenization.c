/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pda-silv <pda-silv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 10:23:36 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/03 20:42:55 by pda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_cmd(t_data *data, char **cmd_args)
{
	if (data && data->cmd_path)
	{
		ft_free((void **)&data->cmd_path);
		data->cmd_path = NULL;
	}
	if (cmd_args)
	{
		ft_free_array((void **)cmd_args);
		cmd_args = NULL;
	}
}

static void	ft_handle_pipe_token(t_data *data, int *count)
{
	data->cmd_count++;
	*count = 0;
}

static void	ft_pipe_syntax_error(t_data *data, t_token token)
{
	ft_printf(C_RED"syntax error near unexpected token `|'\n"
		RESET_ALL);
	ft_free((void **)&token.value);
	ft_free_tokens(data);
	data->status = 258;
}

static int	ft_process_token(t_data *data, t_token token,
	char **ptr, int *count)
{
	if (token.type == ERROR)
	{
		data->status = 2;
		return (NOK);
	}
	if (!token.value)
		return (OK);
	if (token.type == PIPE)
	{
		if (*count == 0 || !*ft_skip_whitespace(*ptr))
		{
			ft_pipe_syntax_error(data, token);
			return (NOK);
		}
		ft_free((void **)&token.value);
		ft_handle_pipe_token(data, count);
	}
	else
		ft_add_token_to_command(data, token, count);
	*ptr = ft_skip_whitespace(*ptr);
	return (OK);
}

int	ft_tokenize_input(t_data *data, char *ptr, int count)
{
	t_token	token;

	if (!ft_is_quotes_balanced(ptr))
		return (NOK);
	while (*ptr)
	{
		token = ft_parse_token(&ptr, data);
		if (ft_process_token(data, token, &ptr, &count) == NOK)
			return (NOK);
	}
	return (OK);
}
