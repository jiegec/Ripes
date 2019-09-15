#include "lib.h"

volatile int* arg = (int*)0x80000000;

int factorial(int arg) {
    if (arg == 0 || arg == 1) {
        return 1;
    }
    return arg * factorial(arg - 1);
}

int main() {
    printf("Arg before loading is %d\n", *arg);
    loaddata();
    int a = *arg;
    printf("Arg after loading is %d\n", a);
    printf("Factorial of %d is %d\n", a, factorial(a));
    return 0;
}
