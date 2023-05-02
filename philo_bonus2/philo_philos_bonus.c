/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_philos_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 09:52:28 by gpanico           #+#    #+#             */
/*   Updated: 2023/05/02 09:52:44 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_philo(t_philo philo)
{
	sem_unlink(philo.lock_name);
	philo.lock = sem_open(philo.lock_name, O_CREAT, 4278, 1);
	pthread_create(&philo.life, NULL, ft_status, (void *) &philo);
	pthread_detach(philo.life);
	if (philo.id % 2 == 0)
		ft_msleep(10);
	while (philo.args[4])
	{
		if (ft_takes_forks(&philo))
			exit(1);
		ft_update_status(&philo);
		if (ft_eat(&philo))
			exit(1);
		printf("%ld ms: %i is thinking.\n", gettime(philo.o_time), philo.id);
		usleep(100);
	}
	sem_close(philo.forks);
	sem_close(philo.lock);
	sem_unlink(philo.lock_name);
	free(philo.lock_name);
	exit(0);
}

void	ft_start_philos(t_philo *philo)
{
	int	i;

	i = 0;
	philo->death = 0;
	philo->o_time = gettime(0);
	philo->last_eat = philo->o_time;
	while (i < philo->args[0])
	{
		philo->id = i + 1;
		philo->lock_name = ft_sem_name(philo->id);
		philo->pids[i] = fork();
		if (!philo->pids[i])
			ft_philo(*philo);
		free(philo->lock_name);
		i++;
	}
	sem_close(philo->forks);
}

int	ft_takes_forks(t_philo *philo)
{
	sem_wait(philo->forks);
	sem_wait(philo->lock);
	if (philo->death)
	{
		sem_post(philo->lock);
		sem_post(philo->forks);
		return (1);
	}
	sem_post(philo->lock);
	printf("%ld ms: %i has taken a fork.\n", gettime(philo->o_time), philo->id);
	sem_wait(philo->forks);
	sem_wait(philo->lock);
	if (philo->death)
	{
		sem_post(philo->lock);
		sem_post(philo->forks);
		return (1);
	}
	sem_post(philo->lock);
	printf("%ld ms: %i has taken a fork.\n", gettime(philo->o_time), philo->id);
	return (0);
}

int	ft_eat(t_philo *philo)
{
	sem_wait(philo->lock);
	if (philo->death)
		return (1);
	sem_post(philo->lock);
	printf("%ld ms: %i is eating.\n", gettime(philo->o_time), philo->id);
	ft_msleep(philo->args[2]);
	sem_wait(philo->lock);
	if (philo->death)
		return (1);
	sem_post(philo->lock);
	sem_post(philo->forks);
	sem_post(philo->forks);
	printf("%ld ms: %i is sleeping.\n", gettime(philo->o_time), philo->id);
	ft_msleep(philo->args[3]);
	sem_wait(philo->lock);
	if (philo->death)
		return (1);
	sem_post(philo->lock);
	return (0);
}
