/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 08:52:52 by gpanico           #+#    #+#             */
/*   Updated: 2023/04/28 08:53:04 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_check_death(t_philo *p, int i)
{
	long			last_eat;
	struct timeval	time;

	pthread_mutex_lock(p[i].lock_t_eat);
	last_eat = gettime(&time, p[i].last_eat);
	pthread_mutex_unlock(p[i].lock_t_eat);
	if (last_eat >= p[0].shared->args[1])
	{
		printf(DIE, gettime(&time, p[i].shared->o_time), i + 1);
		p[i].shared->death = 1;
		return (1);
	}
	return (0);
}
