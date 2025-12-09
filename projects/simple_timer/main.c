#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#define SLEEP(seconds) Sleep((seconds) * 1000)
#else
#include <unistd.h>
#define SLEEP(seconds) sleep(seconds)
#endif

int main(void) {
    int seconds;
    printf("Enter seconds to count down: ");
    if (scanf("%d", &seconds) != 1 || seconds < 0) {
        printf("Enter a non-negative integer.\n");
        return 1;
    }

    for (int i = seconds; i > 0; i--) {
        printf("%d...\n", i);
        SLEEP(1);
    }

    printf("Time's up!\n");
    return 0;
}
