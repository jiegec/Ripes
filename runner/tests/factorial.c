#include "lib.h"

int factorial(int arg) {
	if (arg == 0 || arg == 1) {
		return 1;
	}
	return arg * factorial(arg - 1);
}

__attribute__((section(".init"))) void _start() {
	int a = 10;
    printf("Factorial of %d is %d\n", a, factorial(a));
    printf("Factorial exited\n");
    exit();
	return;
}

