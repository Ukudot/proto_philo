#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <pthread.h>

typedef struct s_shared
{
	int		args[5];
	int		death;
	long	o_time;
	char	*sem_name;
	sem_t	*sem;
}	t_shared;

#endif
