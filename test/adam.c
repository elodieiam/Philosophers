#include <pthread.h>
#include <stdio.h>

void	*routine(void *ptr)
{
	pthread_mutex_t	*mtx;

	mtx = (pthread_mutex_t *)ptr;
	if (mtx->__align)
		printf("locked\n");
	else
		printf("unlocked\n");
	return (NULL);
}

int	main(void)
{
	pthread_t	thread;
	pthread_mutex_t	mtx;

	pthread_mutex_init(&mtx, NULL);
	pthread_mutex_lock(&mtx);
	pthread_mutex_unlock(&mtx);
	pthread_create(&thread, NULL, routine, &mtx);
	pthread_join(thread, NULL);
	pthread_mutex_destroy(&mtx);
	return (0);
}