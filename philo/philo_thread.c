/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:10:59 by gpanico           #+#    #+#             */
/*   Updated: 2023/04/28 08:54:51 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_take_forks(t_philo *philo, struct timeval *time)
{
	pthread_mutex_lock(philo->fork_s);
	if (philo->shared->death)
		return (1);
	printf(FORK, gettime(time, philo->shared->o_time), philo->id);
	pthread_mutex_lock(philo->next_fork_s);
	if (philo->shared->death)
		return (1);
	printf(FORK, gettime(time, philo->shared->o_time), philo->id);
	return (0);
}

int	ft_put_forks(t_philo *philo, struct timeval *time)
{
	pthread_mutex_unlock(philo->fork_s);
	pthread_mutex_unlock(philo->next_fork_s);
	if (philo->shared->death)
		return (1);
	printf(SLEEP, gettime(time, philo->shared->o_time), philo->id);
	return (0);
}

void	ft_cycle(t_philo *philo, struct timeval *time)
{
	while (philo->n_meals)
	{
		if (ft_take_forks(philo, time))
			break ;
		pthread_mutex_lock(philo->lock_t_eat);
		philo->last_eat = gettime(time, 0);
		pthread_mutex_unlock(philo->lock_t_eat);
		if (--philo->n_meals == 0)
		{
			pthread_mutex_lock(&philo->shared->live_s);
			philo->shared->n_philo_eat++;
			pthread_mutex_unlock(&philo->shared->live_s);
		}
		if (philo->shared->death)
			break ;
		printf(EAT, gettime(time, philo->shared->o_time), philo->id);
		ft_msleep(philo->shared->args[2]);
		if (ft_put_forks(philo, time))
			break ;
		ft_msleep(philo->shared->args[3]);
		if (philo->shared->death)
			break ;
		printf(THINK, gettime(time, philo->shared->o_time), philo->id);
	}
}

void	*ft_routine(void *arg)
{
	t_philo			*philo;
	struct timeval	time;

	philo = (t_philo *) arg;
	pthread_mutex_lock(&philo->shared->live_s);
	philo->t_init = 1;
	pthread_mutex_unlock(&philo->shared->live_s);
	while (philo->shared->start)
		;
	pthread_mutex_lock(philo->lock_t_eat);
	philo->last_eat = gettime(&time, 0);
	pthread_mutex_unlock(philo->lock_t_eat);
	if (philo->id % 2 == 0)
		ft_msleep(30);
	ft_cycle(philo, &time);
	return (NULL);
}

void	*ft_death_routine(void *arg)
{
	t_philo			*p;
	int				n_philo_eat;
	int				i;

	p = (t_philo *) arg;
	while (p[0].shared->start)
		;
	ft_msleep(50);
	i = 0;
	while (!p[0].shared->death)
	{
		if (ft_check_death(p, i))
			return (NULL);
		pthread_mutex_lock(&p[0].shared->live_s);
		n_philo_eat = p[0].shared->n_philo_eat;
		pthread_mutex_unlock(&p[0].shared->live_s);
		if (n_philo_eat == p[0].shared->args[0])
			return (NULL);
		if (i == p[0].shared->args[0] - 1)
			i = -1;
		i++;
	}
	return (NULL);
}
