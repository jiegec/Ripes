#include "lib.h"

struct Input {
    int a, b;
} input;

int c;

int main() {
    loaddata(&input);
    c = input.a + input.b;
    setanswer(&c);
    return 0;
}
