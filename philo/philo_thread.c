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

void	*ft_routine(void *arg)
{
	t_philo			*philo;
	struct timeval	time;
	int				forks;

	forks = 0;
	philo = (t_philo *) arg;
	philo->t_init = 1;
	while (philo->shared->start)
		;
	philo->last_eat = gettime(&time, 0);
	if (philo->id % 2 == 0)
		ft_msleep(30);
	while (philo->n_meals)
	{
		pthread_mutex_lock(philo->fork_s);
		if (philo->shared->death)
			break ;
		printf("%ld ms: %d has taken a fork\n", gettime(&time, philo->shared->o_time), philo->id);
		pthread_mutex_lock(philo->next_fork_s);
		if (philo->shared->death)
			break ;
		printf("%ld ms: %d has taken a fork\n", gettime(&time, philo->shared->o_time), philo->id);
		philo->last_eat = gettime(&time, 0);
		philo->n_meals--;
		if (philo->shared->death)
			break ;
		printf("%ld ms: %d is eating\n", gettime(&time, philo->shared->o_time), philo->id);
		ft_msleep(philo->shared->args[2]);
		pthread_mutex_unlock(philo->fork_s);
		pthread_mutex_unlock(philo->next_fork_s);
		if (philo->shared->death)
			break ;
		printf("%ld ms: %d is sleeping\n", gettime(&time, philo->shared->o_time), philo->id);
		ft_msleep(philo->shared->args[3]);
		if (philo->shared->death)
			break ;
		printf("%ld ms: %d is thinking\n", gettime(&time, philo->shared->o_time), philo->id);
	}
	philo->shared->n_philo_eat++;
	return (NULL);
}

void	*ft_death_routine(void *arg)
{
	t_philo			*philos;
	struct timeval	time;
	int				i;

	philos = (t_philo *) arg; 
	while (philos[0].shared->start)
		;
	i = 0;
	while (!philos[0].shared->death)
	{
		if (gettime(&time, philos[i].last_eat) >= philos[0].shared->args[1])
		{
			printf("%ld ms: %d died\n", gettime(&time, philos[i].shared->o_time), i + 1);
			philos[i].shared->death = 1;
			return (NULL);
		}
		if (philos[i].shared->n_philo_eat == philos[i].shared->args[0])
			return (NULL);
		if (i == philos[0].shared->args[0] - 1)
			i = -1;
		i++;
		usleep(100);
	}
	return (NULL);
}
