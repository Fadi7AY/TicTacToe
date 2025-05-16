#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PLAYER_X 'X'
#define PLAYER_O 'O'

char board[3][3];
int difficulty = 1;
int vs_ai = 0;

void reset_board();
void display_board();
int check_winner(char player);
int is_draw();
void make_move(char player);
void make_random_move(char player);
void make_medium_ai_move(char player);
void make_best_move(char player);
int minimax(char b[3][3], int depth, int is_maximizing);

int main() {
    char current_player = PLAYER_X;
    int moves = 0;
    srand(time(NULL));

    printf("Tic Tac Toe\n");
    printf("1. Play against another player\n");
    printf("2. Play against the system (AI)\n");
    printf("Enter your choice: ");
    scanf("%d", &vs_ai);
    if (vs_ai == 2) {
        vs_ai = 1;
        printf("Choose AI difficulty:\n");
        printf("1. Easy\n2. Medium\n3. Hard\n");
        scanf("%d", &difficulty);
    } else {
        vs_ai = 0;
    }

    reset_board();

    while (1) {
        display_board();

        if (!vs_ai || current_player == PLAYER_X)
            make_move(current_player);
        else {
            if (difficulty == 1)
                make_random_move(current_player);
            else if (difficulty == 2)
                make_medium_ai_move(current_player);
            else
                make_best_move(current_player);
        }

        moves++;

        if (check_winner(current_player)) {
            display_board();
            printf("\n%c wins!\n", current_player);
            break;
        }

        if (moves == 9 || is_draw()) {
            display_board();
            printf("\nIt's a draw!\n");
            break;
        }

        current_player = (current_player == PLAYER_X) ? PLAYER_O : PLAYER_X;
    }

    return 0;
}

void reset_board() {
    char ch = '1';
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = ch++;
}

void display_board() {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        if (i) printf("-----------\n");
        for (int j = 0; j < 3; j++) {
            if (j) printf("|");
            printf(" %c ", board[i][j]);
        }
        printf("\n");
    }
}

void make_move(char player) {
    int pick, row, col, valid = 0;
    while (!valid) {
        printf("Player %c, enter your move (1-9): ", player);
        scanf("%d", &pick);
        if (pick < 1 || pick > 9) {
            printf("Invalid input. Try again.\n");
            continue;
        }
        pick--; // adjust to 0-based
        row = pick / 3;
        col = pick % 3;
        if (board[row][col] == PLAYER_X || board[row][col] == PLAYER_O) {
            printf("Position taken. Try again.\n");
        } else {
            board[row][col] = player;
            valid = 1;
        }
    }
}

void make_random_move(char player) {
    int empty[9], count = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != PLAYER_X && board[i][j] != PLAYER_O)
                empty[count++] = i * 3 + j;

    if (count == 0) return;

    int index = empty[rand() % count];
    int row = index / 3;
    int col = index % 3;
    board[row][col] = player;
    printf("System (Easy) plays at %d\n", index + 1);
}

void make_medium_ai_move(char player) {
    char opponent = (player == PLAYER_X) ? PLAYER_O : PLAYER_X;

    for (int i = 0; i < 9; i++) {
        int row = i / 3, col = i % 3;
        if (board[row][col] != PLAYER_X && board[row][col] != PLAYER_O) {
            char temp = board[row][col];
            board[row][col] = player;
            if (check_winner(player)) {
                printf("System (Medium) plays to win at %d\n", i + 1);
                return;
            }
            board[row][col] = temp;
        }
    }

    for (int i = 0; i < 9; i++) {
        int row = i / 3, col = i % 3;
        if (board[row][col] != PLAYER_X && board[row][col] != PLAYER_O) {
            char temp = board[row][col];
            board[row][col] = opponent;
            if (check_winner(opponent)) {
                board[row][col] = player;
                printf("System (Medium) blocks at %d\n", i + 1);
                return;
            }
            board[row][col] = temp;
        }
    }

    if (board[1][1] != PLAYER_X && board[1][1] != PLAYER_O) {
        board[1][1] = player;
        printf("System (Medium) takes center\n");
        return;
    }

    int corners[4][2] = {{0,0},{0,2},{2,0},{2,2}};
    for (int i = 0; i < 4; i++) {
        int r = corners[i][0], c = corners[i][1];
        if (board[r][c] != PLAYER_X && board[r][c] != PLAYER_O) {
            board[r][c] = player;
            printf("System (Medium) plays corner\n");
            return;
        }
    }

    make_random_move(player);
}

void make_best_move(char player) {
    int best_score = -1000, move_row = -1, move_col = -1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != PLAYER_X && board[i][j] != PLAYER_O) {
                char temp = board[i][j];
                board[i][j] = player;
                int score = minimax(board, 0, 0);
                board[i][j] = temp;
                if (score > best_score) {
                    best_score = score;
                    move_row = i;
                    move_col = j;
                }
            }
    board[move_row][move_col] = player;
    printf("System (Hard) plays at position %d\n", move_row * 3 + move_col + 1);
}

int minimax(char b[3][3], int depth, int is_max) {
    if (check_winner(PLAYER_O)) return 10 - depth;
    if (check_winner(PLAYER_X)) return depth - 10;
    if (is_draw()) return 0;

    int best_score = is_max ? -1000 : 1000;
    char current = is_max ? PLAYER_O : PLAYER_X;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (b[i][j] != PLAYER_X && b[i][j] != PLAYER_O) {
                char temp = b[i][j];
                b[i][j] = current;
                int score = minimax(b, depth + 1, !is_max);
                b[i][j] = temp;
                best_score = is_max
                    ? (score > best_score ? score : best_score)
                    : (score < best_score ? score : best_score);
            }
    return best_score;
}

int check_winner(char p) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == p && board[i][1] == p && board[i][2] == p) return 1;
        if (board[0][i] == p && board[1][i] == p && board[2][i] == p) return 1;
    }
    if (board[0][0] == p && board[1][1] == p && board[2][2] == p) return 1;
    if (board[0][2] == p && board[1][1] == p && board[2][0] == p) return 1;
    return 0;
}

int is_draw() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != PLAYER_X && board[i][j] != PLAYER_O)
                return 0;
    return 1;
}