#include <stdio.h>

void selection_sort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
        int temp = arr[i];
        arr[i] = arr[min_index];
        arr[min_index] = temp;
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

    selection_sort(data, n);

    printf("Sorted values: ");
    for (int i = 0; i < n; i++) {
        printf("%d", data[i]);
        if (i < n - 1) printf(" ");
    }
    printf("\n");

    return 0;
}
