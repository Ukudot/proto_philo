#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_shared
{
	int	start;
	int	n_philos;
	int	death;
	long	o_time;
	int	time_to_eat;
	int	time_to_die;
	int	time_to_sleep;
}	t_shared;

typedef struct s_philo
{
	int		id;
	pthread_t	tid;
	long		last_eat;
	int		t_init;
	int		fork;
	pthread_mutex_t	*fork_s;
	int		*next_fork;
	pthread_mutex_t	*next_fork_s;
	t_shared	*shared;
}	t_philo;

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

void	*ft_routine(void *arg)
{
	t_philo		*philo;
	struct timeval	time;
	int		forks;

	forks = 0;
	philo = (t_philo *) arg;
	philo->t_init = 1;
	printf("%ld ms: philo %d is created\n", gettime(&time, philo->shared->o_time), philo->id);
	while (philo->shared->start)
		;
	printf("%ld ms: philo %d started\n", gettime(&time, philo->shared->o_time), philo->id);
	philo->last_eat = gettime(&time, 0);
	while (true)
	{
		pthread_mutex_lock(philo->fork_s);
		if (philo->fork == 1)
			forks += (philo->fork)--;
		pthread_mutex_unlock(philo->fork_s);
		pthread_mutex_lock(philo->next_fork_s);
		if (*(philo->next_fork) == 1)
			forks += (*(philo->next_fork))--;
		pthread_mutex_unlock(philo->fork_s);
		if (forks == 2)
		{
			//reset time_eat
			//eat
			//put_down_forks
			//sleep
		}
		if (gettime(&time, philo->last_eat) > philo->shared->time_to_die || philo->shared->death)
		{
			philo->shared->death = 1;
			break ;
		}
	}
	return (NULL);
}

void	*ft_death_routine(void *arg)
{
	t_shared	*shared;

	shared = (t_shared *) arg;
	while (!shared->death)
		;
	return (NULL);
}

t_philo *ft_set_philos(t_shared *shared)
{
	int		i;
	t_philo		*philos;
	pthread_mutex_t	*fork_s;

	philos = (t_philo *)malloc(sizeof(t_philo) * shared->n_philos);
	if (!philos)
		return (NULL);
	fork_s = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * shared->n_philos);
	if (!fork_s)
	{
		free(philos);
		return (NULL);
	}
	i = 0;
	while (i < shared->n_philos)
	{
		philos[i].id = i + 1;
		philos[i].t_init = 0;
		philos[i].fork = 1;
		philos[i].fork_s = &fork_s[i];
		if (i == shared->n_philos - 1)
		{
			philos[i].next_fork_s = philos[0].fork_s;
			philos[i].next_fork = &philos[0].fork;
		}
		else
		{
			philos[i].next_fork_s = philos[i + 1].fork_s;
			philos[i].next_fork = &philos[i + 1].fork;
		}
		philos[i].shared = shared;
		i++;
	}
	return (philos);
}

int	ft_init_philos(t_philo *philos)
{
	int	err;
	int	i;

	i = 0;
	while (i < philos[0].shared->n_philos)
	{
		err = pthread_create(&philos[i].tid, NULL, ft_routine, (void *) &philos[i]);
		if (err)
		{
			free(philos);
			printf("Error occurred\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_wait_philos(t_philo *philos)
{
	int	err;
	int	i;

	i = 0;
	while (i < philos[0].shared->n_philos)
	{
		err = pthread_detach(philos[i].tid, NULL);
		if (err)
		{
			free(philos);
			printf("Error occurred\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_philo		*philos;
	t_shared	shared;
	pthread_t	death;
	struct timeval	time;
	int		n;

	printf("Program started at time: %ld ms\n", gettime(&time, 0));
	if (argc != 2)
	{
		write(2, "Usage error\n", 12);
		return (1);
	}
	shared.n_philos = ft_atoi(argv[1]);
	shared.death = 0;
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
		free(philos[n].s_fork);
		n++;
	}
	free(philos);
	return (0);
}
