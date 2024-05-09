NAME		=	ft_shield

SRCS		= 	\
				cleanup.c \
				main.c \
				daemonize.c \
				lock.c \
				$(addprefix polling/, \
					prepare_socket.c \
					poll_routine.c \
					cli.c \
				)

_OBJS		=	${SRCS:.c=.o}
OBJS		=	$(addprefix build/, $(_OBJS))
OBJS_DEPEND	=	${OBJS:.o=.d}

CC			=	clang
CFLAGS		=	-Wall -Wextra -Werror -g3
INCLUDE		=	-I includes/

all		:	$(NAME)

build/%.o	:	srcs/%.c
	@if [ ! -d $(dir $@) ]; then\
		mkdir -p $(dir $@);\
	fi
	$(CC) ${CFLAGS} -MMD -MF $(@:.o=.d) ${INCLUDE} -c $< -o $@

$(NAME)	:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

-include $(OBJS_DEPEND)

clean	:	
	rm -Rf build/ $(TEST_NAME)

fclean	:	clean
	rm -f ${NAME}

re		:	fclean
			make ${NAME}

.PHONY	:	all clean fclean re
