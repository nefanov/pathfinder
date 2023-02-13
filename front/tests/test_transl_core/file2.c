#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int f(int a, int b) {
    return a + b;
}

int main(int argc, char* argv[]) {
    int a = 5;
    int b = 6;
    int c = f(a, b);
    return 0;
}
