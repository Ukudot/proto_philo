/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:10:59 by gpanico           #+#    #+#             */
/*   Updated: 2023/04/26 15:12:13 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_cycle(t_philo *philo, struct timeval *time)
{
	while (philo->n_meals)
	{
		pthread_mutex_lock(philo->fork_s);
		if (philo->shared->death)
			break ;
		printf(FORK, gettime(time, philo->shared->o_time), philo->id);
		pthread_mutex_lock(philo->next_fork_s);
		if (philo->shared->death)
			break ;
		printf(FORK, gettime(time, philo->shared->o_time), philo->id);
		philo->last_eat = gettime(time, 0);
		if (philo->n_meals-- && philo->shared->death)
			break ;
		printf(EAT, gettime(time, philo->shared->o_time), philo->id);
		ft_msleep(philo->shared->args[2]);
		pthread_mutex_unlock(philo->fork_s);
		pthread_mutex_unlock(philo->next_fork_s);
		if (philo->shared->death)
			break ;
		printf(SLEEP, gettime(time, philo->shared->o_time), philo->id);
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
	philo->t_init = 1;
	while (philo->shared->start)
		;
	philo->last_eat = gettime(&time, 0);
	if (philo->id % 2 == 0)
		ft_msleep(30);
	ft_cycle(philo, &time);
	philo->shared->n_philo_eat++;
	philo->n_meals = -1;
	ft_cycle(philo, &time);
	return (NULL);
}

void	*ft_death_routine(void *arg)
{
	t_philo			*p;
	struct timeval	time;
	int				i;

	p = (t_philo *) arg;
	while (p[0].shared->start)
		;
	i = 0;
	while (!p[0].shared->death)
	{
		if (gettime(&time, p[i].last_eat) >= p[0].shared->args[1])
		{
			printf(DIE, gettime(&time, p[i].shared->o_time), i + 1);
			p[i].shared->death = 1;
			return (NULL);
		}
		if (p[i].shared->n_philo_eat == p[i].shared->args[0])
			return (NULL);
		if (i == p[0].shared->args[0] - 1)
			i = -1;
		i++;
		usleep(50);
	}
	return (NULL);
}
