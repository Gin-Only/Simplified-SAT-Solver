#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 9

void printSudoku(FILE *file, int sudoku[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fprintf(file, "%d ", sudoku[i][j]);
        }
        fprintf(file, "\n");
    }
}

int isValid(int sudoku[SIZE][SIZE], int row, int col, int num) {
    for (int i = 0; i < SIZE; i++) {
        if (sudoku[row][i] == num || sudoku[i][col] == num) {
            return 0;
        }
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (sudoku[startRow + i][startCol + j] == num) {
                return 0;
            }
        }
    }

    return 1;
}

int solveSudoku(int sudoku[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (sudoku[row][col] == 0) {
                for (int num = 1; num <= SIZE; num++) {
                    if (isValid(sudoku, row, col, num)) {
                        sudoku[row][col] = num;

                        if (solveSudoku(sudoku)) {
                            return 1;
                        }

                        sudoku[row][col] = 0;
                    }
                }
                return 0;
            }
        }
    }
    return 1;
}

void generateSudoku(int sudoku[SIZE][SIZE]) {
    srand(time(NULL));
    solveSudoku(sudoku);

    int cellsToRemove = SIZE * SIZE / 2;
    while (cellsToRemove > 0) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        if (sudoku[row][col] != 0) {
            sudoku[row][col] = 0;
            cellsToRemove--;
        }
    }
}

int main() {
    int sudoku[SIZE][SIZE] = {0};

    generateSudoku(sudoku);

    FILE *file = fopen("sudoku_problem.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    fprintf(file, "Generated Sudoku Problem:\n");
    printSudoku(file, sudoku);

    fclose(file);

    return 0;
}
