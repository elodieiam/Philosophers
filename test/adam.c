#include <pthread.h>
#include <stdio.h>

int	*f(void)
{
	static int var;
	return (&var); 
}

void	*test(void	*ptr)
{
	pthread_mutex_t	*mtx;

	mtx = (pthread_mutex_t *)ptr;
	if (mtx->__align)
		printf("locked, %li\n", mtx->__align);
	else
		printf("unlocked %li\n", mtx->__align);
	return (NULL);
}

void	*routine(void *ptr)
{
	pthread_mutex_t	*mtx;
	pthread_t	thread;

	mtx = (pthread_mutex_t *)ptr;
	pthread_create(&thread, NULL, test, &mtx);
	pthread_mutex_lock(mtx);
	pthread_join(thread, NULL);
	return (NULL);
}

int	main(void)
{
	pthread_t	thread;
	pthread_mutex_t	mtx;

	pthread_mutex_init(&mtx, NULL);
	pthread_mutex_lock(&mtx);
	// pthread_mutex_unlock(&mtx);
	pthread_create(&thread, NULL, routine, &mtx);
	pthread_join(thread, NULL);
	pthread_mutex_destroy(&mtx);
	return (0);
}