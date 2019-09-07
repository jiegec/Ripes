#include "lib.h"

int factorial(int arg) {
    if (arg == 0 || arg == 1) {
        return 1;
    }
    return arg * factorial(arg - 1);
}

int main() {
    int a = 10;
    printf("Factorial of %d is %d\n", a, factorial(a));
    return 0;
}
