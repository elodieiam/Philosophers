/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:34:41 by elrichar          #+#    #+#             */
/*   Updated: 2023/09/21 15:05:27 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*"substitut" de variable globale
A chaque fois qu'on appelle cette fonction, on obtient l'adresse de la variable
On peut donc accéder à la même variable partout pr voir sa valeur et/ou la modifier
Les variables booléennes sont initialisées à 'faux' par défaut 
Voir test2.c
*/

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

void	pick_forks(t_philo *philo)
{
	if (philo->pos % 2 == 0)
	{
		pthread_mutex_lock((philo->r_fork));
		print_messages(1, philo);
		pthread_mutex_lock((philo->l_fork));
		print_messages(1, philo);	
	}
	else
	{
		pthread_mutex_lock((philo->l_fork));
		print_messages(1, philo);
		pthread_mutex_lock((philo->r_fork));
		print_messages(1, philo);
	}
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

void	print_messages(int flag, t_philo *philo)
{
	struct timeval tv;
	
	long long	current_time;

	pthread_mutex_lock(philo->write);
	gettimeofday(&tv, NULL);
	
	current_time = ((tv.tv_sec * 1000000 + tv.tv_usec) / 1000);
	if (flag == 1)
		printf("%lld %d has taken a fork\n", current_time, philo->pos);
	else if (flag == 2)
		printf("%lld %d is thinking\n", current_time, philo->pos);
	else if (flag == 3)
		printf("%lld %d is eating\n", current_time, philo->pos);
	else if (flag == 4)
		printf("%lld %d is sleeping\n", current_time, philo->pos);
	pthread_mutex_unlock(philo->write);
}

void	eat(t_philo *philo)
{
	pick_forks(philo);
	philo->time_die = get_time() + philo->death_time;
	print_messages(2, philo);
	usleep((philo->time_eat) * 1000);
	philo->meals_eaten += 1;
	drop_forks(philo);
}

void	sleeping(t_philo *philo)
{
	print_messages(4, philo);
	usleep(philo->time_sleep);
}

void	think(t_philo *philo)
{
	print_messages(2, philo);
}

void	synchronize_launch(t_philo *philo)
{
	int	nb;
	
	nb = philo->nb_philo;
	usleep((150 * (nb - 1)) * 1000);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	synchronize_launch(philo);
	while (philo->status != 1)
	{
		eat(philo);
		sleeping(philo);
		think(philo);
	}
	return (NULL);
}

int	init_threads(t_philo *philos)
{
	int			nb;
	int			i;

	nb = philos->nb_philo;
	i = 0;
	while (i < nb)
	{
		if (pthread_create(&(philos[i].ID), NULL, routine, (void *)(&philos[i])) != 0) 
		//pthread_create(&(((*philos)[i].ID)), NULL, routine, (void *)&(*philos)[i]) != 0 avec t_philo **philo
			return (0); //+ trouver un moyen de free les ressources associées à chaque thread créé
		i++;
	}
	i = 0;
	while (i < nb)
	{
		pthread_join(philos[i].ID, NULL);
		i++;
	}
	return (1);
}


int main(int ac, char **av)
{
	pthread_mutex_t	*forks;
	t_philo	*philos;
	
	forks = NULL;
	philos = NULL;
	if (!check_args(ac, av))
		return (1);
	if (!init_variables(av, ac, &forks, &philos))
		return (1);
	if (!init_threads(philos))
		return (1); //+free tout ce qu'il faut (créer une struct pour garder tout ce qu'on doit free en mémoire ?)
	return (0);
}