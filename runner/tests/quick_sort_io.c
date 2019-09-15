#include "lib.h"

int partition(int* a, int p, int q) {
    int pivot = a[p];
    int i = p, j = q;
    while (i < j) {
        while (i < j && a[j] > pivot)
            j--;
        a[i] = a[j];

        while (i < j && a[i] <= pivot)
            i++;
        a[j] = a[i];
    }

    a[i] = pivot;
    return i;
}

void _quick_sort(int* a, int p, int q) {
    if (p >= q)
        return;
    int m = partition(a, p, q);
    _quick_sort(a, p, m - 1);
    _quick_sort(a, m + 1, q);
}

int* quick_sort(int* a, int N) {
    _quick_sort(a, 0, N - 1);
    return a;
}

#define N 300
int data[N];

int main(void) {
    int* result;
    printf("quick sort test begin.\n");
    loaddata(data);
    result = quick_sort(data, N);
    return 0;
}