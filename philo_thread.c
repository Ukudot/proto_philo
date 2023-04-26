/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:10:59 by gpanico           #+#    #+#             */
/*   Updated: 2023/04/24 16:11:10 by gpanico          ###   ########.fr       */
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
	printf("%ld ms: philo %d is created\n", gettime(&time, philo->shared->o_time), philo->id);
	while (philo->shared->start)
		;
	printf("%ld ms: philo %d started\n", gettime(&time, philo->shared->o_time), philo->id);
	philo->last_eat = gettime(&time, 0);
	if (philo->id % 2 == 0)
		ft_msleep(30);
	while (philo->n_meals)
	{
		pthread_mutex_lock(philo->fork_s);
		if (gettime(&time, philo->last_eat) > philo->shared->time_to_die || philo->shared->death)
		{
			pthread_mutex_lock(&philo->shared->death_s);
			if (!philo->shared->death)
			{
				usleep(10);
				printf("%ld ms: %d died\n", gettime(&time, philo->shared->o_time), philo->id);
			}
			philo->shared->death = 1;
			pthread_mutex_lock(&philo->shared->death_s);
			break ;
		}
		printf("%ld ms: %d has taken a fork\n", gettime(&time, philo->shared->o_time), philo->id);
		pthread_mutex_lock(philo->next_fork_s);
		if (gettime(&time, philo->last_eat) > philo->shared->time_to_die || philo->shared->death)
		{
			pthread_mutex_lock(&philo->shared->death_s);
			if (!philo->shared->death)
			{
				usleep(10);
				printf("%ld ms: %d died\n", gettime(&time, philo->shared->o_time), philo->id);
			}
			philo->shared->death = 1;
			pthread_mutex_lock(&philo->shared->death_s);
			break ;
		}
		printf("%ld ms: %d has taken a fork\n", gettime(&time, philo->shared->o_time), philo->id);
		philo->last_eat = gettime(&time, 0);
		philo->n_meals--;
		if (gettime(&time, philo->last_eat) > philo->shared->time_to_die || philo->shared->death)
		{
			pthread_mutex_lock(&philo->shared->death_s);
			if (!philo->shared->death)
			{
				usleep(10);
				printf("%ld ms: %d died\n", gettime(&time, philo->shared->o_time), philo->id);
			}
			philo->shared->death = 1;
			pthread_mutex_lock(&philo->shared->death_s);
			break ;
		}
		printf("%ld ms: %d is eating\n", gettime(&time, philo->shared->o_time), philo->id);
		ft_msleep(philo->shared->time_to_eat);
		pthread_mutex_unlock(philo->fork_s);
		pthread_mutex_unlock(philo->next_fork_s);
		if (gettime(&time, philo->last_eat) > philo->shared->time_to_die || philo->shared->death)
		{
			pthread_mutex_lock(&philo->shared->death_s);
			if (!philo->shared->death)
			{
				usleep(10);
				printf("%ld ms: %d died\n", gettime(&time, philo->shared->o_time), philo->id);
			}
			philo->shared->death = 1;
			pthread_mutex_lock(&philo->shared->death_s);
			break ;
		}
		printf("%ld ms: %d is sleeping\n", gettime(&time, philo->shared->o_time), philo->id);
		ft_msleep(philo->shared->time_to_sleep);
		if (gettime(&time, philo->last_eat) > philo->shared->time_to_die || philo->shared->death)
		{
			pthread_mutex_lock(&philo->shared->death_s);
			if (!philo->shared->death)
			{
				usleep(10);
				printf("%ld ms: %d died\n", gettime(&time, philo->shared->o_time), philo->id);
			}
			philo->shared->death = 1;
			pthread_mutex_lock(&philo->shared->death_s);
			break ;
		}
		printf("%ld ms: %d is thinking\n", gettime(&time, philo->shared->o_time), philo->id);
	}
	philo->shared->n_philo_eat++;
	return (NULL);
}

void	*ft_death_routine(void *arg)
{
	t_shared	*shared;

	shared = (t_shared *) arg;
	while (!shared->death)
	{
		if (shared->n_philo_eat == shared->n_philos)
			return (NULL);
		usleep(100);
	}
	printf("DIED\n");
	return (NULL);
}
