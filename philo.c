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
	long	o_time;
}	t_shared;

typedef struct s_philo
{
	int		id;
	pthread_t	tid;
	int		t_init;
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

	philo = (t_philo *) arg;
	philo->t_init = 1;
	printf("%ld ms: philo %d is created\n", gettime(&time, philo->shared->o_time), philo->id);
	while (philo->shared->start)
		;
	printf("%ld ms: philo %d started\n", gettime(&time, philo->shared->o_time), philo->id);
	return (NULL);
}

t_philo *ft_set_philos(t_shared *shared)
{
	int	i;
	t_philo	*philos;

	philos = (t_philo *)malloc(sizeof(t_philo) * shared->n_philos);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < shared->n_philos)
	{
		philos[i].id = i + 1;
		philos[i].t_init = 0;
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
		err = pthread_join(philos[i].tid, NULL);
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
	struct timeval	time;

	printf("Program started at time: %ld ms\n", gettime(&time, 0));
	if (argc != 2)
	{
		write(2, "Usage error\n", 12);
		return (1);
	}
	shared.n_philos = ft_atoi(argv[1]);
	philos = ft_set_philos(&shared);
	if (!philos)
	{
		write(2, "Memory error\n", 13);
		return (12);
	}
	shared.o_time = gettime(&time, 0);
	printf("All philos are defined: %ld ms\n", shared.o_time);
	shared.start = 1;
	if (ft_init_philos(philos))
		return (1);
	while (!philos[shared.n_philos - 1].t_init)
		;
	memset((void *) &shared.start, 0, sizeof(int));
	if (ft_wait_philos(philos))
		return (1);
	printf("All philos are terminated\n");
	return (0);
}
