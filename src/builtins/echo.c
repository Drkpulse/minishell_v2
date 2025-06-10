/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 14:27:48 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/10 02:29:24 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/************************/
/*Checks for the -n flag*/
/************************/
int	is_valid_echo_flag(char *flag)
{
	int	i;
	char	*temp;

	i = 1;
	temp = flag;
	if(temp[0] != '-' || temp[1] == '\0')
		return (0);
	while (temp[i] != '\0')
	{
		if (temp[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/**********************/
/*Echo Builtin Command*/
/**********************/
void	ft_echo(t_data *data, char **cmd_args)
{
	int	newline;
	int	i;

	newline = 1;
	i = 1;
	while ((cmd_args[i] && cmd_args[i][0] == '-' )
		&& is_valid_echo_flag(cmd_args[i]) == 1)
	{
		printf("Newline\n");
		newline = 0;
		i++;
	}
	while (cmd_args[i])
	{
		printf("%s", cmd_args[i]);
		if (cmd_args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	data->status = 0;
}
