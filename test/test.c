#include <stdio.h>
#include <stdlib.h>

void modify(int **tab)
{
	(*tab)[0] = 18;
	(*tab)[1] = 20;
}

void	modify2(int **tab)
{
	*(*tab) = 288;
}
int main ()
{
	int *tab = malloc(sizeof(int) * 2);
	int	value = 4;
	int	*pointer_to_value = &value;

	tab[0] = 2;
	tab[1] = 3;
	printf("%d et %d et %d\n", tab[0], tab[1], value);
	modify(&tab);
	modify2(&pointer_to_value);
	printf("%d et %d et %d\n", tab[0], tab[1], value);
}