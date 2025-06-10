/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   startup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 09:23:36 by pda-silv          #+#    #+#             */
/*   Updated: 2025/06/10 03:07:24 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Starts with the Beauty of ASCII Art */
static void	ft_header(void)
{
	printf(C_BRT_GREEN "\n");
	printf(" _        _         __     __)                             \n");
	printf(" /   /   '  )      (, /|  /|   ,      ,     /)       /) /) \n");
	printf("/___/_  ,--'         / | / |    __      _  (/    _  // //  \n");
	printf("   /   /___       ) /  |/  |__(_/ (__(_/_)_/ )__(/_(/_(/_  \n");
	printf("  /              (_/   '                                   \n");
	printf("\n\tProject by joseferr & anferrei\n\n" RESET_COLOR);
}


static int	ft_init_data(t_data **data)
{
	*data = ft_calloc(1, sizeof(t_data));
	if (!data)
	{
		perror("calloc");
		return (NOK);
	}
	return (OK);
}

static char	**ft_envdup(char **env)
{
	int		count;
	char	**copy;
	int		i;

	count = 0;
	while (env[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(env[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

int	ft_initilaize(t_data **data, char **env)
{
	ft_header();
	if (ft_init_data(data))
		return (NOK);
	(*data)->env = ft_envdup(env);
	if (!(*data)->env)
		return (NOK);
	ft_update_shlvl(*data);
	return (OK);
}
