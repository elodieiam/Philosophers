/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:34:41 by elrichar          #+#    #+#             */
/*   Updated: 2023/09/17 22:12:04 by elrichar         ###   ########.fr       */
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

int	*f(void)
{
	static int var;
	return (&var); //faire un mutex dessus pour protéger l'accès
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

int	init_variables(char **av, pthread_mutex_t **forks, t_philo **philos)
{
	if (!init_forks(av, forks))
		return (0);
	if (!init_philos(av, philos, forks))
	{
		free_mutex(av, forks); //faut-il laisser pthread_join avant de free les mutex 
		return (0);
	}
	return (1);
}

void	pick_fork(t_philo *philo)
{
	if (philo->pos % 2 == 0)
	{
		pthread_mutex_lock(&(philo->r_fork));
		pthread_mutex_lock(&(philo->l_fork));
		printf("Philo %d is eating\n", philo->pos);
		pthread_mutex_unlock(&(philo->r_fork));
		pthread_mutex_unlock(&(philo->l_fork));
		
	}
	else
	{
		pthread_mutex_lock((&philo->l_fork));
		pthread_mutex_lock(&(philo->r_fork));
		printf("Philo %d is eating\n", philo->pos);
		pthread_mutex_unlock(&(philo->l_fork));
		pthread_mutex_unlock(&(philo->r_fork));
	}
}

void	wait(t_philo *philo)
{
	struct timeval	tv;
	long long		time_start;
	long long		time_launch;

	gettimeofday(&tv, NULL);
	time_start = (tv.tv_sec * 1000000 + tv.tv_usec) / 1000; //pour passer de micro à milli seconde
	time_launch = time_start + (150 * (philo->nb_philo - philo->pos));
	while (time_start < time_launch)
	{
		//check_is_dead;
		time_start = gettimeofday(&tv, NULL);
		time_start = (tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
		usleep(500); // sinon la condition est vérifiée constamment, ça surcharge la capacité du CPU
	} 
}

void	think(t_philo *philo)
{
	printf("Philo %d is thinking.\n", philo->pos);
}

void	*routine(void *arg)
{
	t_philo *philo;
	int		*indicator;
	

	philo = arg;
	indicator = NULL;
	
	printf("Thread created\n");
	wait(philo);//usleep (n° du philo x un certain temps défini)
	indicator = f();
	if (*indicator == 1)
		return (NULL);
	if (philo->pos % 2 == 0)
	{
		pick_fork(philo);
	}
	else
	{
		think(philo);
		pick_fork(philo);
	}
	//pick_fork(philo);
	//lancer les actions : recup fourchette, drop fourchettes, sleep, think
	//print message 
	return (NULL);
}

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
int	init_philos(char **av, t_philo **philos, pthread_mutex_t **forks)
{
	int	nb;
	int	i;
	int	*indicator;

	nb = ft_atoi(av[1]);
	i = 0;
	indicator = f();
	*philos = malloc(sizeof(t_philo) * nb);
	if (!*philos)
		return (0);
	while (i < nb)
	{
		//clockwise
		if (i == 0)
		{
			(*philos)[i].l_fork = (*forks)[0];
			(*philos)[i].r_fork = (*forks)[nb - 1];
		}
		else
		{
			(*philos)[i].l_fork = (*forks)[i];
			(*philos)[i].r_fork = (*forks)[i - 1];
		}
		(*philos)[i].pos = (i + 1);
		(*philos)[i].nb_philo = ft_atoi(av[1]);
		(*philos)[i].time_die = ft_atoi(av[2]);
		(*philos)[i].time_eat = ft_atoi(av[3]);
		(*philos)[i].time_sleep = ft_atoi(av[4]); // = tab[i] == *(tab + i)
		if (av[5])
			(*philos)[i].number_meals = ft_atoi(av[5]);
		else
			(*philos)[i].number_meals = -1;
		(*philos)[i].status = 1;
		i++;
	}
	i = 0;
	while (i < nb)
	{
		if (pthread_create(&(((*philos)[i].ID)), NULL, routine, (void *)&(*philos)[i]) != 0)
		{
			*indicator = 1;
			printf("Error : pthread_create issue.\n");
			return (0);
		}
		i++;
	}
	i = 0;
	while (i < nb)
	{
		if (pthread_join(((*philos)[i]).ID, NULL) != 0)
		{
			printf("Error : pthread_join issue.\n");
		}
		i++;
	}
	free_mutex(av, forks);
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
	

		
   printf("%ld\n", philos[0].ID);
   printf("%ld\n", philos[1].ID);
   printf("%ld\n", philos[2].ID);
	return (0);
	}