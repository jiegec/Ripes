void put_int(int arg) {
	asm volatile("li a0, 1\n"
		"mv a1, %0\n"
		"ecall"
		:
		:"r"(arg)
		:"a0", "a1");
}

void exit() {
	asm volatile("li a0, 10\n"
		"ecall"
		:
		:
		:"a0");
}

__attribute__((section(".init"))) void _start() {
	int a = 1;
	put_int(a);
	exit();
	return;
}

