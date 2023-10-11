/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:34:41 by elrichar          #+#    #+#             */
/*   Updated: 2023/10/05 20:58:22 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_dead(t_philo *philo)
{
	if (get_time() - philo->last_meal > philo->time_die)
	{
		if (pthread_mutex_lock(philo->check_dead))
			return (write(2, "Error : mutex_lock issue\n", 25), 1);
		if (!(*(philo->status)))
		{
			(*(philo->status)) = dead;
			if (pthread_mutex_lock(philo->write))
			{
				if (pthread_mutex_unlock(philo->check_dead))
					return (write(2, "Error : mutex_unlock issue\n", 27), 1);
				return (write(2, "Error : mutex_lock issue\n", 25), 1);
			}
			printf("%lld %d died\n", (get_time() - philo->time) \
				/ 1000, philo->pos);
			if (pthread_mutex_unlock(philo->write))
				return (write(2, "Error : mutex_unlock issue\n", 27), 1);
		}
		if (pthread_mutex_unlock(philo->check_dead))
			return (write(2, "Error : mutex_unlock issue\n", 27), 1);
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
	if (pthread_mutex_lock(philo->l_fork))
		return (write(2, "Error : mutex_lock issue\n", 25), NULL);
	if (print_messages(philo, "has taken a fork"))
	{
		if (pthread_mutex_unlock(philo->l_fork))
			return (write(2, "Error : mutex_unlock issue\n", 27), NULL);
		return (NULL);
	}
	usleep(philo->time_die + 1000);
	is_dead(philo);
	if (pthread_mutex_unlock(philo->l_fork))
		return (write(2, "Error : mutex_unlock issue\n", 27), NULL);
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
