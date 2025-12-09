#include <stdio.h>

void bubble_sort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    int n;
    int data[100];

    printf("How many numbers (max 100)? ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > 100) {
        printf("Enter a valid count up to 100.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Number %d: ", i + 1);
        if (scanf("%d", &data[i]) != 1) {
            printf("Invalid number.\n");
            return 1;
        }
    }

    bubble_sort(data, n);

    printf("Sorted values: ");
    for (int i = 0; i < n; i++) {
        printf("%d", data[i]);
        if (i < n - 1) {
            printf(" ");
        }
    }
    printf("\n");

    return 0;
}
