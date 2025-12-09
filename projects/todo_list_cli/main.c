#include <stdio.h>
#include <string.h>

#define MAX_TASKS 50
#define MAX_LEN 100

int main(void) {
    char tasks[MAX_TASKS][MAX_LEN] = {{0}};
    int count = 0;
    char command;

    printf("Simple To-Do List (a=add, l=list, r=remove, q=quit)\n");

    while (1) {
        printf("Command: ");
        if (scanf(" %c", &command) != 1) {
            break;
        }

        if (command == 'a') {
            if (count >= MAX_TASKS) {
                printf("List is full.\n");
                continue;
            }
            printf("Enter task: ");
            getchar(); // consume newline
            if (!fgets(tasks[count], MAX_LEN, stdin)) {
                printf("Failed to read task.\n");
                continue;
            }
            size_t len = strlen(tasks[count]);
            if (len > 0 && tasks[count][len - 1] == '\n') {
                tasks[count][len - 1] = '\0';
            }
            count++;
        } else if (command == 'l') {
            if (count == 0) {
                printf("No tasks yet.\n");
            } else {
                for (int i = 0; i < count; i++) {
                    printf("%d: %s\n", i + 1, tasks[i]);
                }
            }
        } else if (command == 'r') {
            if (count == 0) {
                printf("No tasks to remove.\n");
                continue;
            }
            int index;
            printf("Task number to remove: ");
            if (scanf("%d", &index) != 1 || index < 1 || index > count) {
                printf("Invalid index.\n");
                continue;
            }
            for (int i = index - 1; i < count - 1; i++) {
                strcpy(tasks[i], tasks[i + 1]);
            }
            count--;
        } else if (command == 'q') {
            printf("Goodbye!\n");
            break;
        } else {
            printf("Unknown command. Use a, l, r, or q.\n");
        }
    }

    return 0;
}
