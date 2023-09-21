/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 11:44:31 by elrichar          #+#    #+#             */
/*   Updated: 2023/09/21 12:00:30 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

typedef struct s_philo
{
	int	a;
} t_philo;

/*L'& affichée est la même. 
On passe un pointeur à la fonction. On travaille donc avec la structure originale, pas une copie
Puisqu'on a la vraie struct, tous ses membres sont aussi les originaux
Si on affiche leur &, c'est donc l'originale, pas celle d'une copie

Si on passait la structure en elle-même à la fonction, on donnerait l'adresse de la copie
des membres de la structure*/
void f(t_philo *philo)
{
	printf("address in function : %p\n", &(philo->a)); // == (*philo).a
	//== &(philos[i].ID) '.' et pas '->' car == &(*(philos + i).ID) . 
}

int main()
{
	t_philo	philo;
	
	philo.a = 3;
	printf("address in main : %p\n", &(philo.a));
	f(&philo);
}