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
# include <signal.h>
# include <sys/time.h>

typedef struct s_shared
{
	int		args[5];
	int		death;
	long	o_time;
	char	*sem_name;
	sem_t	*sem;
}	t_shared;

// philo_utils_bonus
int		ft_strlen(char *str);
int		ft_strcmp(char *s1, char *s2);
int		ft_atoi(char *num);
long	gettime(struct timeval *time, long origin);
void	ft_msleep(int m);

// philo_checks_bonus
int		ft_is_all_digit(int n, char **strs);
int		ft_checks(int argc, char **argv, t_shared *shared);

#endif
