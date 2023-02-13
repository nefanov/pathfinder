#include <stdio.h>

static int factorial (int n)
{
  return (n < 2) ? 1 : n * factorial (n - 1);
}

int main (void)
{
  int n;
  while (scanf ("%d", &n) == 1) {
    printf ("%d\n", factorial (n));
  }
  return 0;
}
