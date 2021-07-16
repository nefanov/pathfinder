#include <stdio.h>

int foo1(int a)
{
    return 2 * a;
}

int foo2(int a)
{
    return a - 3;
}

int main()
{
    int b = 3;
    printf("foo1foo2oo %d", foo2(foo1(foo2(b))));
}