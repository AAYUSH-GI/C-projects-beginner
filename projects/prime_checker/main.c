#include <math.h>
#include <stdbool.h>
#include <stdio.h>

bool is_prime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;

    int limit = (int)sqrt((double)n);
    for (int i = 3; i <= limit; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int main(void) {
    int n;
    printf("Enter an integer to test: ");
    if (scanf("%d", &n) != 1) {
        printf("Invalid input.\n");
        return 1;
    }

    if (is_prime(n)) {
        printf("%d is prime.\n", n);
    } else {
        printf("%d is not prime.\n", n);
    }

    return 0;
}
