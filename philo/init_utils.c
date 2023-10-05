/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 11:30:46 by elrichar          #+#    #+#             */
/*   Updated: 2023/10/05 12:40:08 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	set_common_vars(char **av, int i, t_philo **philos)
{
	(*philos)[i].thread_id = 0;
	(*philos)[i].nb_philo = ft_atoi(av[1]);
	(*philos)[i].time_die = ft_atoi(av[2]) * 1000;
	(*philos)[i].time_eat = ft_atoi(av[3]) * 1000;
	(*philos)[i].time_sleep = ft_atoi(av[4]) * 1000;
	(*philos)[i].meals_eaten = 0;
	(*philos)[i].last_meal = 0;
	(*philos)[i].pos = i + 1;
}

void	set_forks(int i, int nb, t_philo **philos, pthread_mutex_t **forks)
{
	if (i == 0 && nb == 1)
	{
		(*philos)[i].l_fork = (*forks);
		(*philos)[i].r_fork = NULL;
	}
	if (i == 0)
	{
		(*philos)[i].l_fork = (*forks);
		(*philos)[i].r_fork = (*forks) + (nb - 1);
	}
	else
	{
		(*philos)[i].l_fork = (*forks) + i;
		(*philos)[i].r_fork = (*forks) + (i - 1);
	}
}

void	set_sync(int nb, int i, t_philo **philos)
{
	if (nb == 3)
	{
		if (i % 2 == 0)
			(*philos)[i].sync = 500;
		else
			(*philos)[i].sync = 1000;
	}
	else if (nb % 2 != 0 && i % 2 == 0)
		(*philos)[i].sync = 1000;
	else if (nb % 2 == 0)
		(*philos)[i].sync = 500;
	else
		(*philos)[i].sync = 0;
	set_sync_2(nb, i, philos);
}

void	set_sync_2(int nb, int i, t_philo **philos)
{
	if (nb % 2 != 0)
	{
		if ((*philos)[i].time_eat * 3 <= (*philos)[i].time_die \
			&& (*philos)[i].time_sleep < (*philos)[i].time_eat)
			(*philos)[i].sync += (*philos)[i].time_eat - \
			(*philos)[i].time_sleep;
		else if ((*philos)[i].time_eat * 3 > (*philos)[i].time_die \
			&& ((*philos)[i].time_sleep + (*philos)[i].time_eat) \
			< (*philos)[i].time_die)
			(*philos)[i].sync += (*philos)[i].time_die - \
			((*philos)[i].time_eat + (*philos)[i].time_sleep);
	}
	else
	{
		if ((*philos)[i].time_eat * 2 > (*philos)[i].time_die \
			&& ((*philos)[i].time_sleep + (*philos)[i].time_eat) \
			< (*philos)[i].time_die)
			(*philos)[i].sync += (*philos)[i].time_die - \
			((*philos)[i].time_eat + (*philos)[i].time_sleep);
	}
}
