#include <stdio.h>

int main(void) {
    int n;
    printf("How many grades? ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > 100) {
        printf("Enter a positive count up to 100.\n");
        return 1;
    }

    double total = 0.0;
    for (int i = 0; i < n; i++) {
        double grade;
        printf("Grade %d: ", i + 1);
        if (scanf("%lf", &grade) != 1) {
            printf("Invalid grade.\n");
            return 1;
        }
        if (grade < 0.0 || grade > 100.0) {
            printf("Grades should be between 0 and 100.\n");
            return 1;
        }
        total += grade;
    }

    double average = total / n;
    printf("Average grade: %.2f\n", average);

    return 0;
}
