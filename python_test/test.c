#include <stdio.h>

void fun(void)
{
	static int i = 1+1;
	i++;
	printf("%d\n", i);
}

void main(void)
{
	fun();
	fun();
}
