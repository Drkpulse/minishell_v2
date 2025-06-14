##	PROJECT
NAME	=	minishell

##	MAKEFILE VARS
include	./includes/make/vars.mk

##	C FILES
BUILTIN_FILES =	builtins/cd.c builtins/echo.c builtins/env.c builtins/export.c builtins/export_print.c builtins/pwd.c builtins/unset.c builtins/exit.c builtins/builtin.c builtins/export_supp.c builtins/export_shlvl.c
SRC_FILES	=	main.c startup.c parser.c tokenization.c shutdown.c execute.c pathing.c mario.c files.c delim.c execute1.c parser_quotes.c parser_tokens.c parser_vars.c luigi.c mangioni.c token_parse.c token_quotes.c signals.c token_deez_nuts.c
SRC			=	${addprefix ./src/, ${SRC_FILES} ${BUILTIN_FILES}}
OBJ			=	${SRC:.c=.o}

##	BASIC RULES
${NAME}: init libft ${OBJ}
	${CC} ${CFLAGS} ${OBJ} ${INCLUDES} ${LIBRARIES} ${LIBS} -o ${BIN_DIR}/${NAME}

all: ${NAME}

clean: libft_clean
	rm -f ${OBJ}

fclean: clean libft_fclean
	rm -f ${BIN_DIR}/${NAME}

re: fclean all

%.o: %.c
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@

##	HELPER MAKEFILES
include	./includes/make/libs.mk
include	./includes/make/tests.mk

.PHONY: all clean fclean re bonus init \
	libft libft_clone libft_re libft_clean libft_fclean \
	norm run print_valgrind_results compile_with_progress urun
