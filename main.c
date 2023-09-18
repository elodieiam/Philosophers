/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:34:41 by elrichar          #+#    #+#             */
/*   Updated: 2023/09/18 21:19:44 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*"substitut" de variable globale
A chaque fois qu'on appelle cette fonction, on obtient l'adresse de la variable
On peut donc accéder à la même variable partout pr voir sa valeur et/ou la modifier
Les variables booléennes sont initialisées à 'faux' par défaut 
Voir test2.c
*/
bool	*stop(void)
{
	static bool	stop;
	return (&stop);
}

long long *start_time(void)
{
	static long long	starting_time; 
	return (&starting_time);
}

int	*f(void)
{
	static int var;
	return (&var); 
}

void	free_mutex(char **av, pthread_mutex_t **forks)
{
	int	i;

	i = 0;
	while (i < ft_atoi(av[1]))
	{
		pthread_mutex_destroy(&((*forks)[i]));
		i++;
	}
	free(*forks);
}

long long	get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000000 + tv.tv_usec) / 1000);
}




void	join_threads(t_philo **philos, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		if (pthread_join(((*philos)[i]).ID, NULL) != 0)
		{
			printf("Error : pthread_join issue.\n");
		}
		i++;
	}
}

void	set_time_start(void)
{
	long long	*start_tim;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	start_tim = start_time();
	*start_tim = (tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
}






int main(int ac, char **av)
{
	pthread_mutex_t	*forks;
	t_philo	*philos;
	
	forks = NULL;
	philos = NULL;
	if (!check_args(ac, av))
		return (1);
	if (!init_variables(av, &forks, &philos))
		return (1);
	return (0);
}