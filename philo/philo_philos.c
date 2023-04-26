/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_philos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:07:47 by gpanico           #+#    #+#             */
/*   Updated: 2023/04/26 14:53:40 by gpanico          ###   ########.fr       */
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

	philos = (t_philo *)malloc(sizeof(t_philo) * sh->args[0]);
	if (!philos)
	{
		pthread_mutex_destroy(&sh->death_s);
		return (NULL);
	}
	fork_s = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * sh->args[0]);
	if (!fork_s)
	{
		pthread_mutex_destroy(&sh->death_s);
		free(philos);
		return (NULL);
	}
	i = 0;
	while (i < sh->args[0])
	{
		if (ft_philo_set_var(&philos[i], sh, fork_s, i))
			return (NULL);
		i++;
	}
	return (philos);
}

int	ft_init_philos(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos[0].shared->args[0])
	{
		if (pthread_create(&philos[i].tid, NULL,
				ft_routine, (void *) &philos[i]))
		{
			while (i--)
				pthread_detach(philos[i].tid);
			while (i < philos[0].shared->args[0])
			{
				pthread_mutex_destroy(philos[i].fork_s);
				i++;
			}
			pthread_mutex_destroy(&philos[0].shared->death_s);
			free(philos[0].fork_s);
			free(philos);
			printf("Error occurred\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_wait_philos(t_philo *philos)
{
	int	err;
	int	i;

	i = 0;
	while (i < philos[0].shared->args[0])
	{
		err = pthread_detach(philos[i].tid);
		if (err)
		{
			free(philos);
			printf("Error occurred\n");
			return (1);
		}
		i++;
	}
	return (0);
}
