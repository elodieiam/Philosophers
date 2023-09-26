#include <stdio.h>

void	f(int a)
{
	printf("f : %p\n", &a);
}

int main()
{
	int a = 3;
	f(a);
	printf("main : %p\n", &a);
}