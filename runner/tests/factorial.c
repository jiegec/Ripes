
void put_int(int arg) {
	asm volatile("li a0, 1\n"
		"mv a1, %0\n"
		"ecall"
		:
		:"r"(arg)
		:"a0", "a1");
}

void put_str(char * arg) {
	asm volatile("li a0, 4\n"
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

int factorial(int arg) {
	if (arg == 0 || arg == 1) {
		return 1;
	}
	return arg * factorial(arg - 1);
}

__attribute__((section(".init"))) void _start() {
	int a = 10;
	put_str("Factorial of ");
	put_int(a);
	put_str(" is ");
	put_int(factorial(a));
	put_str("\n");
	put_str("Factorial exited");
	exit();
	return;
}

