/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 11:35:27 by elrichar          #+#    #+#             */
/*   Updated: 2023/10/05 16:25:48 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	my_usleep(t_philo *philo, long time)
{
	long long	current;
	long long	time_end;
	long long	time_of_death;

	current = get_time();
	time_end = get_time() + time;
	time_of_death = philo->last_meal + philo->time_die;
	while (current < time_end)
	{
		usleep(330);
		current = get_time();
		if (current > time_of_death)
		{
			if (is_dead(philo))
				return (1);
		}
	}
	return (0);
}

void	wait_after_thinking(int delay)
{
	long long	time_now;

	time_now = get_time();
	while (get_time() - time_now < delay)
		usleep(50);
}

int	sleeping(t_philo *philo)
{
	if (print_messages(philo, "is sleeping"))
		return (1);
	if (my_usleep(philo, philo->time_sleep))
		return (1);
	return (0);
}

int	think(t_philo *philo)
{
	if (print_messages(philo, "is thinking"))
		return (1);
	return (0);
}

int	eat(t_philo *philo)
{
	if (print_messages(philo, "is eating"))
	{
		drop_forks(philo);
		return (1);
	}
	philo->last_meal = get_time();
	if (philo->number_meals != -1)
		philo->meals_eaten += 1;
	if (my_usleep(philo, philo->time_eat))
	{
		drop_forks(philo);
		return (1);
	}
	drop_forks(philo);
	return (0);
}
