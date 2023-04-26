/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpanico <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 11:15:35 by gpanico           #+#    #+#             */
/*   Updated: 2023/04/26 11:52:26 by gpanico          ###   ########.fr       */
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

int	main(int argc, char *argv[])
{
	t_shared		shared;
	int				i;
	pid_t			*pids;
	struct timeval	time;
	long			last_eat;

	if (ft_checks(argc, argv, &shared))
		return (2);
	if (shared.args[0] == 1)
	{
		printf("0 ms: 1 has taken a fork\n");
		ft_msleep(shared.args[1]);
		printf("%d ms: 1 died\n", shared.args[1]);
		return (0);
	}
	shared.sem_name = ft_strdup("/sem");
	if (!shared.sem_name)
	{
		write(2, "Memory error.\n", 14);
		return (12);
	}
	sem_unlink(shared.sem_name);
	shared.death = 0;
	pids = (pid_t *)malloc(sizeof(pid_t) * shared.args[0]);
	if (!pids)
	{
		free(shared.sem_name);
		write(2, "Memory error.\n", 14);
		return (12);
	}
	shared.sem = sem_open(shared.sem_name, O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, shared.args[0]);
	i = 0;
	shared.o_time = gettime(&time, 0);
	while (i < shared.args[0])
	{
		pids[i] = fork();
		if (!pids[i])
		{
			last_eat = gettime(&time, 0);
			if (i % 2 == 0)
				usleep(300);
			while (shared.args[4] != 0)
			{
				sem_wait(shared.sem);
				if (gettime(&time, last_eat) > shared.args[1] || shared.death)
				{
					if (!shared.death)
					{
						usleep(30);
						printf("%ld ms: %d died\n", gettime(&time, shared.o_time), i + 1);
					}
					exit(0);
				}
				printf("%ld ms: %d has taken a fork\n", gettime(&time, shared.o_time), i + 1);
				sem_wait(shared.sem);
				if (gettime(&time, last_eat) > shared.args[1] || shared.death)
				{
					if (!shared.death)
					{
						usleep(30);
						printf("%ld ms: %d died\n", gettime(&time, shared.o_time), i + 1);
					}
					exit(0);
				}
				printf("%ld ms: %d has taken a fork\n", gettime(&time, shared.o_time), i + 1);
				last_eat = gettime(&time, 0);
				if (shared.args[4] != -1)
					shared.args[4]--;
				printf("%ld ms: %d is eating\n", gettime(&time, shared.o_time), i + 1);
				ft_msleep(shared.args[2]);
				sem_post(shared.sem);
				sem_post(shared.sem);
				if (gettime(&time, last_eat) > shared.args[1] || shared.death)
				{
					if (!shared.death)
					{
						usleep(30);
						printf("%ld ms: %d died\n", gettime(&time, shared.o_time), i + 1);
					}
					exit(0);
				}
				printf("%ld ms: %d is sleeping\n", gettime(&time, shared.o_time), i + 1);
				ft_msleep(shared.args[3]);
				if (gettime(&time, last_eat) > shared.args[1] || shared.death)
				{
					if (!shared.death)
					{
						usleep(30);
						printf("%ld ms: %d died\n", gettime(&time, shared.o_time), i + 1);
					}
					exit(0);
				}
				printf("%ld ms: %d is thinking\n", gettime(&time, shared.o_time), i + 1);
			}
			exit(0);
		}
		i++;
	}
	if (shared.args[4] == -1)
	{
		waitpid(-1, NULL, 0);
		ft_kill_all(pids, shared.args[0]);
	}
	else
		ft_wait_all(pids, shared.args[0]);
	free(pids);
	sem_close(shared.sem);
	sem_unlink(shared.sem_name);
	free(shared.sem_name);
}
