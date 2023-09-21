/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 21:18:58 by elrichar          #+#    #+#             */
/*   Updated: 2023/09/21 10:53:43 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// void	wait(t_philo *philo)
// {
// 	struct timeval	tv;
// 	long long		time_start;
// 	long long		time_launch;

// 	gettimeofday(&tv, NULL);
// 	time_start = (tv.tv_sec * 1000000 + tv.tv_usec) / 1000; //pour passer de micro à milli seconde
// 	time_launch = time_start + (0 * (philo->nb_philo - philo->pos));
// 	while (time_start < time_launch)
// 	{
// 		//check_is_dead;
// 		time_start = gettimeofday(&tv, NULL);
// 		time_start = (tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
// 		usleep(200); // sinon la condition est vérifiée constamment, ça surcharge la capacité du CPU
// 	} 
// }
