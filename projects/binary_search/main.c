#include <stdio.h>

int binary_search(const int *arr, int size, int target) {
    int low = 0;
    int high = size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

int main(void) {
    int data[100];
    int n;
    printf("Number of sorted elements (max 100): ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > 100) {
        printf("Enter a valid size up to 100.\n");
        return 1;
    }

    printf("Enter %d sorted integers: \n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &data[i]) != 1) {
            printf("Invalid number.\n");
            return 1;
        }
    }

    int target;
    printf("Value to search for: ");
    if (scanf("%d", &target) != 1) {
        printf("Invalid number.\n");
        return 1;
    }

    int index = binary_search(data, n, target);
    if (index >= 0) {
        printf("Found %d at index %d.\n", target, index);
    } else {
        printf("%d not found.\n", target);
    }

    return 0;
}
