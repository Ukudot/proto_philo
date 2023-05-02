/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 10:18:46 by gpanico           #+#    #+#             */
/*   Updated: 2023/05/02 10:18:48 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <pthread.h>
# include <signal.h>
# include <sys/time.h>

typedef struct s_philo
{
	int			args[5];
	int			id;
	int			death;
	long		o_time;
	long		last_eat;
	pid_t		*pids;
	pthread_t	life;
	sem_t		*forks;
	char		*lock_name;
	sem_t		*lock;
}	t_philo;

// philo_bonus
char	*ft_sem_name(int id);

// philo_death_bonus
void	ft_kill_all(pid_t *pids, int n, long o_time);
void	ft_wait_all(pid_t *pids, int n);
void	ft_update_status(t_philo *philo);
void	*ft_status(void	*arg);
void	ft_die(t_philo *philo);

// philo_philos_bonus
void	ft_philo(t_philo philo);
void	ft_start_philos(t_philo *philo);
int		ft_takes_forks(t_philo *philo);
int		ft_eat(t_philo *philo);

// philo_utils_bonus
int		ft_strlen(char *str);
int		ft_strcmp(char *s1, char *s2);
int		ft_atoi(char *num);
long	gettime(long origin);
void	ft_msleep(int m);

// philo_checks_bonus
int		ft_is_all_digit(int n, char **strs);
int		ft_checks(int argc, char **argv, t_philo *philo);

#endif
