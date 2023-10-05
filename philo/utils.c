/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 11:42:05 by elrichar          #+#    #+#             */
/*   Updated: 2023/10/05 12:40:28 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_init(t_philo *philo)
{
	pthread_mutex_lock(philo->lock_philo);
	if (*(philo->init_check))
		return (1);
	pthread_mutex_unlock(philo->lock_philo);
	return (0);
}

int	print_messages(t_philo *philo, char *str)
{
	if (is_dead(philo))
		return (1);
	pthread_mutex_lock(philo->lock_philo);
	if (*(philo->status) == dead)
	{
		pthread_mutex_unlock(philo->lock_philo);
		return (1);
	}
	pthread_mutex_lock(philo->write);
	printf("%lld %d %s\n", (get_time() - philo->time) / 1000, philo->pos, str);
	pthread_mutex_unlock(philo->write);
	pthread_mutex_unlock(philo->lock_philo);
	return (0);
}

void	free_and_destroy(char **av, t_philo *philos, pthread_mutex_t *forks)
{
	pthread_mutex_destroy((philos->lock_philo));
	pthread_mutex_destroy((philos->write));
	free(philos);
	free_mutex(av, &forks);
}

void	free_mutex(char **av, pthread_mutex_t **forks)
{
	int	i;

	i = 0;
	while (i < ft_atoi(av[1]))
	{
		pthread_mutex_destroy(&((*forks)[i]));
		i++;
	}
	free(*forks);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000000) + (tv.tv_usec / 1000) * 1000);
}
