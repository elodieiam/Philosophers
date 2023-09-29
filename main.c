/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:34:41 by elrichar          #+#    #+#             */
/*   Updated: 2023/09/29 17:39:10 by elrichar         ###   ########.fr       */
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


int	pick_forks(t_philo *philo) //return 1 si mort, 0 si pas mort
{
	/*la fonction peut return sous plusieurs conditions
	soit on a réussi à prendre des forks, auquel cas si qqn meurt dans routine on les drop
	mais elle peut aussi return avant d'avoir réussi à prendre les fourchettes, auqel cas on essaye ensuite de 
	drop des forks qui n'ont pas été prises*/
	if (philo->pos % 2 == 0)
	{
		if (pthread_mutex_lock((philo->r_fork)))
			printf("error\n");
		if (print_messages(philo, "has taken a fork\n"))
		{
			pthread_mutex_unlock((philo->r_fork));
			return (1);
		}
		while (philo->l_fork->__align)
		{
			if (is_dead(philo))
			{
				pthread_mutex_unlock((philo->r_fork));
				return (1);
			}
			usleep(500);
		}
		if (pthread_mutex_lock((philo->l_fork)))
			printf("error\n");
		if (print_messages(philo, "has taken a fork\n"))
		{
			drop_forks(philo);
			return (1);	
		}
	}
	else
	{
		if (pthread_mutex_lock((philo->l_fork)))
			printf("error\n");
		if (print_messages(philo, "has taken a fork\n"))
		{
			pthread_mutex_unlock((philo->l_fork));
			return (1);
		}
		while (philo->r_fork->__align)
		{
			if (is_dead(philo))
			{
				pthread_mutex_unlock((philo->l_fork));
				return (1);
			}
			usleep(500);
		}
		if (pthread_mutex_lock((philo->r_fork)))
			printf("error\n");
		if (print_messages(philo, "has taken a fork\n"))
		{
			drop_forks(philo);
			return (1);
		}
	}//btw on ne vérifie pas la mort entre chaque prise de fork car si l'une est dispo alors l'autre l'est forcément car on inverse
	//la prise et la dépose de fourchette entre pairs et impairs
	return (0); //il n'y a qu'ici qu'on est sûr que le philo a bien ses 2 fourchettes en main
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


int	print_messages(t_philo *philo, char *str)
{
	long long	current_time;
	long long	time;
	
	if (is_dead(philo))
		return (1);
	//lock le mutex philo ici dans certains cas, une mort a eu lieu mais on ne la voit pas car on a un léger temps d'attente pour verrouiller write donc on affiche le msg quand meme, qq ms après la mort
	pthread_mutex_lock(philo->lock_philo);
	if (*(philo->status) == dead)
	{
		pthread_mutex_unlock(philo->lock_philo);
		return (1);
	}
	pthread_mutex_lock(philo->write);
	current_time = get_time();
	time = current_time - (philo->time);
	printf("%lld %d %s\n", time, philo->pos, str);
	pthread_mutex_unlock(philo->write);
	pthread_mutex_unlock(philo->lock_philo);
	//delock
	return (0);
}

int	ft_sleep(t_philo *philo)
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
		/*je reste dans le while tant que < et je checke dans le if >=
		ça n'arrivera donc jamais. Ce n'est pas ça qu'il faut vérifier.*/
		if (is_dead(philo))
			return (1);
		usleep(500); //50 micro = 5ms
		current_time = get_time();
	}
	return (0);
}

int	my_usleep(t_philo *philo)
{
	long long	time;
	long long	current_time;
	long long	arrival_time;
	
	time = philo->time;
	current_time = get_time();
	arrival_time = current_time - time + philo->time_eat;
	//tant que le moment où on en est dans le prog est < au moment où on doit en être avant que le philo finisse de manger
	while ((current_time - time) < (arrival_time))
	{
		if (is_dead(philo))
			return (1);
		usleep(500); //50 micro = 5ms
		current_time = get_time();
	}
	return (0);
}

