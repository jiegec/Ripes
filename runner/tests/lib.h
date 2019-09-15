static void putint(int arg) {
    asm volatile(
        "li a0, 1\n"
        "mv a1, %0\n"
        "ecall"
        :
        : "r"(arg)
        : "a0", "a1");
}

static void putchar(char arg) {
    asm volatile(
        "li a0, 2\n"
        "mv a1, %0\n"
        "ecall"
        :
        : "r"(arg)
        : "a0", "a1");
}

static void loaddata(void *addr) {
    asm volatile(
        "li a0, 3\n"
        "mv a1, %0\n"
        "ecall"
        :
        : "r"(addr)
        : "a0", "a1", "memory");
}

static void putstring(char* arg) {
    asm volatile(
        "li a0, 4\n"
        "mv a1, %0\n"
        "ecall"
        :
        : "r"(arg)
        : "a0", "a1");
}

static void exit(int res) {
    asm volatile(
        "li a0, 10\n"
        "mv a1, %0\n"
        "ecall"
        :
        : "r"(res)
        : "a0", "a1");
}

static int printbase(long v, int w, int base, int sign) {
    int i, j;
    int c;
    char buf[64];
    unsigned long value;
    if (sign && v < 0) {
        value = -v;
        putchar('-');
    } else
        value = v;

    for (i = 0; value; i++) {
        buf[i] = value % base;
        value = value / base;
    }

    for (j = (w > i ? w : i); j > 0; j--) {
        c = j > i ? 0 : buf[j - 1];
        putchar((c <= 9) ? c + '0' : c - 0xa + 'a');
    }
    return 0;
}

static int printf(const char* fmt, ...) {
    int i;
    char c;
    void** arg;
    void* ap;
    int w;
    __builtin_va_start(ap, fmt);
    arg = ap;
    for (i = 0; fmt[i]; i++) {
        c = fmt[i];
        if (c == '%') {
            w = 1;
        again:
            switch (fmt[i + 1]) {
                case 's':
                    putstring(*arg);
                    arg++;
                    i++;
                    break;
                case 'c':
                    putchar((long)*arg);
                    arg++;
                    i++;
                    break;
                case 'u':
                    printbase((long)*arg, w, 10, 0);
                    arg++;
                    i++;
                    break;
                case 'd':
                    printbase((long)*arg, w, 10, 1);
                    arg++;
                    i++;
                    break;
                case 'l':
                    printbase((long)*arg, w, 10, 0);
                    arg++;
                    i = i + 2;
                    break;
                case 'o':
                    printbase((long)*arg, w, 8, 0);
                    arg++;
                    i++;
                    break;
                case 'b':
                    printbase((long)*arg, w, 2, 0);
                    arg++;
                    i++;
                    break;
                case 'p':
                case 'x':
                    printbase((long)*arg, w, 16, 0);
                    arg++;
                    i++;
                    break;
                case '%':
                    putchar('%');
                    i++;
                    break;
                case '0':
                    i++;
                case '1' ... '9':
                    for (w = 0; fmt[i + 1] > '0' && fmt[i + 1] <= '9'; i++)
                        w = w * 10 + (fmt[i + 1] - '0');
                    goto again;
                    break;

                default:
                    putchar('%');
                    break;
            }
        } else {
            if (c == '\n')
                putchar('\r');
            putchar(c);
        }
    }
    return 0;
}

int main();
__attribute__((section(".init"))) void _start() {
    exit(main());
	return;
}