/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:37:12 by gpanico           #+#    #+#             */
/*   Updated: 2023/04/24 16:12:05 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_shared
{
	int				start;
	int				n_philos;
	int				death;
	long			o_time;
	int				time_to_eat;
	int				time_to_die;
	int				time_to_sleep;
	int				n_meals;
	int				n_philo_eat;
	pthread_mutex_t	death_s;
}	t_shared;

typedef struct s_philo
{
	int				id;
	pthread_t		tid;
	long			last_eat;
	int				t_init;
	int				n_meals;
	pthread_mutex_t	*fork_s;
	pthread_mutex_t	*next_fork_s;
	t_shared		*shared;
}	t_philo;

// philo_utils
int		ft_atoi(char *num);
long	gettime(struct timeval *time, long origin);
void	ft_msleep(int m);

// philo_philos
t_philo *ft_set_philos(t_shared *shared);
int	ft_init_philos(t_philo *philos);
int	ft_wait_philos(t_philo *philos);

// philo_thread
void	*ft_routine(void *arg)
void	*ft_death_routine(void *arg)

#endif
