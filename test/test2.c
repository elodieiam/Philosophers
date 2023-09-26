/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:40:36 by elrichar          #+#    #+#             */
/*   Updated: 2023/09/17 12:12:08 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* On crée une variable statique et on renvoie un pointeur dessus
On peut donc la modifier en mémoire (ex 2)
Si on avait besoin de juste regarder la valeur on peut faire comme l'ex 1

On doit la créer sous la forme statique et non pas "int" car sinon on renvoie 
l'& d'une variable dont le scope est celui de la fonction, pas du programme. Sa durée de vie
est celle de la fonction. La var est donc créée puis détruite à chaque appel
A chaque appel, son adresse change donc 

Une statique, elle, a un lifetime différent : sa durée de vie est celle du programme

Scope : région du programme où on peut accéder à la donnée avec son nom
 (dans les 2 cas, la var peut 
être lue uniquement à travers la fonction). Elle n'est pas visible dans d'autres fonctions
si on fait "printf" par ex

Lifetime : durée de vie de la variable

Déclarer une fonction comme static "static int f()" on dit au compilateur que la fonction n'est accessible
que dans le fichier où elle a été déclarée

*/
#include <stdio.h>

// int	f(void)
// {
// 	int var = 3; 
// 	return (var);
// }

// int main()
// {
// 	int	p = f();
// 	printf("%d\n", p);
// }

int	*f(void)
{
	static int var;
	return (&var);
}

int main()
{
	int	*p = f();
	printf("%d\n", *p);
	*p = 2;
	printf("%d\n", *p);
}