/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_philos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:07:47 by gpanico           #+#    #+#             */
/*   Updated: 2023/04/26 11:07:14 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo *ft_set_philos(t_shared *shared)
{
	int				i;
	t_philo			*philos;
	pthread_mutex_t	*fork_s;

	philos = (t_philo *)malloc(sizeof(t_philo) * shared->args[0]);
	if (!philos)
		return (NULL);
	fork_s = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * shared->args[0]);
	if (!fork_s)
	{
		free(philos);
		return (NULL);
	}
	i = 0;
	while (i < shared->args[0])
	{
		philos[i].id = i + 1;
		philos[i].t_init = 0;
		pthread_mutex_init(&fork_s[i], NULL);
		philos[i].n_meals = shared->args[4];
		philos[i].fork_s = &fork_s[i];
		if (i == shared->args[0] - 1)
			philos[i].next_fork_s = &fork_s[0];
		else
			philos[i].next_fork_s = &fork_s[i + 1];
		philos[i].shared = shared;
		i++;
	}
	return (philos);
}

int	ft_init_philos(t_philo *philos)
{
	int	err;
	int	i;

	i = 0;
	while (i < philos[0].shared->args[0])
	{
		err = pthread_create(&philos[i].tid, NULL, ft_routine, (void *) &philos[i]);
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
