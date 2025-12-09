#include <stdio.h>

int main(void) {
    int c;
    long chars = 0;
    long words = 0;
    long lines = 0;
    int in_word = 0;

    printf("Type text and press Ctrl+D (Linux/macOS) or Ctrl+Z (Windows) to finish.\n");

    while ((c = getchar()) != EOF) {
        chars++;
        if (c == '\n') {
            lines++;
        }
        if (c == ' ' || c == '\n' || c == '\t') {
            if (in_word) {
                words++;
                in_word = 0;
            }
        } else {
            in_word = 1;
        }
    }

    if (in_word) {
        words++;
    }

    printf("Characters: %ld\n", chars);
    printf("Words: %ld\n", words);
    printf("Lines: %ld\n", lines);

    return 0;
}
