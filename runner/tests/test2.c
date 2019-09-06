#include "lib.h"

__attribute__((section(".init"))) void _start() {
	int a = 1;
    printf("test2: %s %d", "1 equals ", a);
	exit();
	return;
}

