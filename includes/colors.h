/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:00:00 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/10 02:24:12 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

/* Regular text colors */
# define C_BLACK "\033[0;30m"
# define C_RED "\033[0;31m"
# define C_GREEN "\033[0;32m"
# define C_YELLOW "\033[0;33m"
# define C_BLUE "\033[0;34m"
# define C_PURPLE "\033[0;35m"
# define C_CYAN "\033[0;36m"
# define C_WHITE "\033[0;37m"

/* Bold text colors */
# define C_BRT_BLACK "\033[1;30m"
# define C_BRT_RED "\033[1;31m"
# define C_BRT_GREEN "\033[1;32m"
# define C_BRT_YELLOW "\033[1;33m"
# define C_BRT_BLUE "\033[1;34m"
# define C_BRT_PURPLE "\033[1;35m"
# define C_BRT_CYAN "\033[1;36m"
# define C_BRT_WHITE "\033[1;37m"

/* Reset color */
# define RESET_COLOR "\033[39m"
# define RESET_ALL "\033[0m"

#endif
