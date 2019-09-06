#include "lib.h"

__attribute__((section(".init"))) void _start() {
	int a = 1;
    printf("test1: %d", a);
    exit();
	return;
}

