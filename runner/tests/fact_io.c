#include "lib.h"

int n;

int factorial(int arg) {
    if (arg == 0 || arg == 1) {
        return 1;
    }
    return arg * factorial(arg - 1);
}

int main() {
    printf("Arg before loading is %d\n", n);
    loaddata(&n);
    printf("Arg after loading is %d\n", n);
    printf("Factorial of %d is %d\n", n, factorial(n));
    return 0;
}
