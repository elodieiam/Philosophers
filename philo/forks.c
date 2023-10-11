/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 11:32:52 by elrichar          #+#    #+#             */
/*   Updated: 2023/10/05 20:25:33 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	pick_forks_even(t_philo *philo)
{
	if (pthread_mutex_lock((philo->l_fork)))
		return (write(2, "Error : mutex_lock issue\n", 25), 1);
	if (print_messages(philo, "has taken a fork"))
	{
		if (pthread_mutex_unlock((philo->l_fork)))
			return (write(2, "Error : mutex_unlock issue\n", 27), 1);
		return (1);
	}
	if (pthread_mutex_lock((philo->r_fork)))
	{
		if (pthread_mutex_unlock((philo->l_fork)))
			return (write(2, "Error : mutex_unlock issue\n", 27), 1);
		return (write(2, "Error : mutex_lock issue\n", 25), 1);
	}
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
		return (write(2, "Error : mutex_lock issue\n", 25), 1);
	if (print_messages(philo, "has taken a fork"))
	{
		if (pthread_mutex_unlock((philo->r_fork)))
			return (write(2, "Error : mutex_unlock issue\n", 27), 1);
		return (1);
	}
	if (pthread_mutex_lock((philo->l_fork)))
	{
		if (pthread_mutex_unlock((philo->r_fork)))
			return (write(2, "Error : mutex_unlock issue\n", 27), 1);
		return (write(2, "Error : mutex_lock issue\n", 25), 1);
	}
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

int	drop_forks(t_philo *philo)
{
	if (philo->pos % 2 == 0)
	{
		if (pthread_mutex_unlock((philo->r_fork)))
			return (write(2, "Error : mutex_unlock issue\n", 27), 1);
		if (pthread_mutex_unlock((philo->l_fork)))
			return (write(2, "Error : mutex_unlock issue\n", 27), 1);
	}
	else
	{
		if (pthread_mutex_unlock((philo->l_fork)))
			return (write(2, "Error : mutex_unlock issue\n", 27), 1);
		if (pthread_mutex_unlock((philo->r_fork)))
			return (write(2, "Error : mutex_unlock issue\n", 27), 1);
	}
	return (0);
}
