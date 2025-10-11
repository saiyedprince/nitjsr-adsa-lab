#include <stdio.h>
#include <stdbool.h>

#define MAX 20  // Maximum board size

int board[MAX];
int N;

bool isSafe(int row, int col) {
    for (int i = 0; i < row; i++) {
        if (board[i] == col || 
            board[i] - i == col - row || 
            board[i] + i == col + row)
            return false;
    }
    return true;
}

void printSolution() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i] == j)
                printf("Q ");
            else
                printf(". ");
        }
        printf("\n");
    }
    printf("\n");
}

void solveNQueens(int row) {
    if (row == N) {
        printSolution();
        return;
    }

    for (int col = 0; col < N; col++) {
        if (isSafe(row, col)) {
            board[row] = col;
            solveNQueens(row + 1);
        }
    }
}

int main() {
    printf("Enter the value of N (number of queens): ");
    scanf("%d", &N);

    if (N < 1 || N > MAX) {
        printf("Please enter a value between 1 and %d.\n", MAX);
        return 1;
    }

    printf("Solutions to the %d-Queens problem:\n\n", N);
    solveNQueens(0);
    return 0;
}