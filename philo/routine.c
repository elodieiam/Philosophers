/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:34:41 by elrichar          #+#    #+#             */
/*   Updated: 2023/10/05 12:20:14 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_dead(t_philo *philo)
{
	long long	time;

	time = get_time() - philo->last_meal;
	if (time > philo->time_die)
	{
		pthread_mutex_lock(philo->lock_philo);
		if (!(*(philo->status)))
		{
			(*(philo->status)) = dead;
			pthread_mutex_lock(philo->write);
			printf("%lld %d died\n", (get_time() - philo->time) \
				/ 1000, philo->pos);
			pthread_mutex_unlock(philo->write);
		}
		pthread_mutex_unlock(philo->lock_philo);
		return (1);
	}
	return (0);
}

int	are_fed(t_philo *philo)
{
	if (philo->number_meals == (-1))
		return (0);
	if (philo->meals_eaten == philo->number_meals)
		return (1);
	return (0);
}

void	*case_one(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	if (print_messages(philo, "has taken a fork"))
	{
		pthread_mutex_unlock(philo->l_fork);
		return (NULL);
	}
	usleep(philo->time_die + 500);
	is_dead(philo);
	pthread_mutex_unlock(philo->l_fork);
	return (NULL);
}

void	before_departure(t_philo *philo)
{
	philo->last_meal = 0;
	philo->time = philo->time + (philo->nb_philo * 1000);
	while (get_time() < philo->time)
		usleep(1000);
	philo->last_meal = get_time();
	print_messages(philo, "is thinking");
	if (philo->nb_philo % 2 == 0 && philo->pos % 2 != 0)
		usleep(philo->time_eat / 2);
	else if (philo->nb_philo % 2 != 0 && philo->pos % 2 == 0)
		usleep(philo->time_eat / 2);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	before_departure(philo);
	if (philo->nb_philo == 1)
		return (case_one(philo));
	while (!is_dead(philo))
	{
		if (pick_forks(philo))
			return (NULL);
		if (eat(philo))
			return (NULL);
		if (are_fed(philo))
			return (NULL);
		if (sleeping(philo))
			return (NULL);
		if (think(philo))
			return (NULL);
		wait_after_thinking(philo->sync);
	}
	return (NULL);
}

