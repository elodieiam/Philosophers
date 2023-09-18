/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 10:58:02 by elrichar          #+#    #+#             */
/*   Updated: 2023/09/18 11:10:33 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>

typedef struct s_philo
{
	pthread_t 		ID;
	pthread_mutex_t	l_fork;
	pthread_mutex_t r_fork;
	pthread_mutex_t	check;
	int				pos;
	int				nb_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				number_meals;
	int				status;
	// bool			*stop;
} t_philo;

long long	ft_atoi(const char *str);
int	check_is_number(int ac, char **av);
int	check_zero (char *av1);
int	check_is_int(int ac, char **av);
int	check_args(int ac, char **av);
void	free_mutex(char **av, pthread_mutex_t **forks);
int	init_variables(char **av, pthread_mutex_t **forks, t_philo **philos);
int	init_philos(char **av, t_philo **philos, pthread_mutex_t **forks);
int	init_forks(char **av, pthread_mutex_t **forks);
#endif