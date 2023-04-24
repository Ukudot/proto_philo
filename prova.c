#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t	lock;

void	*ft_routine(void *arg)
{
	int	*id;

	id = (int *)arg;
	printf("Thread %d is started\n", *id);
	pthread_mutex_lock(&lock);
	printf("Critical section\n");
	sleep(1);
	pthread_mutex_unlock(&lock);
	printf("Thread %d is completed\n", *id);
	return ((void *) NULL);
}

int	main(void)
{
	int		n;
	int		t_1;
	int		t_2;
	pthread_t	tid_1;
	pthread_t	tid_2;

	n = 0;
	t_1 = 1;
	t_2 = 2;
	while (n < 2)
	{
		if (n == 0)
			pthread_create(&tid_1, NULL, ft_routine ,(void *) &t_1);
		else
			pthread_create(&tid_2, NULL, ft_routine ,(void *) &t_2);
		n++;
	}
	pthread_join(tid_1, NULL);
	pthread_join(tid_2, NULL);
	printf("Threads terminated\n");
	return (0);
}
