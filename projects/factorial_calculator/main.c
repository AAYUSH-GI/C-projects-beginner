#include <stdio.h>

unsigned long long factorial(int n) {
    if (n < 0) {
        return 0;
    }
    unsigned long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= (unsigned long long)i;
    }
    return result;
}

int main(void) {
    int n;
    printf("Enter a non-negative integer: ");
    if (scanf("%d", &n) != 1 || n < 0) {
        printf("Please enter a valid non-negative integer.\n");
        return 1;
    }

    printf("%d! = %llu\n", n, factorial(n));
    return 0;
}
