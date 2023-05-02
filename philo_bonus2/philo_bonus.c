/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 11:15:35 by gpanico           #+#    #+#             */
/*   Updated: 2023/05/02 09:57:00 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

char	*ft_sem_name(int id)
{
	char	*name;
	int		i;

	name = (char *) malloc(sizeof(char) * 10);
	i = 0;
	while (id + 31 > 126 && i < 9)
	{
		name[i] = id % 96 + 31;
		id = id - 95;
		i++;
	}
	name[i] = id + 31;
	while (++i < 10)
		name[i] = 0;
	return (name);
}

int	main(int argc, char *argv[])
{
	t_philo	philo;

	if (ft_checks(argc, argv, &philo))
		return (2);
	if (philo.args[0] > 200)
		return (write(2, "Too many philosophers\n", 23));
	sem_unlink("/forks");
	philo.forks = sem_open("/forks", O_CREAT, 4278, philo.args[0]);
	philo.pids = (pid_t *) malloc(sizeof(pid_t) * philo.args[0]);
	if (!philo.pids)
	{
		sem_close(philo.forks);
		sem_unlink("/forks");
		return (12);
	}
	ft_start_philos(&philo);
	ft_die(&philo);
	return (0);
}
