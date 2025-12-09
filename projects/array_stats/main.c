#include <stdio.h>

int main(void) {
    int count;
    printf("How many numbers? ");
    if (scanf("%d", &count) != 1 || count <= 0) {
        printf("Enter a positive integer.\n");
        return 1;
    }

    int total = 0;
    for (int i = 0; i < count; i++) {
        int value;
        printf("Value %d: ", i + 1);
        if (scanf("%d", &value) != 1) {
            printf("Invalid number.\n");
            return 1;
        }
        total += value;
    }

    double average = (double)total / count;
    printf("Sum = %d\n", total);
    printf("Average = %.2f\n", average);

    return 0;
}
