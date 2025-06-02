/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pda-silv <pda-silv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 10:23:36 by joseferr          #+#    #+#             */
/*   Updated: 2025/06/02 22:10:26 by pda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_skip_whitespace(char *ptr)
{
	while (*ptr == ' ')
		ptr++;
	return (ptr);
}

static void	ft_parse_output_redir(char **ptr, t_token *token)
{
	if (*(*ptr + 1) == '>')
	{
		token->type = REDIR_APPEND;
		token->value = ft_strdup(">>");
		(*ptr) += 2;
	}
	else
	{
		token->type = REDIR_OUT;
		token->value = ft_strdup(">");
		(*ptr)++;
	}
}

static void	ft_parse_input_redir(char **ptr, t_token *token)
{
	if (*(*ptr + 1) == '<')
	{
		token->type = REDIR_DELIM;
		token->value = ft_strdup("<<");
		(*ptr) += 2;
		*ptr = ft_skip_whitespace(*ptr);
		if (!**ptr || **ptr == '|' || **ptr == '>' || **ptr == '<')
		{
			ft_printf(C_RED
				"minishell: syntax error near unexpected token `newline'\n"
				RESET_ALL);
			ft_free((void **)&token->value);
			token->type = ERROR;
		}
	}
	else
	{
		token->type = REDIR_IN;
		token->value = ft_strdup("<");
		(*ptr)++;
	}
}

static t_token	ft_parse_redirection(char **ptr)
{
	t_token	token;

	ft_bzero(&token, sizeof(t_token));
	if (**ptr == '>')
		ft_parse_output_redir(ptr, &token);
	else if (**ptr == '<')
	{
		ft_parse_input_redir(ptr, &token);
		if (token.type == ERROR)
			return (token);
	}
	return (token);
}

t_token	ft_parse_token(char **ptr, t_data *data)
{
	t_token	token;

	*ptr = ft_skip_whitespace(*ptr);
	if (**ptr == '|')
	{
		token.type = PIPE;
		token.value = ft_strdup("|");
		(*ptr)++;
	}
	else if (**ptr == '>' || **ptr == '<')
		token = ft_parse_redirection(ptr);
	else
	{
		token.type = CMD;
		token.value = ft_parse_word(ptr, data);
		if (ft_is_builtin(token.value))
			token.type = BUILTIN;
	}
	return (token);
}
