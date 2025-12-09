#include <stdio.h>

void insertion_sort(int *arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int main(void) {
    int data[100];
    int n;

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

    insertion_sort(data, n);

    printf("Sorted values: ");
    for (int i = 0; i < n; i++) {
        printf("%d", data[i]);
        if (i < n - 1) printf(" ");
    }
    printf("\n");

    return 0;
}
