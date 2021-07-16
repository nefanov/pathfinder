#include <stdio.h>
int foo(int a, int b)
{
	if (a == 1)
		return a;
		
	return a + b;
}
int main()
{
	int a, b;
	a = 1;
	b = 2;
	for (int i = 0; i < 10; i++)
	{
		printf("%d", foo(a, foo(a, b)));
	}
	return 0;
}
