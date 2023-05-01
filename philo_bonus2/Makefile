# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gpanico <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/27 07:32:54 by gpanico           #+#    #+#              #
#    Updated: 2023/05/01 16:00:26 by gpanico          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo_bonus
SRCS	= philo_bonus.c philo_checks_bonus.c philo_utils_bonus.c
OBJS	= ${SRCS:.c=.o}
CC		= gcc -Wall -Werror -Wextra -fsanitize=thread 
RM		= rm -f

%.o:%.c
	${CC} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	${CC} ${OBJS} -o ${NAME}

all: ${NAME}

bonus: all

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re all
