/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 11:15:35 by gpanico           #+#    #+#             */
/*   Updated: 2023/05/01 16:12:01 by gpanico          ###   ########.fr       */
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

int	ft_takes_forks(t_philo *philo)
{
	sem_wait(philo->forks);
	sem_wait(philo->lock);
	if (philo->death)
	{
		sem_post(philo->lock);
		sem_post(philo->forks);
		return (1);
	}
	sem_post(philo->lock);
	printf("%ld ms: %i has taken a fork.\n", gettime(philo->o_time), philo->id);
	sem_wait(philo->forks);
	sem_wait(philo->lock);
	if (philo->death)
	{
		sem_post(philo->lock);
		sem_post(philo->forks);
		return (1);
	}
	sem_post(philo->lock);
	printf("%ld ms: %i has taken a fork.\n", gettime(philo->o_time), philo->id);
	return (0);
}

int	ft_eat(t_philo *philo)
{
	sem_wait(philo->lock);
	if (philo->death)
	{
		sem_post(philo->lock);
		return (1);
	}
	sem_post(philo->lock);
	printf("%ld ms: %i is eating.\n", gettime(philo->o_time), philo->id);
	ft_msleep(philo->args[2]);
	sem_wait(philo->lock);
	if (philo->death)
	{
		sem_post(philo->lock);
		return (1);
	}
	sem_post(philo->lock);
	sem_post(philo->forks);
	sem_post(philo->forks);
	printf("%ld ms: %i is sleeping.\n", gettime(philo->o_time), philo->id);
	ft_msleep(philo->args[3]);
	sem_wait(philo->lock);
	if (philo->death)
	{
		sem_post(philo->lock);
		return (1);
	}
	sem_post(philo->lock);
	return (0);
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
	//int		n_meals;

	philo = (t_philo *) arg;
	//sem_wait(philo->lock);
	//n_meals = philo->args[5];
	//m_post(philo->lock);
	while (42)//n_meals)
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
		//sem_wait(philo->lock);
		//n_meals = philo->args[5];
		//sem_post(philo->lock);
	}
	return (NULL);
}

void	ft_philo(t_philo philo)
{
	sem_unlink(philo.lock_name);
	philo.lock = sem_open(philo.lock_name, O_CREAT, 0660, 1);
	pthread_create(&philo.life, NULL, ft_status, (void *) &philo);
	pthread_detach(philo.life);
	if (philo.id % 2 == 0)
		ft_msleep(10);
	while (philo.args[4])
	{
		if (ft_takes_forks(&philo))
			exit(1);
		ft_update_status(&philo);
		if (ft_eat(&philo))
			exit(1);
		printf("%ld ms: %i is thinking.\n", gettime(philo.o_time), philo.id);
	}
	sem_close(philo.forks);
	sem_close(philo.lock);
	sem_unlink(philo.lock_name);
	free(philo.lock_name);
	exit(0);
}

void	ft_start_philos(t_philo *philo)
{
	int	i;

	i = 0;
	philo->death = 0;
	philo->o_time = gettime(0);
	philo->last_eat = philo->o_time;
	while (i < philo->args[0])
	{
		philo->id = i + 1;
		philo->lock_name = ft_sem_name(philo->id);
		philo->pids[i] = fork();
		if (!philo->pids[i])
			ft_philo(*philo);
		free(philo->lock_name);
		i++;
	}
	sem_close(philo->forks);
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

int	main(int argc, char *argv[])
{
	t_philo	philo;

	if (ft_checks(argc, argv, &philo))
		return (2);
	if (philo.args[0] > 200)
		return (write(2, "Too many philosophers\n", 23));
	// set semaphore
	sem_unlink("/forks");
	philo.forks = sem_open("/forks", O_CREAT, 0660, philo.args[0]);
	// allocate a pids array
	philo.pids = (pid_t *) malloc(sizeof(pid_t) * philo.args[0]);
	if (!philo.pids)
	{
		sem_close(philo.forks);
		sem_unlink("/forks");
		return (12);
	}
	// launch each philo
	ft_start_philos(&philo);
	// wait for philosophers
	ft_die(&philo);
	return (0);
}
