#include <stdio.h>
int fact(int b)
{
    if (b > 1)
        return fact(b - 1) * b;
    else if (b == 1)
        return 1;
    else
        return 0;
}
int main()
{
    printf("%d\n", fact(3));
}