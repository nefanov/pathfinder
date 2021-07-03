#include <stdio.h>
int foo()
{
	printf("printf_in_foo");
    int a = 0;
    for (int i = 0; i < 10; i++)
        a += 2;
    return a;
}
int main()
{
	int a = 0, b = 0;
	while (1)
        a += foo();
    return 0;
}
