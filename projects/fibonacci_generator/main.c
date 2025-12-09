#include <stdio.h>

int main(void) {
    int n;
    printf("How many Fibonacci numbers should be printed? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Enter a positive integer.\n");
        return 1;
    }

    long long a = 0;
    long long b = 1;

    printf("Fibonacci sequence: ");
    for (int i = 0; i < n; i++) {
        printf("%lld", a);
        if (i < n - 1) {
            printf(", ");
        }
        long long next = a + b;
        a = b;
        b = next;
    }
    printf("\n");

    return 0;
}
