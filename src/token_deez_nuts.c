/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_deez_nuts.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pda-silv <pda-silv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 20:43:08 by pda-silv          #+#    #+#             */
/*   Updated: 2025/06/03 20:43:18 by pda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_token_to_command(t_data *data, t_token token, int *count)
{
	data->commands[data->cmd_count].tokens[(*count)++] = token;
	data->commands[data->cmd_count].token_count = *count;
	data->commands[data->cmd_count].redir.in_fd = 0;
	data->commands[data->cmd_count].redir.out_fd = 1;
}
