/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:34:41 by elrichar          #+#    #+#             */
/*   Updated: 2023/09/22 17:13:13 by elrichar         ###   ########.fr       */
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
	//return ((tv.tv_sec * 1000000 + ((tv.tv_usec / 1000) * 1000)));
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
		if (pthread_mutex_lock((philo->r_fork)))
			printf("error\n");
		print_messages(philo, "has taken a fork\n");
		if (pthread_mutex_lock((philo->l_fork)))
			printf("error\n");
		print_messages(philo, "has taken a fork\n");	
	}
	else
	{
		if (pthread_mutex_lock((philo->l_fork)))
			printf("error\n");
		print_messages(philo, "has taken a fork\n");
		if (pthread_mutex_lock((philo->r_fork)))
			printf("error\n");
		print_messages(philo, "has taken a fork\n");
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

void	print_messages(t_philo *philo, char *str)
{
	long long	current_time;
	long long	time;

	//pthread_mutex_t mutex;
	//pthread_mutex_init(&mutex, NULL);
	//pthread_mutex_lock(philo->write);
	current_time = get_time();
	time = current_time - (philo->time);
		printf("%lld %d %s\n", time, philo->pos, str);
	//pthread_mutex_unlock(philo->write);
	//pthread_mutex_destroy(&mutex);
}

void	ft_sleep(t_philo *philo)
{
	long long	time;
	long long	current_time;
	long long	arrival_time;
	
	time = philo->time;
	current_time = get_time();
	arrival_time = current_time - time + philo->time_sleep;
	//tant que le moment où on en est dans le prog est < au moment où on doit en être avant que le philo finisse de manger
	while ((current_time - time) < (arrival_time))
	{
		//printf("%lld\n", arrival_time);
		usleep(5000); //50 micro = 5ms
		current_time = get_time();
	}
	if ((current_time - time) >= (current_time - time + philo->time_sleep))
	{
		*(philo->status) = 1;
		return ;
	}
}

void	eat(t_philo *philo)
{
	pick_forks(philo);
	philo->time_die = get_time() + philo->death_time;
	print_messages(philo, "is eating\n");
	usleep(philo->time_eat * 1000); //recoder usleep car pas aasez precis, faire plein de miniusleep
	philo->meals_eaten += 1;
	drop_forks(philo);
}

void	sleeping(t_philo *philo)
{
	print_messages(philo, "is sleeping\n");
	ft_sleep(philo);
}

void	think(t_philo *philo)
{
	print_messages(philo, "is thinking\n");
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
	philo->time = get_time();
	//*(philo->status) = alive;
	pthread_mutex_lock(philo->write);
	printf("statut%p\n", philo->status);
	*(philo->status) = 45;
	printf("statut%d\n", *(philo->status));
	pthread_mutex_unlock(philo->write);
	//lock philo pour acceder a la valeur partagee
	//check last meal : c'est comme s'il mangeait MAINTENANT
	while (*(philo->status) == alive)
	{
		printf("passe\n");
		if (*(philo->status) == dead)
		{
			print_messages(philo, "a philo has died\n");
			return (NULL);
		}
		eat(philo);
		sleeping(philo);
		if (*(philo->status) == dead)
		{
			print_messages(philo, "a philo has died\n");
			return (NULL);
		}
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