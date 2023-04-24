#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_shared
{
	int				start;
	int				n_philos;
	int				death;
	long			o_time;
	int				time_to_eat;
	int				time_to_die;
	int				time_to_sleep;
	int				n_meals;
	int				n_philo_eat;
	pthread_mutex_t	death_s;
}	t_shared;

typedef struct s_philo
{
	int				id;
	pthread_t		tid;
	long			last_eat;
	int				t_init;
	int				n_meals;
	pthread_mutex_t	*fork_s;
	pthread_mutex_t	*next_fork_s;
	t_shared		*shared;
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

void	ft_msleep(int m)
{
	struct timeval	time;
	long			t;

	t = gettime(&time, 0);
	while (m > gettime(&time, t))
		usleep(100);
}

void	*ft_routine(void *arg)
{
	t_philo			*philo;
	struct timeval	time;
	int				forks;

	forks = 0;
	philo = (t_philo *) arg;
	philo->t_init = 1;
	printf("%ld ms: philo %d is created\n", gettime(&time, philo->shared->o_time), philo->id);
	while (philo->shared->start)
		;
	printf("%ld ms: philo %d started\n", gettime(&time, philo->shared->o_time), philo->id);
	philo->last_eat = gettime(&time, 0);
	if (philo->id % 2 == 0)
		usleep(30);
	while (philo->n_meals)
	{
		pthread_mutex_lock(philo->fork_s);
		if (gettime(&time, philo->last_eat) > philo->shared->time_to_die || philo->shared->death)
		{
			pthread_mutex_lock(&philo->shared->death_s);
			if (!philo->shared->death)
			{
				usleep(10);
				printf("%ld ms: %d died\n", gettime(&time, philo->shared->o_time), philo->id);
			}
			philo->shared->death = 1;
			pthread_mutex_lock(&philo->shared->death_s);
			break ;
		}
		printf("%ld ms: %d has taken a fork\n", gettime(&time, philo->shared->o_time), philo->id);
		pthread_mutex_lock(philo->next_fork_s);
		if (gettime(&time, philo->last_eat) > philo->shared->time_to_die || philo->shared->death)
		{
			pthread_mutex_lock(&philo->shared->death_s);
			if (!philo->shared->death)
			{
				usleep(10);
				printf("%ld ms: %d died\n", gettime(&time, philo->shared->o_time), philo->id);
			}
			philo->shared->death = 1;
			pthread_mutex_lock(&philo->shared->death_s);
			break ;
		}
		printf("%ld ms: %d has taken a fork\n", gettime(&time, philo->shared->o_time), philo->id);
		philo->last_eat = gettime(&time, 0);
		philo->n_meals--;
		if (gettime(&time, philo->last_eat) > philo->shared->time_to_die || philo->shared->death)
		{
			pthread_mutex_lock(&philo->shared->death_s);
			if (!philo->shared->death)
			{
				usleep(10);
				printf("%ld ms: %d died\n", gettime(&time, philo->shared->o_time), philo->id);
			}
			philo->shared->death = 1;
			pthread_mutex_lock(&philo->shared->death_s);
			break ;
		}
		printf("%ld ms: %d is eating\n", gettime(&time, philo->shared->o_time), philo->id);
		ft_msleep(philo->shared->time_to_eat);
		pthread_mutex_unlock(philo->fork_s);
		pthread_mutex_unlock(philo->next_fork_s);
		if (gettime(&time, philo->last_eat) > philo->shared->time_to_die || philo->shared->death)
		{
			pthread_mutex_lock(&philo->shared->death_s);
			if (!philo->shared->death)
			{
				usleep(10);
				printf("%ld ms: %d died\n", gettime(&time, philo->shared->o_time), philo->id);
			}
			philo->shared->death = 1;
			pthread_mutex_lock(&philo->shared->death_s);
			break ;
		}
		printf("%ld ms: %d is sleeping\n", gettime(&time, philo->shared->o_time), philo->id);
		ft_msleep(philo->shared->time_to_sleep);
		if (gettime(&time, philo->last_eat) > philo->shared->time_to_die || philo->shared->death)
		{
			pthread_mutex_lock(&philo->shared->death_s);
			if (!philo->shared->death)
			{
				usleep(10);
				printf("%ld ms: %d died\n", gettime(&time, philo->shared->o_time), philo->id);
			}
			philo->shared->death = 1;
			pthread_mutex_lock(&philo->shared->death_s);
			break ;
		}
		printf("%ld ms: %d is thinking\n", gettime(&time, philo->shared->o_time), philo->id);
	}
	philo->shared->n_philo_eat++;
	return (NULL);
}

void	*ft_death_routine(void *arg)
{
	t_shared	*shared;

	shared = (t_shared *) arg;
	while (!shared->death)
	{
		if (shared->n_philo_eat == shared->n_philos)
			return (NULL);
	}
	printf("DIED\n");
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
		pthread_mutex_init(&fork_s[i], NULL);
		philos[i].n_meals = shared->n_meals;
		philos[i].fork_s = &fork_s[i];
		if (i == shared->n_philos - 1)
			philos[i].next_fork_s = &fork_s[0];
		else
			philos[i].next_fork_s = &fork_s[i + 1];
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
		err = pthread_detach(philos[i].tid);
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
	if (argc != 5 && argc != 6)
	{
		write(2, "Usage error\n", 12);
		return (1);
	}
	shared.n_philos = ft_atoi(argv[1]);
	shared.time_to_die = ft_atoi(argv[2]);
	shared.time_to_eat = ft_atoi(argv[3]);
	shared.time_to_sleep = ft_atoi(argv[4]);
	shared.n_philo_eat = 0;
	shared.death = 0;
	if (argc == 6)
		shared.n_meals = ft_atoi(argv[5]);
	else
		shared.n_meals = -1;
	pthread_mutex_init(&shared.death_s, NULL);
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
		pthread_mutex_destroy(philos[n].fork_s);
		n++;
	}
	pthread_mutex_destroy(&shared.death_s);
	free(philos[0].fork_s);
	free(philos);
	return (0);
}
