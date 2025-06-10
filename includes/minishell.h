/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joseferr <joseferr@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:39:39 by pda-silv          #+#    #+#             */
/*   Updated: 2025/06/10 02:27:31 by joseferr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <termios.h>
# include <limits.h>
# include <fcntl.h>
# include "libft/libft.h"
# include "colors.h"
# include <errno.h>
# include <stdbool.h>

# define OP_SQUOTE '\''
# define OP_DQUOTE '\"'
# define OP_REDIRECT_IN '<'
# define OP_REDIRECT_OUT '>'
# define OP_REDIRECT_DELIM "<<"
# define OP_REDIRECT_APPEND ">>"
# define OP_PIPE '|'
# define OP_VAR '$'
# define OP_EXIT_STATUS "$?"
# define OP_ECHO "echo"
# define OP_NO_NL "-n"
# define OP_PWD "pwd"
# define OP_EXP "export"
# define OP_UNS "unset"
# define OP_ENV "env"
# define OP_EXT "exit"
# define OP_CD "cd"
# define OP_OR "||"
# define OP_AND "&&"
# define OP_WILD "*"
# define MAX_INPUT_SIZE 1024
# define MAX_CWD_SIZE 4096
# define MAX_PIPE_COUNT 10
# define MAX_TOKENS 100
# define FILE_PERM 0664

extern int	g_sigint_received;

typedef enum e_token_type
{
	CMD,
	BUILTIN,
	ARG,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_DELIM,
	REDIR_APPEND,
	ERROR
}	t_token_type;

typedef struct s_redir
{
	char	*delim;
	char	*delim_buf;
	int		in_fd;
	int		out_fd;
	bool	append;
}	t_redir;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
}	t_token;

typedef struct s_command
{
	t_redir	redir;
	t_token	tokens[MAX_TOKENS];
	int		token_count;
}	t_command;

typedef struct s_data
{
	t_command	commands[MAX_PIPE_COUNT];
	char		cwd[MAX_CWD_SIZE];
	char		**env;
	char		*input;
	char		*cmd_path;
	int			status;
	int8_t		cmd_count;
	int			prev_pipe;
	int			original_stdin;
	int			original_stdout;
	int			piped;
	int8_t		retval;
	pid_t		*pids;
	int			heredoc_sync[MAX_PIPE_COUNT][2];
}	t_data;

//	Parsing

//parser.c
int		ft_replace_tabs(char *str);
char	*ft_parse_word(char **ptr, t_data *data);
bool	ft_is_builtin(const char *command);
//parser_quotes.c
char	*ft_remove_quotes(char *str);
int		handle_quotes(char **ptr, int *in_quotes, char *quote_type);
char	*handle_quote(char *word, int *i, char *result, int *in_quotes);
//parser_tokens.c
char	**ft_tokens_to_args(t_command *command);
//parser_vars.c
char	*append_char(char *str, char c);
char	*ft_expand_variables(char *word, t_data *data);

//pathing.c
char	*ft_getenv(const char *name, char **env);
void	ft_getpath(t_data *data, char *arg);

//delim.c
void	ft_get_delim_buf(t_command *command, char *delim);

//	Execution
//execute.c
void	ft_cleanup_command_resources(t_data *data);
void	ft_execute_command(t_data *data, char **cmd_args, t_token_type type);
void	ft_execute(t_data *data);
//execute1.c
void	ft_safe_close(int *fd);
void	ft_handle_command(t_data *data, int *pipefd, int cmd_index,
			char **cmd_args);
void	ft_execute_lone_builtin(t_data *data, int cmd_index, char **cmd_args);

//	Builtin
//builtin.c
void	ft_execute_builtin(t_data *data, char **cmd_args);
void	ft_cd(t_data *data, char **cmd_args);
void	ft_echo(t_data *data, char **cmd_args);
void	ft_pwd(t_data *data, char **cmd_args);
void	ft_env(t_data *data);
void	ft_exit(t_data *data, char **cmd_args);
//export:
void	ft_export(t_data *data, char **cmd_args);
void	ft_print_sorted_env(char **env);
void	ft_update_shlvl(t_data *data);

void	ft_unset(t_data *data, char **cmd_args);
//	FD handling

//redict_fd.c
void	ft_open_redirect_fds(t_redir *redir, const char *in, const char *out);
void	ft_close_redirect_fds(t_redir *redir);

// Heredoc

//redirect_heredoc.c
void	ft_redirect_heredoc_to_file(t_command *command);
void	ft_setup_heredoc_pipe(t_command *cmd);
void	ft_handle_heredoc(t_data *data, t_command cmd, int cmd_index);
void	ft_setup_heredoc_sync(t_data *data);

//	Pipes

//redirect_pipes.c
void	ft_handle_pipes(t_data *data, int pipefd[2],
			t_command command, int cmd_index);
//pipes.c
void	ft_wait_children(t_data *data, pid_t *pids);
void	ft_setup_pipes(int pipefd[2]);
void	ft_pipe_error(t_data *data, char	**cmd_args);
void	ft_safe_pipe(int pipefd[2], t_data *data, char **cmd_args);
//shutdown.c
void	ft_free_tokens(t_data *data);
void	ft_free_env_array(t_data *data);
void	ft_shutdown(t_data **data, int retval);
void	ft_godark(t_data *data, char **cmd_args);
void	ft_cleanup_execution(t_data *data);
//signals.c
void	ft_set_prompt_signals(void);
void	ft_set_child_signals(void);
//starup.c
int		ft_initilaize(t_data **data, char **env);
//token_deez_nuts.c
void	ft_add_token_to_command(t_data *data, t_token token, int *count);

//token_parse.c
char	*ft_skip_whitespace(char *ptr);
t_token	ft_parse_token(char **ptr, t_data *data);
//token_quotes
int		ft_is_quotes_balanced(const char *str);

//tokenization.c
int		ft_tokenize_input(t_data *data, char *ptr, int count);
void	ft_free_cmd(t_data *data, char	**cmd_args);

#endif
