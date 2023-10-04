/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:34:41 by elrichar          #+#    #+#             */
/*   Updated: 2023/10/04 15:39:53 by elrichar         ###   ########.fr       */
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
	return ((tv.tv_sec * 1000000) + (tv.tv_usec / 1000) * 1000);
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
		// while ((philo->r_fork)->__align)
		// {
		// 	if (is_dead(philo))
		// 		return (1);
		// 	usleep(50);
		// }
		if (pthread_mutex_lock((philo->l_fork)))
			printf("error\n");
		if (print_messages(philo, "has taken a fork\n"))
		{
			pthread_mutex_unlock((philo->l_fork));
			return (1);
		}
			
		// while (philo->l_fork->__align)
		// {
		// 	if (is_dead(philo))
		// 	{
		// 		pthread_mutex_unlock((philo->r_fork));
		// 		return (1);
		// 	}
		// 	usleep(50);
		// }
		if (pthread_mutex_lock((philo->r_fork)))
			printf("error\n");
		if (print_messages(philo, "has taken a fork\n"))
		{
			drop_forks(philo);
			return (1);	
		}
	}
	else
	{
		// while ((philo->l_fork)->__align)
		// {
		// 	if (is_dead(philo))
		// 		return (1);
		// 	usleep(50);
		// }
		if (pthread_mutex_lock((philo->r_fork)))
			printf("error\n");
		if (print_messages(philo, "has taken a fork\n"))
		{
			pthread_mutex_unlock((philo->r_fork));
			return (1);
		}
		// while (philo->r_fork->__align)
		// {
		// 	if (is_dead(philo))
		// 	{
		// 		pthread_mutex_unlock((philo->l_fork));
		// 		return (1);
		// 	}
		// 	usleep(50);
		// }
		if (pthread_mutex_lock((philo->l_fork)))
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
	//long long	time = get_time() - philo->time;
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
	printf("%lld %d %s\n",  (get_time() - philo->time) / 1000, philo->pos, str);
	pthread_mutex_unlock(philo->write);
	pthread_mutex_unlock(philo->lock_philo);
	return (0);
	//important de garder les mutex dans cet ordre sinon msg affichés après la mort d'un philo
}

int	ft_sleep(t_philo *philo, long time)
{
	long long	current;
	long long	time_end;
	long long	time_of_death;

	current = get_time();
	time_end = get_time() + time;
	time_of_death = philo->last_meal + philo->time_die;
	//tant que le moment où on en est dans le prog est < au moment où on doit en être avant que le philo finisse de manger
	while (current < time_end)
	{
		usleep(330); //50 micro = 5ms
		current = get_time();
		if (current > time_of_death)
		{
			if (is_dead(philo))
				return (1);	
		}
	}
	return (0);
}


int	eat(t_philo *philo)
{
	if (print_messages(philo, "is eating\n"))
	{
		drop_forks(philo);
		return (1);
	}
	philo->last_meal = get_time();
	if (philo->number_meals != -1)
		philo->meals_eaten += 1;
	if (ft_sleep(philo, philo->time_eat)) //recoder usleep car pas aasez precis, faire plein de miniusleep + checker en même temps la mort
	{
		drop_forks(philo);
		return (1);
	}
	drop_forks(philo);	
	return (0);
}

int	sleeping(t_philo *philo)
{
	if (print_messages(philo, "is sleeping\n"))
		return (1);
	if (ft_sleep(philo, philo->time_sleep))
		return (1);
	return (0);
}

int	think(t_philo *philo)
{
	if (print_messages(philo, "is thinking\n"))
		return (1);
	return (0);
}

int	is_dead(t_philo *philo)
{
	long long	time;

	time = get_time() - philo->last_meal;
	if (time > philo->time_die)
	{
		pthread_mutex_lock(philo->lock_philo);
		if (!(*(philo->status)))
		{
			(*(philo->status)) = dead;
			pthread_mutex_lock(philo->write);
			printf("%lld %d died\n", (get_time() - philo->time) \
				/ 1000, philo->pos);
			pthread_mutex_unlock(philo->write);
		}
		pthread_mutex_unlock(philo->lock_philo);
		return (1);
	}
	return (0);
}


int	are_fed(t_philo *philo)
{
	if (philo->number_meals == (-1))
		return (0); //0 et pas 1 à cause de && et || c'est chiant à réfléchir
	if (philo->meals_eaten == philo->number_meals)
		return (1);
	return (0);
}

void	*case_one(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	if (print_messages(philo, "has taken a fork\n"))
	{
		pthread_mutex_unlock(philo->l_fork);
		return (NULL);
	}
	usleep(philo->time_die + 500);
	is_dead(philo);
	pthread_mutex_unlock(philo->l_fork);
	return (NULL);
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
	while ((get_time() - philo->time) < to_wait)
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
	pthread_mutex_lock(philo->lock_philo);
	printf("%lld %d is thinking\n\n", get_time() - philo->time, philo->pos);
	pthread_mutex_unlock(philo->lock_philo);
	if ((philo->nb_philo % 2) && (philo->pos == philo->nb_philo)) //si nb philo impair et que c'est le dernier philo
	{
		if (philo_odd_waits(philo, philo->time_eat + philo->time_eat / 2))
			return (1);
	}
	else if (philo->pos % 2)
	{
		if (philo_odd_waits(philo, philo->time_eat))
			return (1);
	}
	return (0);
}

void	wait_after_thinking(int delay)
{
	long long	time_now;

	time_now = get_time();
	while (get_time() - time_now < delay)
		usleep(50);
}

void	before_departure(t_philo * philo)
{
	philo->last_meal = 0;
	philo->time = philo->time + (philo->nb_philo * 1000);
	while (get_time() < philo->time)
		usleep(1000);
	philo->last_meal = get_time();
	print_messages(philo, "is thinking\n");
	if (philo->nb_philo % 2 == 0 && philo->pos % 2 != 0)
		usleep(philo->time_eat / 2);
	else if (philo->nb_philo % 2 != 0 && philo->pos % 2 == 0)
		usleep(philo->time_eat / 2);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	before_departure(philo);
	if (philo->nb_philo == 1)
		return (case_one(philo));
	while (!is_dead(philo))
	{
		if (pick_forks(philo))
			return (NULL);
		if (eat(philo))
			return (NULL);
		if (are_fed(philo))
			return (NULL);
		if (sleeping(philo))
			return (NULL);
		if (think(philo))
			return (NULL);
		wait_after_thinking(philo->sync);
	}
	return (NULL);
}

void join_threads(t_philo *philos, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		if (pthread_join((philos[i]).ID, NULL) != 0)
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
		if (pthread_create(&(philos[i].ID), 0, routine, (void *)(&philos[i])))
		{
			*(philos->init_check) = 1;
			printf("Error : pthread_create failed\n");
			while (--i >= 0)
			{
				if (pthread_join(philos[i].ID, NULL) != 0)
					printf("Error : pthread_join failed\n");
			}
			return (0);
		}
		i++;
	}
	join_threads(philos, nb);
	return (1);
}

void free_and_destroy(char **av, t_philo *philos, pthread_mutex_t *forks)
{
	pthread_mutex_destroy((philos->lock_philo));
	pthread_mutex_destroy((philos->write));
	free(philos);
	free_mutex(av, &forks);
}

int main (int ac, char **av)
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
