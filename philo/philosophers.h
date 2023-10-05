/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 10:58:02 by elrichar          #+#    #+#             */
/*   Updated: 2023/10/05 12:04:31 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef enum s_bool{ alive, dead }	t_bool;

typedef struct s_philo
{
	pthread_t		thread_id;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*lock_philo;
	pthread_mutex_t	*write;
	int				pos;
	int				nb_philo;
	long long		time_die;
	long long		time_eat;
	long long		time_sleep;
	int				number_meals;
	t_bool			*status;
	long long		last_meal;
	int				meals_eaten;
	long long		time;
	int				*init_check;
	int				sync;
}	t_philo;

//actions.c
int			my_usleep(t_philo *philo, long time);
void		wait_after_thinking(int delay);
int			sleeping(t_philo *philo);
int			think(t_philo *philo);
int			eat(t_philo *philo);

//check_args.c
long long	ft_atoi(const char *str);
int			check_is_number(int ac, char **av);
int			check_zero(char *av);
int			check_is_int(int ac, char **av);
int			check_args(int ac, char **av);

//forks.c
int			pick_forks_even(t_philo *philo);
int			pick_forks_odd(t_philo *philo);
int			pick_forks(t_philo *philo);
void		drop_forks(t_philo *philo);

//init_utils.c
void		set_common_vars(char **av, int i, t_philo **philos);
void		set_forks(int i, int nb, t_philo **philos, pthread_mutex_t **forks);
void		set_sync(int nb, int i, t_philo **philos);
void		set_sync_2(int nb, int i, t_philo **philos);

//init.c
int			init_data_philos(char **av, int ac, t_philo **philos);
int			init_mutex_philos(int nb, t_philo **p, pthread_mutex_t **f);
int			init_philos(char **av, int ac, t_philo **p, pthread_mutex_t **f);
int			init_forks(char **av, pthread_mutex_t **forks);
int			init_variables(char **av, int ac, pthread_mutex_t **f, t_philo **p);

//main.c
void		join_threads(t_philo *philos, int nb);
int			init_threads(t_philo *philos);

//routine.c
int			is_dead(t_philo *philo);
int			are_fed(t_philo *philo);
void		*case_one(t_philo *philo);
void		before_departure(t_philo *philo);
void		*routine(void *arg);

//utils.c
int			check_init(t_philo *philo);
int			print_messages(t_philo *philo, char *str);
void		free_and_destroy(char **av, t_philo *philos, pthread_mutex_t *fork);
void		free_mutex(char **av, pthread_mutex_t **forks);
long long	get_time(void);

#endif