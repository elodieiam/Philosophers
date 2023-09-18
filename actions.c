/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 21:14:57 by elrichar          #+#    #+#             */
/*   Updated: 2023/09/18 21:15:52 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_action(t_philo *philo, int indicator)
{
	struct timeval tv;
	long long	*start_tim;
	long long	current_time;

	gettimeofday(&tv, NULL);
	start_tim = start_time();
	current_time = ((tv.tv_sec * 1000000 + tv.tv_usec) / 1000) - *(start_tim);
	if (indicator == 1)
		printf("%lld %d has taken a fork\n", current_time, philo->pos);
	else if (indicator == 2)
		printf("%lld %d is thinking\n", current_time, philo->pos);
	else if (indicator == 3)
		printf("%lld %d is eating\n", current_time, philo->pos);
	else if (indicator == 4)
		printf("%lld %d is sleeping\n", current_time, philo->pos);
}

void	pick_fork(t_philo *philo)
{
	if (philo->pos % 2 == 0)
	{
		pthread_mutex_lock((philo->r_fork));
		pthread_mutex_lock((philo->l_fork));
		pthread_mutex_lock(philo->check);
		print_action(philo, 1);
		pthread_mutex_unlock((philo->r_fork));
		pthread_mutex_unlock((philo->l_fork));
		pthread_mutex_unlock(philo->check);
		
	}
	else
	{
		pthread_mutex_lock((philo->l_fork));
		pthread_mutex_lock((philo->r_fork));
		pthread_mutex_lock(philo->check);
		print_action(philo, 1);
		pthread_mutex_unlock((philo->l_fork));
		pthread_mutex_unlock((philo->r_fork));
		pthread_mutex_unlock(philo->check);
	}
}

void	think(t_philo *philo)
{
	pthread_mutex_lock((philo->check));
	print_action(philo, 2);
	pthread_mutex_unlock((philo->check));
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock((philo->check));
	print_action(philo, 3);
	philo->time_die = get_time() + philo->time_die;
	pthread_mutex_unlock((philo->check));
}

void	sleeping(t_philo *philo)
{
	pthread_mutex_lock((philo->check));
	print_action(philo, 4);
	usleep(philo->time_sleep);
	pthread_mutex_unlock((philo->check));
}