/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eport_shlvl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pda-silv <pda-silv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 20:25:22 by pda-silv          #+#    #+#             */
/*   Updated: 2025/06/03 20:26:27 by pda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_shlvl_var(char *shlvl_str)
{
	int		shlvl;
	char	*new_shlvl;
	char	*var;

	if (!shlvl_str || !*shlvl_str)
		shlvl = 0;
	else
		shlvl = ft_atoi(shlvl_str);
	shlvl++;
	new_shlvl = ft_itoa(shlvl);
	if (!new_shlvl)
		return (NULL);
	var = ft_strjoin("SHLVL=", new_shlvl);
	free(new_shlvl);
	return (var);
}

void	ft_update_shlvl(t_data *data)
{
	char	*shlvl_str;
	char	*var;
	int		count;

	shlvl_str = ft_getenv("SHLVL", data->env);
	var = create_shlvl_var(shlvl_str);
	if (!var)
		return ;
	if (var_exists(data->env, "SHLVL"))
		process_var_with_equal(data, var);
	else
	{
		count = 0;
		while (data->env[count])
			count++;
		add_env_variable(data, var, count);
	}
	free(var);
}
