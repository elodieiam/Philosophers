/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 15:06:51 by elrichar          #+#    #+#             */
/*   Updated: 2023/09/13 19:19:55 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	ft_atoi(const char *str)
{
	int			i;
	int			sign;
	long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9' )
	{
		result *= 10;
		result += str[i] - '0';
		i++;
	}
	return (result * sign);
}

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

int	check_zero (char *av)
{
	int	i;

	i = 0;
	while (av[i])
	{
		if (av[i] != '0')
			return (1);
		i++;
	}
	return (0);
}

int	check_is_int(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < 	ac)
	{
		if (ft_atoi(av[i]) > 2147483647)
			return (0);
		i++;
	}
	return (1);
}

/*Cette fonction vérifie qu'on a au moins un philo pour lancer la simulation 
(si on en a 1 il meurt mais il faut pouvoir lancer le programme alors que si on a 
0 philo on ne lance pas le programme)
Elle vérifie aussi qu'on a un nombre correct d'arguments et qu'ils ne contiennent que des
nombres (pas de signes négatifs acceptés)
Et enfin que les valeurs passées en arguments ne dépassent pas la limite des int
*/
int	check_args(int ac, char **av)
{
	if (ac == 5 || ac == 6)
	{
		if (!check_zero(av[1]))
		{
			printf("Error : only one philo.\n");
			return (0);
		}
		if (!check_is_number(ac, av))
		{
			printf("Error : arguments are not digits.\n");
			return (0);
		}
		if (!check_is_int(ac, av))
		{
			printf("Error : arguments are not ints.\n");
			return (0);
		}
		if (ac == 6 && !check_zero(av[5]))
		{
			printf("Error : philos must eat at least once.\n");
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
