/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 11:42:05 by elrichar          #+#    #+#             */
/*   Updated: 2023/10/05 20:48:19 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_init(t_philo *philo)
{
	if (pthread_mutex_lock(philo->check_dead))
		return (write(2, "Error : mutex_lock issue\n", 25), 1);
	if (*(philo->init_check))
		return (1);
	if (pthread_mutex_unlock(philo->check_dead))
		return (write(2, "Error : mutex_lock issue\n", 25), 1);
	return (0);
}

int	print_messages(t_philo *philo, char *str)
{
	if (is_dead(philo))
		return (1);
	if (pthread_mutex_lock(philo->check_dead))
		return (write(2, "Error : mutex_lock issue\n", 25), 1);
	if (*(philo->status) == dead)
	{
		if (pthread_mutex_unlock(philo->check_dead))
			return (write(2, "Error : mutex_unlock issue\n", 27), 1);
		return (1);
	}
	if (pthread_mutex_lock(philo->write))
	{
		if (pthread_mutex_unlock(philo->check_dead))
			return (write(2, "Error : mutex_unlock issue\n", 27), 1);
		return (write(2, "Error : mutex_lock issue\n", 25), 1);
	}
	printf("%lld %d %s\n", (get_time() - philo->time) / 1000, philo->pos, str);
	if (pthread_mutex_unlock(philo->write))
		return (write(2, "Error : mutex_unlock issue\n", 27), 1);
	if (pthread_mutex_unlock(philo->check_dead))
		return (write(2, "Error : mutex_unlock issue\n", 27), 1);
	return (0);
}

void	free_and_destroy(char **av, t_philo *philos, pthread_mutex_t *forks)
{
	if (pthread_mutex_destroy((philos->check_dead)))
		write(2, "Error : mutex_destroy issue\n", 28);
	if (pthread_mutex_destroy((philos->write)))
		write(2, "Error : mutex_destroy issue\n", 28);
	free(philos);
	free_mutex(av, &forks);
}

void	free_mutex(char **av, pthread_mutex_t **forks)
{
	int	i;

	i = 0;
	while (i < ft_atoi(av[1]))
	{
		if (pthread_mutex_destroy(&((*forks)[i])))
			write(2, "Error : mutex_destroy issue\n", 28);
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
