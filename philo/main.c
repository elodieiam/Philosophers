/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 11:45:30 by elrichar          #+#    #+#             */
/*   Updated: 2023/10/05 12:02:58 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	join_threads(t_philo *philos, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		if (pthread_join((philos[i]).thread_id, NULL) != 0)
			printf("Error : pthread_join issue.\n");
		i++;
	}
}

int	init_threads(t_philo *philos)
{
	int			nb;
	int			i;

	nb = philos->nb_philo;
	i = 0;
	while (i < nb)
	{
		if (pthread_create(&(philos[i].thread_id), 0, routine, (void *)(&philos[i])))
		{
			*(philos->init_check) = 1;
			printf("Error : pthread_create failed\n");
			while (--i >= 0)
			{
				if (pthread_join(philos[i].thread_id, NULL) != 0)
					printf("Error : pthread_join failed\n");
			}
			return (0);
		}
		i++;
	}
	join_threads(philos, nb);
	return (1);
}

int	main(int ac, char **av)
{
	pthread_mutex_t	*forks;
	t_philo			*philos;

	forks = NULL;
	philos = NULL;
	if (!check_args(ac, av))
		return (1);
	if (!init_variables(av, ac, &forks, &philos))
	{
		free(philos);
		free(forks);
		return (1);
	}
	if (!init_threads(philos))
	{
		free_and_destroy(av, philos, forks);
		return (1);
	}
	free_and_destroy(av, philos, forks);
	return (0);
}