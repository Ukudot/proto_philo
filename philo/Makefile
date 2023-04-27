# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gpanico <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/27 07:32:54 by gpanico           #+#    #+#              #
#    Updated: 2023/04/27 07:41:59 by gpanico          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo
SRCS	= philo.c philo_checks.c philo_thread.c philo_philos.c philo_utils.c
OBJS	= ${SRCS:.c=.o}
CC		= gcc -Wall -Werror -Wextra
RM		= rm -f

%.o:%.c
	${CC} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	${CC} ${OBJS} -o ${NAME}

all: ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
