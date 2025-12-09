#include <stdio.h>

void draw_board(const char board[9]) {
    printf(" %c | %c | %c\n", board[0], board[1], board[2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", board[3], board[4], board[5]);
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", board[6], board[7], board[8]);
}

int check_winner(const char board[9]) {
    const int lines[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    for (int i = 0; i < 8; i++) {
        int a = lines[i][0];
        int b = lines[i][1];
        int c = lines[i][2];
        if (board[a] != ' ' && board[a] == board[b] && board[b] == board[c]) {
            return board[a];
        }
    }
    return 0;
}

int main(void) {
    char board[9];
    for (int i = 0; i < 9; i++) {
        board[i] = ' ';
    }

    int moves = 0;
    char current = 'X';

    while (moves < 9) {
        draw_board(board);
        int pos;
        printf("Player %c, choose a position (1-9): ", current);
        if (scanf("%d", &pos) != 1 || pos < 1 || pos > 9) {
            printf("Invalid position.\n");
            return 1;
        }

        if (board[pos - 1] != ' ') {
            printf("That spot is taken. Try again.\n");
            continue;
        }

        board[pos - 1] = current;
        moves++;

        int winner = check_winner(board);
        if (winner) {
            draw_board(board);
            printf("Player %c wins!\n", winner);
            return 0;
        }

        current = (current == 'X') ? 'O' : 'X';
    }

    draw_board(board);
    printf("It's a draw!\n");
    return 0;
}