int	eat(t_philo *philo)
{
	if (is_dead(philo))
	{
		drop_forks(philo);
		return (1);
	}
	philo->death_time = (get_time() - philo->time) + philo->time_die;
	if (print_messages(philo, "is eating\n"))
	{
		drop_forks(philo);
		return (1);
	}
	if (my_usleep(philo)) //recoder usleep car pas aasez precis, faire plein de miniusleep + checker en même temps la mort
	{
		drop_forks(philo);
		return (1);
		
	}
	drop_forks(philo);
	if (is_dead(philo))
			return (1);
	if (philo->number_meals != -1)
		philo->meals_eaten += 1;
	return (0);
}

int	sleeping(t_philo *philo)
{
	if (print_messages(philo, "is sleeping\n"))
		return (1);
	if (ft_sleep(philo))
		return (1);
	return (0);
}

int	think(t_philo *philo)
{
	if (print_messages(philo, "is thinking\n"))
		return (1);
	if (philo->pos % 2 == 0)
	{
		// aussi pour les impairs est-ce que le temps ecoule depuis last meal > eat time + eat_time / 2? si oui, je peux prendre ma fourchette ? (si nb philo pair), si impair = 2.5, en attendant usleep(50) comme dhab + check is dead comme dhab
		usleep(philo->time_eat + (philo->time_eat / 2));
		while ((philo->r_fork)->__align)
		{
			if (is_dead(philo))
				return (1);
			usleep(500);
		}
	}
	else
	{
		while ((philo->l_fork)->__align)
		{
			if (is_dead(philo))
				return (1);
			usleep(500);
		}
	}
	return (0);
}

void	synchronize_launch(t_philo *philo)
{
	int	nb;

	nb = philo->nb_philo;
	while (philo->time > get_time()) //on attend tant que le futur rattrape le present
	{
		usleep(500);
	}
}

int	is_dead(t_philo *philo)
{
	long long	current_time;
	long long	time;

	pthread_mutex_lock(philo->lock_philo);
	if (*(philo->status) == dead)
	{
		pthread_mutex_unlock(philo->lock_philo);
		return (1);
	}
	pthread_mutex_unlock(philo->lock_philo);
	current_time = get_time();
	time = current_time - (philo->time);
	/*si on constate que notre philo est mort, il faut lock le statut général
	on doit lock nest le mutex sur write car sinon ce mutex
	peut être lock par qqn d'autre qui constate la mort d'un philo mais n'affiche rien
	car on vérifie avant d'afficher dans print_messages si un philo est mort*/
	if (philo->death_time < time)
	{
		pthread_mutex_lock(philo->lock_philo);
		//sinon plusieurs philos peuvent entrer et mettre le statut à 'dead' alors que l'un d'eux est déjà mort
		if(*(philo->status) == dead)
		{
			pthread_mutex_unlock(philo->lock_philo);
			return (1);
		}
		*(philo->status) = dead;
		pthread_mutex_lock(philo->write);
		// current_time = get_time();
		// time = current_time - (philo->time);
		printf("%lld %d a philo is dead\n", time, philo->pos);
		pthread_mutex_unlock(philo->write);
		pthread_mutex_unlock(philo->lock_philo);
		return (1);
	}
	return (0);
}


void	set_death_time(t_philo *philo)
{
	long long	time;
	long long	current_time;

	time = philo->time;
	current_time = get_time();
	philo->death_time = (current_time - time) + philo->time_die;
}

int	are_fed(t_philo *philo)
{
	if (philo->number_meals == (-1))
		return (0); //0 et pas 1 à cause de && et || c'est chiant à réfléchir
	if (philo->meals_eaten == philo->number_meals)
		return (1);
	return (0);
}

void	case_one(t_philo *philo)
{
	long long	time;
	long long	current_time;
	
	time = philo->time;
	current_time = get_time() - time;
	pthread_mutex_lock(philo->l_fork);//quitter le thread pas le programme
	print_messages(philo, "has taken a fork\n");
	while (current_time < philo->death_time)
	{
		usleep(50);
		current_time = get_time() - time;
	}
	pthread_mutex_unlock(philo->l_fork);
	print_messages(philo, "is dead\n");
	
}

int	check_init(t_philo *philo)
{
	pthread_mutex_lock(philo->lock_philo);
	if (*(philo->init_check))
		return (1);
	pthread_mutex_unlock(philo->lock_philo);
	return (0);
}

