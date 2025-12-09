#include <stdio.h>

static double c_to_f(double c) { return (c * 9.0 / 5.0) + 32.0; }
static double f_to_c(double f) { return (f - 32.0) * 5.0 / 9.0; }

int main(void) {
    double value;
    char scale;

    printf("Enter temperature with scale (e.g., 32 F or 0 C): ");
    if (scanf("%lf %c", &value, &scale) != 2) {
        printf("Invalid input.\n");
        return 1;
    }

    if (scale == 'C' || scale == 'c') {
        printf("%.2f C = %.2f F\n", value, c_to_f(value));
    } else if (scale == 'F' || scale == 'f') {
        printf("%.2f F = %.2f C\n", value, f_to_c(value));
    } else {
        printf("Unknown scale. Use C or F.\n");
        return 1;
    }

    return 0;
}
