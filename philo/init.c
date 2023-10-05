/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 21:16:57 by elrichar          #+#    #+#             */
/*   Updated: 2023/10/05 12:00:03 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_data_philos(char **av, int ac, t_philo **philos)
{
	int				i;
	int				nb;
	static t_bool	status;
	static int		init_check;
	long long		time;

	i = -1;
	nb = ft_atoi(av[1]);
	status = alive;
	init_check = 0;
	time = get_time();
	while (++i < nb)
	{
		set_common_vars(av, i, philos);
		if (ac == 6)
			(*philos)[i].number_meals = ft_atoi(av[5]);
		else
			(*philos)[i].number_meals = (-1);
		(*philos)[i].status = &status;
		(*philos)[i].time = time;
		(*philos)[i].init_check = &init_check;
		set_sync(nb, i, philos);
	}
	return (1);
}

int	init_mutex_philos(int nb, t_philo **philos, pthread_mutex_t **forks)
{
	int						i;
	static pthread_mutex_t	lock_philo;
	static pthread_mutex_t	write;

	i = 0;
	if (pthread_mutex_init(&lock_philo, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&write, NULL) != 0)
	{
		pthread_mutex_destroy(&lock_philo);
		return (0);
	}
	while (i < nb)
	{
		set_forks(i, nb, philos, forks);
		(*philos)[i].lock_philo = &lock_philo;
		(*philos)[i].write = &write;
		i++;
	}
	return (1);
}

int	init_philos(char **av, int ac, t_philo **philos, pthread_mutex_t **forks)
{
	int	nb;
	int	i;

	nb = ft_atoi(av[1]);
	i = 0;
	*philos = malloc(sizeof(t_philo) * nb);
	if (!*philos)
		return (0);
	if (!init_mutex_philos(nb, philos, forks))
		return (0);
	if (!init_data_philos(av, ac, philos))
	{
		free_mutex(av, forks);
		while (i < nb)
		{
			pthread_mutex_destroy((*philos)[i].lock_philo);
			pthread_mutex_destroy((*philos)[i].write);
			i++;
		}
		return (0);
	}
	return (1);
}

int	init_forks(char **av, pthread_mutex_t **forks)
{
	int	nb;
	int	i;
	int	j;

	nb = ft_atoi(av[1]);
	i = 0;
	j = 0;
	*forks = malloc(sizeof(pthread_mutex_t) * nb);
	if (!*forks)
		return (0);
	while (i < nb)
	{
		if (pthread_mutex_init((&((*forks)[i])), NULL) != 0)
		{
			while (j < i)
			{
				pthread_mutex_destroy(&((*forks)[j]));
				j++;
			}
			free (*forks);
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_variables(char **av, int ac, pthread_mutex_t **forks, t_philo **philos)
{
	if (!init_forks(av, forks))
		return (0);
	if (!init_philos(av, ac, philos, forks))
	{
		free_mutex(av, forks);
		return (0);
	}
	return (1);
}
