#include "lib.h"

struct Input {
    int n;
    int a[100];
    int b[100];
} input;

int output[100];

int add(int arg1, int arg2) {
    return arg1 + arg2;
}

int main() {
    loaddata(&input);
    for (int i = 0; i < input.n; i++) {
        output[i] = add(input.a[i], input.b[i]);
    }
    setanswer(output, sizeof(int) * input.n);
    return 0;
}
