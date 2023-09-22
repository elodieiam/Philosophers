/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 21:16:57 by elrichar          #+#    #+#             */
/*   Updated: 2023/09/22 15:59:20 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


/* ((*philos)[i]).pos : quand on fait ça on accède à la structure en indice i
On accède à ses composantes via le point et pas la -> car on travaille avec la structure
directement et pas un pointeur dessus 
(on a un tableau de structures, pas de pointeurs sur structures)
Quand on regarde dans le main si ça a été modifié en mémoire c'est bien le cas CAR
on travaille avec un TABLEAU de structures
Tout comme avec les tab de int par ex, quand on modifie tab[i] avec une fonction 
on modifie la valeur en mémoire : tab[i] = *(tab + i)
Par ailleurs, quand on passe l'adresse d'une adresse et qu'on modifie ce qui se trouve
au bout de ces 2 adresses c'est modifié en mémoire même si on a un déréférencement en plus
*philos accesses the first element of the philos array.
[i] accesses the i-th element of the array.
.pos accesses the pos member of the structure in the i-th element.
*/

/* On commence par initialiser la structure du philo PUIS on l'envoie au thread pour le creer
Sinon, on envoie une strucutre au thread qui commence a travailler dessus et acceder aux donnees
alors au'en meme temps dans la fonction on initialise ses composantes : data races
On envoie au thread un pointeur sur la structure qu'on veut donc &(*philos)[i] : l'adresse de
la strcuture en indice i du tableau.
(*philos)[i] c'est la struct mais on veut envoyer un pointeur*/

int	init_data_philos(char **av, int ac, t_philo **philos)
{
	int		i;
	int		nb;
	bool	status;

	i = 0;
	nb = ft_atoi(av[1]);
	status = alive;
	while (i < nb)
	{
		(*philos)[i].ID = 0;
		(*philos)[i].pos = i + 1;
		(*philos)[i].nb_philo = ft_atoi(av[1]);
		(*philos)[i].time_die = ft_atoi(av[2]);
		(*philos)[i].time_eat = ft_atoi(av[3]);
		(*philos)[i].time_sleep = ft_atoi(av[4]);
		if (ac == 6)
			(*philos)[i].number_meals = ft_atoi(av[5]);
		else
			(*philos)[i].number_meals = (-1);
		(*philos)[i].status = &status;
		(*philos)[i].death_time = ft_atoi(av[2]);
		(*philos)[i].meals_eaten = 0;
		(*philos)[i].time = get_time();
		printf("%p et %d\n",(*philos)[i].status, *((*philos)[i].status));
		i++;
	}
	return (1);
}

int	init_mutex_philos(int nb, t_philo **philos, pthread_mutex_t **forks)
{
	int	i;
	pthread_mutex_t	lock_philo;
	pthread_mutex_t	write;

	if (pthread_mutex_init(&lock_philo, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&write, NULL) != 0)
	{
		pthread_mutex_destroy(&lock_philo);
		return (0);
	}
	i = 0;
	while (i < nb)
	{
		//clockwise
		if (i == 0)
		{
			(*philos)[i].l_fork = (*forks);//l_fork du 0 = sa propre fork ([0])
			(*philos)[i].r_fork = (*forks) + (nb - 1);//r_fork du 0 = fork du dernier ([nb - 1])
		}
		else
		{
			(*philos)[i].l_fork = (*forks)+ i;//l_fork du i = sa propre fork ([i])
			(*philos)[i].r_fork = (*forks) + (i - 1);//r_fork du i = fork de celui d'avant ([i - 1])
		}
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

/*Cette fonction initialise un tableau de mutex sur les fourchettes
Il y a un mutex par fourchette
pthread_mutex_init((&((*forks)[i])) car on veut accéder au mutex en indice i
(en indice i on a un objet de type mutex)
et passer son adresse à la fonction pour qu'elle modifie sa valeur en mémoire
(car on modifie avec un pointeur sur le mutex)
*/

/*Attention : on crée un objet de type pthread_t ou mutex_t et on passe son adresse aux 
fonctions pour qu'elles modifient leur valeur en mémoire
On ne crée pas un pointeur sur ces objets*/
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
		free_mutex(av, forks); //faut-il laisser pthread_join avant de free les mutex 
		return (0);
	}
	return (1);
}