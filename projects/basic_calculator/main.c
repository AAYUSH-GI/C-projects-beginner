#include <stdio.h>

int main(void) {
    double a, b;
    char op;

    printf("Enter expression (e.g., 4.5 * 2): ");
    if (scanf("%lf %c %lf", &a, &op, &b) != 3) {
        printf("Invalid input.\n");
        return 1;
    }

    switch (op) {
    case '+':
        printf("%.2f + %.2f = %.2f\n", a, b, a + b);
        break;
    case '-':
        printf("%.2f - %.2f = %.2f\n", a, b, a - b);
        break;
    case '*':
        printf("%.2f * %.2f = %.2f\n", a, b, a * b);
        break;
    case '/':
        if (b == 0) {
            printf("Division by zero is not allowed.\n");
            return 1;
        }
        printf("%.2f / %.2f = %.2f\n", a, b, a / b);
        break;
    default:
        printf("Unsupported operator: %c\n", op);
        return 1;
    }

    return 0;
}
