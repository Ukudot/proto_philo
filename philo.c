/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:06:33 by gpanico           #+#    #+#             */
/*   Updated: 2023/04/24 16:24:48 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	t_philo			*philos;
	t_shared		shared;
	pthread_t		death;
	struct timeval	time;
	int				n;

	if (argc != 5 && argc != 6)
	{
		write(2, "Usage error\n", 12);
		return (1);
	}
	shared.n_philos = ft_atoi(argv[1]);
	shared.time_to_die = ft_atoi(argv[2]);
	shared.time_to_eat = ft_atoi(argv[3]);
	shared.time_to_sleep = ft_atoi(argv[4]);
	shared.n_philo_eat = 0;
	shared.death = 0;
	if (argc == 6)
		shared.n_meals = ft_atoi(argv[5]);
	else
		shared.n_meals = -1;
	pthread_mutex_init(&shared.death_s, NULL);
	philos = ft_set_philos(&shared);
	if (!philos)
	{
		write(2, "Memory error\n", 13);
		return (12);
	}
	shared.o_time = gettime(&time, 0);
	printf("All philos are defined: %ld ms\n", shared.o_time);
	pthread_create(&death, NULL, ft_death_routine, (void *) &shared);
	shared.start = 1;
	if (ft_init_philos(philos))
		return (1);
	while (!philos[shared.n_philos - 1].t_init)
		;
	memset((void *) &shared.start, 0, sizeof(int));
	if (ft_wait_philos(philos))
		return (1);
	pthread_join(death, NULL);
	printf("All philos are terminated\n");
	n = 0;
	while (n < shared.n_philos)
	{
		pthread_mutex_destroy(philos[n].fork_s);
		n++;
	}
	pthread_mutex_destroy(&shared.death_s);
	free(philos[0].fork_s);
	free(philos);
	return (0);
}
