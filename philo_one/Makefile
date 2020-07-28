NAME = philo_one

CC = gcc

CFLAGS = -Wall -Wextra -Werror

SRCS =  create_philo.c \
		ft_atoi.c \
		init.c \
		libft.c \
		philosophers.c \
		print_status.c \
		utils.c

OBJS = $(SRCS:.c=.o)

RM = rm -f

%.o : %.c
		${CC} ${CFLAGS} -c $< -o $@

$(NAME) : ${OBJS}
		${CC} ${CFLAGS} ${OBJS} -o $(NAME)
	
all :	${NAME}

clean:
		${RM} ${OBJS}

fclean:	clean
		${RM} ${NAME}

re:		fclean all
