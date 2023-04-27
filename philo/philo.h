/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:37:12 by gpanico           #+#    #+#             */
/*   Updated: 2023/04/27 08:01:28 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PHILO_H
# define PHILO_H
# define FORK "%ld ms: %d has taken a fork\n"
# define EAT "%ld ms: %d is eating\n"
# define SLEEP "%ld ms: %d is sleeping\n"
# define THINK "%ld ms: %d is thinking\n"
# define DIE "%ld ms: %d died\n"
# include <pthread.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_shared
{
	int				start;
	int				args[5];
	int				death;
	long			o_time;
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

// philo
int		ft_init_shared(t_shared *shared);
void	ft_die(t_philo *philos);

// philo_utils
int		ft_atoi(char *num);
long	gettime(struct timeval *time, long origin);
void	ft_msleep(int m);

// philo_philos
int		ft_philo_set_var(t_philo *philo, t_shared *shared,
			pthread_mutex_t *fork_s, int i);
t_philo	*ft_set_philos(t_shared *shared);
int		ft_init_philos(t_philo *philos);
int		ft_wait_philos(t_philo *philos);

// philo_thread
int		ft_take_forks(t_philo *philo, struct timeval *time);
void	ft_cycle(t_philo *philo, struct timeval *time);
void	*ft_routine(void *arg);
void	*ft_death_routine(void *arg);

// philo_checks
int		ft_is_all_digit(int n, char **strs);
int		ft_checks(int argc, char **argv, t_shared *shared);

#endif
