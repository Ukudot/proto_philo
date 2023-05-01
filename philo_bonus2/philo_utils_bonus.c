/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:04:11 by gpanico           #+#    #+#             */
/*   Updated: 2023/05/01 15:35:32 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_strlen(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	if (!s1 || !s2)
		return (512);
	i = 0;
	while (s1[i] && s1[i] - s2[i] == 0)
		i++;
	return (s1[i] - s2[i]);
}

int	ft_atoi(char *num)
{
	int	i;
	int	n;

	if (ft_strlen(num) >= 10 && ft_strcmp(num, "2147483647") > 0)
		return (-1);
	i = 0;
	n = 0;
	while (num[i])
	{
		n *= 10;
		n += (num[i] - 48);
		i++;
	}
	return (n);
}

long	gettime(long origin)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((long) time.tv_sec * 1000 + time.tv_usec / 1000 - origin);
}

void	ft_msleep(int m)
{
	long			t;

	t = gettime(0);
	while (m > gettime(t))
		usleep(100);
}
