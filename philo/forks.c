/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 11:32:52 by elrichar          #+#    #+#             */
/*   Updated: 2023/10/05 12:12:39 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	pick_forks_even(t_philo *philo)
{
	if (pthread_mutex_lock((philo->l_fork)))
		printf("error\n");
	if (print_messages(philo, "has taken a fork"))
	{
		pthread_mutex_unlock((philo->l_fork));
		return (1);
	}
	if (pthread_mutex_lock((philo->r_fork)))
		printf("error\n");
	if (print_messages(philo, "has taken a fork"))
	{
		drop_forks(philo);
		return (1);
	}
	return (0);
}

int	pick_forks_odd(t_philo *philo)
{
	if (pthread_mutex_lock((philo->r_fork)))
		printf("error\n");
	if (print_messages(philo, "has taken a fork"))
	{
		pthread_mutex_unlock((philo->r_fork));
		return (1);
	}
	if (pthread_mutex_lock((philo->l_fork)))
		printf("error\n");
	if (print_messages(philo, "has taken a fork"))
	{
		drop_forks(philo);
		return (1);
	}
	return (0);
}

int	pick_forks(t_philo *philo)
{
	if (philo->pos % 2 == 0)
	{
		if (pick_forks_even(philo))
			return (1);
	}
	else
	{
		if (pick_forks_odd(philo))
			return (1);
	}
	return (0);
}

void	drop_forks(t_philo *philo)
{
	if (philo->pos % 2 == 0)
	{
		pthread_mutex_unlock((philo->r_fork));
		pthread_mutex_unlock((philo->l_fork));
	}
	else
	{
		pthread_mutex_unlock((philo->l_fork));
		pthread_mutex_unlock((philo->r_fork));
	}
}