/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_philos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:07:47 by gpanico           #+#    #+#             */
/*   Updated: 2023/05/01 11:27:37 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_philo_set_var(t_philo *philo, t_shared *shared,
		pthread_mutex_t *fork_s, int i)
{
	int	n;

	philo->id = i + 1;
	philo->t_init = 0;
	philo->n_meals = shared->args[4];
	philo->shared = shared;
	if (pthread_mutex_init(&fork_s[i], NULL))
	{
		n = 0;
		while (n < i)
		{
			pthread_mutex_destroy(&fork_s[n]);
			n++;
		}
		pthread_mutex_destroy(&shared->live_s);
		pthread_mutex_destroy(&shared->death_s);
		free(philo - i);
		return (1);
	}
	philo->fork_s = &fork_s[i];
	if (i == shared->args[0] - 1)
		philo->next_fork_s = &fork_s[0];
	else
		philo->next_fork_s = &fork_s[i + 1];
	return (0);
}

t_philo	*ft_set_philos(t_shared *sh)
{
	int				i;
	t_philo			*philos;
	pthread_mutex_t	*fork_s;
	pthread_mutex_t	*lock;

	philos = (t_philo *)malloc(sizeof(t_philo) * sh->args[0]);
	if (!philos)
		return (ft_destroy(NULL, NULL, sh));
	fork_s = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * sh->args[0]);
	if (!fork_s)
		return (ft_destroy(philos, NULL, sh));
	lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * sh->args[0]);
	if (!fork_s)
		return (ft_destroy(philos, fork_s, sh));
	i = 0;
	while (i < sh->args[0])
	{
		if (ft_philo_set_var(&philos[i], sh, fork_s, i))
			return (NULL);
		pthread_mutex_init(&lock[i], NULL);
		philos[i].lock_t_eat = &lock[i];
		i++;
	}
	return (philos);
}

int	ft_init_philos(t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < philos[0].shared->args[0])
	{
		if (pthread_create(&philos[i].tid, NULL,
				ft_routine, (void *) &philos[i]))
		{
			while (i--)
				pthread_detach(philos[i].tid);
			while (i < philos[0].shared->args[0])
			{
				pthread_mutex_destroy(philos[i].lock_t_eat);
				pthread_mutex_destroy(philos[i].fork_s);
				i++;
			}
			pthread_mutex_destroy(&philos[0].shared->live_s);
			pthread_mutex_destroy(&philos[0].shared->death_s);
			free(philos[0].fork_s);
			free(philos[0].lock_t_eat);
			free(philos);
			return (printf("Error occurred\n"));
		}
	}
	return (0);
}

void	ft_wait_philos(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos[0].shared->args[0])
	{
		pthread_detach(philos[i].tid);
		i++;
	}
}

void	*ft_destroy(t_philo *philos, pthread_mutex_t *fork_s, t_shared *shared)
{
	pthread_mutex_destroy(&shared->live_s);
	pthread_mutex_destroy(&shared->death_s);
	free(fork_s);
	free(philos);
	return (NULL);
}
