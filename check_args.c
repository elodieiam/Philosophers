/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 15:06:51 by elrichar          #+#    #+#             */
/*   Updated: 2023/09/11 15:06:54 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	check_is_number(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
		while (i < ac)
		{
			j = 0;
			while (av[i][j])
			{
				if (av[i][j] < '0' || av[i][j] > '9')
					return (0);
				j++;
			}
			i++;
		}
	return (1);
}

int	zero_philo (char *av1)
{
	int	i;

	i = 0;
	while (av1[i])
	{
		if (av1[i] != '0')
			return (1);
		i++;
	}
	return (0);
}

/*Cette fonction vérifie qu'on a au moins un philo pour lancer la simulation 
(si on en a 1 il meurt mais il faut pouvoir lancer le programme alors que si on a 
0 philo on ne lance pas le programme)
Elle vérifie aussi qu'on a un nombre correct d'arguments et qu'ils ne contiennent que des
nombres (pas de signes négatifs acceptés)
*/
int	check_args(int ac, char **av)
{
	if (ac == 5 || ac == 6)
	{
		if (!zero_philo(av[1]))
		{
			printf("Error : only one philo.\n");
			return (0);
		}
		if (!check_is_number(ac, av))
		{
			printf("Error : arguments are not digits.\n");
			return (0);
		}
		else
			return (1);
	}
	else
	{
		printf("Error : number of arguments incorrect.\n");
		return (0);
	}
}
