#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    srand((unsigned int)time(NULL));
    int secret = rand() % 100 + 1;
    int guess;
    int attempts = 0;

    printf("Guess the number (1-100)!\n");

    while (1) {
        printf("Enter your guess: ");
        if (scanf("%d", &guess) != 1) {
            printf("Please enter a valid number.\n");
            return 1;
        }
        attempts++;

        if (guess == secret) {
            printf("Correct! You guessed it in %d tries.\n", attempts);
            break;
        } else if (guess < secret) {
            printf("Too low. Try again.\n");
        } else {
            printf("Too high. Try again.\n");
        }
    }

    return 0;
}
