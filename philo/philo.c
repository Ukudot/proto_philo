/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:06:33 by gpanico           #+#    #+#             */
/*   Updated: 2023/04/26 11:03:49 by gpanico          ###   ########.fr       */
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

	if (ft_checks(argc, argv, &shared))
		return (2);
	shared.death = 0;
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
	while (!philos[shared.args[0] - 1].t_init)
		;
	memset((void *) &shared.start, 0, sizeof(int));
	if (ft_wait_philos(philos))
		return (1);
	pthread_join(death, NULL);
	printf("All philos are terminated\n");
	n = 0;
	while (n < shared.args[0])
	{
		pthread_mutex_destroy(philos[n].fork_s);
		n++;
	}
	pthread_mutex_destroy(&shared.death_s);
	free(philos[0].fork_s);
	free(philos);
	return (0);
}