int	philo_odd_waits(t_philo *philo, int to_wait)
{
	long long	time;
	long long	current_time;

	time = philo->time;
	current_time = get_time();
	while ((current_time - time) < to_wait)
	{
		if (is_dead(philo))
			return (1);
		usleep(500); 
		current_time = get_time();
	}
	while ((philo->l_fork)->__align)
	{
		if (is_dead(philo))
			return (1);
		usleep(500);
	}
	return (0);
}

/* on fait attendre les philos impairs 
les impairs classiques attendent "time to eat", le temps que les pairs mangent
le dernier impair attend le temps qu'ils mangent + moitié du temps qu'ils mangent
pr laisser le temps aux autres impairs de commencer à manger sans interférence*/
int	lauch_odd_philos(t_philo *philo)
{
	int	nb_philo;

	nb_philo = philo->nb_philo;
	print_messages(philo, "is thinking\n");
	if ((nb_philo % 2) && (philo->pos == nb_philo)) //si nb philo impair et que c'est le dernier philo
	{
		if (philo_odd_waits(philo, philo->time_eat + (philo->time_eat / 2)))
			return (1);
		
	}
	else if (philo->pos % 2)
	{
		if (philo_odd_waits(philo, philo->time_eat))
			return (1);
	}
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	//printf("before %lld\n", get_time() - philo->time);
	// printf("%lld = philo time et %lld : current time\n", philo->time, get_time());
	
	synchronize_launch(philo);
	//printf("start: %lld\n", get_time() - (philo->time));
	//petit decalage dans set death time car on recalcule get_time a chque fois
	
	set_death_time(philo);

	
	if (check_init(philo))
		return (NULL);
	if (philo->nb_philo == 1)
	{
		case_one(philo);
		return (NULL);
	}
	if (philo->pos % 2)
	{
		if (lauch_odd_philos(philo))
			return (NULL);
	}
	while (!is_dead(philo) && !are_fed(philo))
	{
		if (pick_forks(philo))
			return (NULL);
		if (eat(philo) || are_fed(philo))
		{
			printf("%d\n", philo->meals_eaten);
			return (NULL);
		}
		if ((philo->number_meals) != -1)
		{
			if (philo->meals_eaten == philo->number_meals)
			{
					//printf("%d ate %d time\n", philo->pos, philo->meals_eaten);
				return (NULL);		
			}
		}
		if (is_dead(philo))
			return (NULL);
		if (sleeping(philo))
			return (NULL);
		// if (is_dead(philo)) //est-ce utile ?
		// 	return (NULL);
		if (think(philo))
			return (NULL);
	}
	//printf("%d ate %d time\n", philo->pos, philo->meals_eaten);
	return (NULL);
}


int	init_threads(t_philo *philos)
{
	int			nb;
	int			i;
	//creation monito
	nb = philos->nb_philo;
	i = 0;
	
	while (i < nb)
	{
		if (pthread_create(&(philos[i].ID), NULL, routine, (void *)(&philos[i])) != 0)
		{
		//pthread_create(&(((*philos)[i].ID)), NULL, routine, (void *)&(*philos)[i]) != 0 avec t_philo **philo
			*(philos->init_check) = 1;//creer var qui verifie si tous les threads ont ete crees ou non
			printf("Error : pthread_create failed\n");
			//join
			return (0);
		}
		i++;
	}
	i = 0;
	i = 0;
	while (i < nb)
	{
		if (pthread_join((philos[i]).ID, NULL) != 0)
		{
			printf("Error : pthread_join issue.\n");
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
	if (!init_variables(av, ac, &forks, &philos))
	{
		free(philos);
		free(forks);
		return (1);
	}
	if (!init_threads(philos))
	{
		free_mutex(av, &forks);
		free(philos);
		free(forks);
		return (1); //+free tout ce qu'il faut (créer une struct pour garder tout ce qu'on doit free en mémoire ?)
	}
	pthread_mutex_destroy((philos->lock_philo));
	pthread_mutex_destroy((philos->write));
	free(philos);
	free_mutex(av, &forks);
	return (0);
}