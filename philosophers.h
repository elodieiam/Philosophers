/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 10:58:02 by elrichar          #+#    #+#             */
/*   Updated: 2023/09/28 14:59:51 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

typedef enum { alive, dead } bool;

typedef struct s_philo
{
	pthread_t 		ID;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t *r_fork;
	pthread_mutex_t	*lock_philo;
	pthread_mutex_t *write;
	int				pos;
	int				nb_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				number_meals;
	bool			*status;
	long long		death_time;
	int				meals_eaten;
	long long		time;
	int				personal_status;
	int				*init_check;
} t_philo;

long long	ft_atoi(const char *str);
int	check_is_number(int ac, char **av);
int	check_zero (char *av1);
int	check_is_int(int ac, char **av);
int	check_args(int ac, char **av);
void	free_mutex(char **av, pthread_mutex_t **forks);

void	set_philos_vars(char **av, t_philo **philos, pthread_mutex_t **forks, pthread_mutex_t *check);
int	create_threads(t_philo **philos, int nb);
int	init_data_philos(char **av, int ac, t_philo **philos);
int	init_mutex_philos(int nb, t_philo **philos, pthread_mutex_t **forks);
int	init_philos(char **av, int ac, t_philo **philos, pthread_mutex_t **forks);
int	init_forks(char **av, pthread_mutex_t **forks);
int	init_variables(char **av, int ac, pthread_mutex_t **forks, t_philo **philos);
long long	get_time(void);

void	join_threads(t_philo **philos, int nb);
int	pick_forks(t_philo *philo);
void	drop_forks(t_philo *philo);
int	print_messages(t_philo *philo, char *message);
int	eat(t_philo *philo);
int sleeping(t_philo *philo);
int	think(t_philo *philo);
void	*routine(void *arg);
int	init_threads(t_philo *philos);
int	is_dead(t_philo *philo);
void	philo_odd_waits(t_philo *philo, int to_wait);


#endif