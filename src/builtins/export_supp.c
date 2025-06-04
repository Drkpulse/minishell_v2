/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_supp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 14:27:48 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/01 18:31:36 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	swap_strings(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	var_exists(char **env, char *var)
{
	int	i;
	int	var_len;

	i = 0;
	var_len = ft_strlen(var);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, var_len) == 0
			&& (env[i][var_len] == '=' || env[i][var_len] == '\0'))
			return (1);
		i++;
	}
	return (0);
}

void	write_error_message(char *var)
{
	write(2, "minishell: export: `", 20);
	write(2, var, ft_strlen(var));
	write(2, "': not a valid identifier\n", 26);
}

int	is_valid_identifier_char(char c, int first_char)
{
	if (first_char)
		return (ft_isalpha(c) || c == '_');
	return (ft_isalnum(c) || c == '_');
}

int	check_identifier(char *arg)
{
	int	i;

	if (!arg || !*arg)
		return (0);
	if (!is_valid_identifier_char(arg[0], 1))
		return (0);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!is_valid_identifier_char(arg[i], 0))
			return (0);
		i++;
	}
	return (1);
}
