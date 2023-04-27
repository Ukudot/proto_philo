/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:06:33 by gpanico           #+#    #+#             */
/*   Updated: 2023/04/27 07:50:11 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_init_shared(t_shared *shared)
{
	struct timeval	time;

	shared->o_time = gettime(&time, 0);
	shared->n_philo_eat = 0;
	shared->death = 0;
	if (pthread_mutex_init(&shared->death_s, NULL))
		return (1);
	shared->start = 1;
	return (0);
}

void	ft_die(t_philo *philos)
{
	int	n;

	n = 0;
	while (n < philos[0].shared->args[0])
	{
		pthread_mutex_destroy(philos[n].fork_s);
		n++;
	}
	pthread_mutex_destroy(&philos[0].shared->death_s);
	free(philos[0].fork_s);
	free(philos);
}

int	main(int argc, char *argv[])
{
	t_philo			*philos;
	t_shared		shared;
	pthread_t		death;

	if (ft_checks(argc, argv, &shared))
		return (2);
	if (ft_init_shared(&shared))
		return (1);
	philos = ft_set_philos(&shared);
	if (!philos)
		return (write(2, "Memory error\n", 13) - 1);
	if (ft_init_philos(philos))
		return (1);
	while (!philos[shared.args[0] - 1].t_init)
		;
	pthread_create(&death, NULL, ft_death_routine, (void *) philos);
	memset((void *) &shared.start, 0, sizeof(int));
	if (ft_wait_philos(philos))
		return (1);
	pthread_join(death, NULL);
	ft_die(philos);
	return (0);
}
