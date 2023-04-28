/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 11:15:35 by gpanico           #+#    #+#             */
/*   Updated: 2023/04/28 10:02:31 by gpanico          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_kill_all(pid_t *pids, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		kill(pids[i], SIGKILL);
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

char	*ft_strdup(char *str)
{
	int	i;
	int	len;
	char	*dup;

	len = ft_strlen(str);
	dup = (char *)malloc(sizeof(char) * len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = 0;
	return (dup);
}

void	*ft_death_check(void *arg)
{
	t_shared	*shared;
	struct timeval	time;
	long		t;

	shared = (t_shared *) arg;
	sem_wait(shared->sem_child);
	t = gettime(&time, shared->last_eat);
	sem_post(shared->sem_child);
	while ( t < shared->args[1])
	{
		if (!shared->args[4])
		{
			sem_wait(shared->sem_death);
			shared->death = 1;
			sem_post(shared->sem_death);
			return (NULL);
		}
		sem_wait(shared->sem_child);
		t = gettime(&time, shared->last_eat);
		sem_post(shared->sem_child);
	}
	printf("%ld ms: %d died\n", gettime(&time, shared->o_time), shared->id);
	sem_wait(shared->sem_death);
	shared->death = 1;
	sem_post(shared->sem_death);
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_shared		shared;
	int				i;
	struct timeval	time;
	pthread_t		*deaths;
	sem_t			*start;

	if (ft_checks(argc, argv, &shared))
		return (2);
	shared.sem_name = ft_strdup("/sem");
	if (!shared.sem_name)
	{
		write(2, "Memory error.\n", 14);
		return (12);
	}
	sem_unlink(shared.sem_name);
	shared.death = 0;
	shared.pids = (pid_t *)malloc(sizeof(pid_t) * shared.args[0]);
	if (!shared.pids)
	{
		free(shared.sem_name);
		write(2, "Memory error.\n", 14);
		return (12);
	}
	deaths = (pthread_t *)malloc(sizeof(pthread_t) * shared.args[0]);
	if (!deaths)
	{
		free(shared.pids);
		free(shared.sem_name);
		write(2, "Memory error.\n", 14);
		return (12);
	}
	shared.sem = sem_open(shared.sem_name, O_CREAT, 0660, shared.args[0]);
	i = 0;
	sem_unlink("/start");
	start = sem_open("/start", O_CREAT, 0660, shared.args[0]);
	for (int n = 0; n < shared.args[0]; n++)
		sem_wait(start);
	shared.o_time = gettime(&time, 0);
	while (i < shared.args[0])
	{
		shared.pids[i] = fork();
		if (!shared.pids[i])
		{
			sem_wait(start);
			printf("--%ld ms\n", gettime(&time, shared.o_time));
			if (i % 2 == 0)
				ft_msleep(30);
			shared.id = i + 1;
			shared.last_eat = gettime(&time, 0);
			sem_unlink("/sem_child");
			sem_unlink("/sem_death");
			shared.sem_child = sem_open("/sem_child", O_CREAT, 0660, 1);
			shared.sem_death = sem_open("/sem_death", O_CREAT, 0660, 1);
			pthread_create(&deaths[i], NULL, ft_death_check, (void *) &shared);
			pthread_detach(deaths[i]);
			while (shared.args[4] != 0)
			{
				sem_wait(shared.sem);
				sem_wait(shared.sem_death);
				if (shared.death)
				{
					sem_post(shared.sem_death);
					exit(0);
				}
				sem_post(shared.sem_death);
				printf("%ld ms: %d has taken a fork\n", gettime(&time, shared.o_time), i + 1);
				sem_wait(shared.sem);
				sem_wait(shared.sem_death);
				if (shared.death)
				{
					sem_post(shared.sem_death);
					exit(0);
				}
				sem_post(shared.sem_death);
				printf("%ld ms: %d has taken a fork\n", gettime(&time, shared.o_time), i + 1);
				sem_wait(shared.sem_child);
				shared.last_eat = gettime(&time, 0);
				sem_post(shared.sem_child);
				if (shared.args[4] != -1)
					shared.args[4]--;
				sem_wait(shared.sem_death);
				if (shared.death)
				{
					sem_post(shared.sem_death);
					exit(0);
				}
				sem_post(shared.sem_death);
				printf("%ld ms: %d is eating\n", gettime(&time, shared.o_time), i + 1);
				ft_msleep(shared.args[2]);
				sem_post(shared.sem);
				sem_post(shared.sem);
				sem_wait(shared.sem_death);
				if (shared.death)
				{
					sem_post(shared.sem_death);
					exit(0);
				}
				sem_post(shared.sem_death);
				printf("%ld ms: %d is sleeping\n", gettime(&time, shared.o_time), i + 1);
				ft_msleep(shared.args[3]);
				sem_wait(shared.sem_death);
				if (shared.death)
				{
					sem_post(shared.sem_death);
					exit(0);
				}
				sem_post(shared.sem_death);
				printf("%ld ms: %d is thinking\n", gettime(&time, shared.o_time), i + 1);
			}
			exit(0);
		}
		i++;
	}
	for (int n = 0; n < shared.args[0]; n++)
		sem_post(start);
	if (shared.args[4] == -1)
	{
		waitpid(-1, NULL, 0);
		ft_kill_all(shared.pids, shared.args[0]);
	}
	else
		ft_wait_all(shared.pids, shared.args[0]);
	free(deaths);
	free(shared.pids);
	sem_close(shared.sem);
	sem_unlink(shared.sem_name);
	free(shared.sem_name);
}
