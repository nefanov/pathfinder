#include <stdio.h>
int foo()
{
	printf("printf_in_foo");
}
int main()
{
	int a, b;
	a = 1;
	b = 2;
	printf("%d", a + b);
}
