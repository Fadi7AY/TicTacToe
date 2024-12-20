#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define PLAYER_X 'X'
#define PLAYER_O 'O'

void display_board();
int check_winner(char player);
int is_draw();
void make_move(char player);

char board[3][3] = {
    {'7', '8', '9'},
    {'4', '5', '6'},
    {'1', '2', '3'}
};

int main() {
    char current_player = PLAYER_X;
    int moves = 0;

    printf("\n          Tic Tac Toe\n\n\n");
    printf("Player 1 : X  ,  Player 2 : O.\n\n");
    printf("\nChoose a number :\n");

    while (1) {
        display_board();
        make_move(current_player);
        moves++;

        // Check if the current player has won
        if (check_winner(current_player)) {
            display_board();
            printf("\nPlayer %c is the winner!\n", current_player);
            break;
        }

        // Check if the game is a draw
        if (moves == 9) {
            display_board();
            printf("\nIt's a draw!\n");
            break;
        }

        // Switch player
        current_player = (current_player == PLAYER_X) ? PLAYER_O : PLAYER_X;
    }

    printf("\nDeveloped by Fadi7AY\n\n");
    return 0;
}

void display_board() {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        if (i != 0) printf("---------------------\n");
        for (int j = 0; j < 3; j++) {
            printf(" |  %c ", board[i][j]);
        }
        printf(" |\n");
    }
    printf("\n");
}

void make_move(char player) {
    int pick;
    int valid = 0;

    while (!valid) {
        printf("Player %c, enter your move (1-9): ", player);
        scanf("%d", &pick);

        if (pick < 1 || pick > 9) {
            printf("Invalid input. Please choose a number between 1 and 9.\n");
            continue;
        }

        // Convert pick to board indices
        int row = 2 - (pick - 1) / 3;
        int col = (pick - 1) % 3;

        // Check if the position is already taken
        if (board[row][col] == PLAYER_X || board[row][col] == PLAYER_O) {
            printf("Position already taken. Please choose another.\n");
        }
        else {
            board[row][col] = player;
            valid = 1;
        }
    }
}

int check_winner(char player) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) return 1; // Row
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) return 1; // Column
    }

    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) return 1;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) return 1;

    return 0;
}

int is_draw() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != PLAYER_X && board[i][j] != PLAYER_O) return 0;
        }
    }
    return 1;
}
