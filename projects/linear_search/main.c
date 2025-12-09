#include <stdio.h>

int main(void) {
    int n;
    printf("Number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Enter a positive integer.\n");
        return 1;
    }

    int data[100];
    if (n > 100) {
        printf("Limit to 100 elements for simplicity.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Element %d: ", i);
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

    int found_index = -1;
    for (int i = 0; i < n; i++) {
        if (data[i] == target) {
            found_index = i;
            break;
        }
    }

    if (found_index >= 0) {
        printf("Found %d at index %d.\n", target, found_index);
    } else {
        printf("%d was not found.\n", target);
    }

    return 0;
}
