/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_death_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 09:56:22 by gpanico           #+#    #+#             */
/*   Updated: 2023/05/02 09:56:24 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_kill_all(pid_t *pids, int n, long o_time)
{
	int	ret;
	int	i;

	i = 0;
	while (i < n)
	{
		ret = kill(pids[i], SIGKILL);
		if (ret == -1)
			printf("%ld ms: %d died\n", gettime(o_time), i + 1);
		i++;
	}
}

void	ft_wait_all(pid_t *pids, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
}

void	ft_update_status(t_philo *philo)
{
	sem_wait(philo->lock);
	philo->last_eat = gettime(0);
	philo->args[4] -= (philo->args[4] != -1);
	sem_post(philo->lock);
}

void	*ft_status(void	*arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	while (42)
	{
		sem_wait(philo->lock);
		if (gettime(philo->last_eat) >= philo->args[1])
		{
			philo->death = 1;
			sem_post(philo->lock);
			exit(1);
		}
		sem_post(philo->lock);
		usleep(100);
	}
	return (NULL);
}

void	ft_die(t_philo *philo)
{
	int	signal;
	int	i;

	i = 0;
	while (i < philo->args[0])
	{
		waitpid(-1, &signal, 0);
		if (!WEXITSTATUS(signal))
		{
			i++;
			continue ;
		}
		else
		{
			ft_kill_all(philo->pids, philo->args[0], philo->o_time);
			break ;
		}
	}
}
