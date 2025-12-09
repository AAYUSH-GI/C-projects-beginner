#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool is_palindrome(const char *text) {
    size_t left = 0;
    size_t right = strlen(text);
    if (right == 0) return true;
    right--;

    while (left < right) {
        while (left < right && isspace((unsigned char)text[left])) left++;
        while (left < right && isspace((unsigned char)text[right])) right--;
        char a = (char)tolower((unsigned char)text[left]);
        char b = (char)tolower((unsigned char)text[right]);
        if (a != b) return false;
        left++;
        right--;
    }
    return true;
}

int main(void) {
    char buffer[256];
    printf("Enter text to check: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        printf("Failed to read input.\n");
        return 1;
    }

    if (is_palindrome(buffer)) {
        printf("It's a palindrome!\n");
    } else {
        printf("Not a palindrome.\n");
    }

    return 0;
}
