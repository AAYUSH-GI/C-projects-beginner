#include <stdio.h>
#include <string.h>

void reverse(char *str) {
    size_t len = strlen(str);
    if (len == 0) return;
    size_t i = 0;
    size_t j = len - 1;
    while (i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

int main(void) {
    char buffer[256];
    printf("Enter text to reverse: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        printf("Failed to read input.\n");
        return 1;
    }

    // Remove trailing newline if present
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    reverse(buffer);
    printf("Reversed: %s\n", buffer);
    return 0;
}
