/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:04:11 by gpanico           #+#    #+#             */
/*   Updated: 2023/04/24 16:04:18 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(char *num)
{
	int	n;
	int	i;

	i = 0;
	n = 0;
	while (num[i])
	{
		n *= 10;
		n += num[i] - 48;
		i++;
	}
	return (n);
}

long	gettime(struct timeval *time, long origin)
{
	gettimeofday(time, NULL);
	return ((long) time->tv_sec * 1000 + time->tv_usec / 1000 - origin);
}

void	ft_msleep(int m)
{
	struct timeval	time;
	long			t;

	t = gettime(&time, 0);
	while (m > gettime(&time, t))
		usleep(100);
}
