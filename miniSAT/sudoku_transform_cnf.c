#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sudoku_transform_cnf.h"

#define SIZE 9
#define MAX_CLAUSES (SIZE * SIZE * SIZE*SIZE)


void printSudoku(FILE *file, int sudoku[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fprintf(file, "%d ", sudoku[i][j]);
        }
        fprintf(file, "\n");
    }
}

void sudokuToCNF(int sudoku[SIZE][SIZE], FILE *cnfFile) {
    int numClauses = SIZE * SIZE * SIZE*SIZE;
   

    // 在适当的位置添加以下代码，确保为clauses数组分配了内存
Clause*clauses = malloc(sizeof(Clause) * MAX_CLAUSES);
if (clauses == NULL) {
    perror("Failed to allocate memory for clauses");
    exit(EXIT_FAILURE);
}


    for (int i = 0; i < numClauses; i++) {
        clauses[i].literals = malloc(sizeof(int) * 3);
        if (clauses[i].literals == NULL) {
            perror("内存分配失败");
            exit(EXIT_FAILURE);
        }
    }

    int clauseIndex = 0;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            int number = sudoku[row][col];

            if (number != 0) {
                clauses[clauseIndex].literals[0] = row * SIZE * SIZE + col * SIZE + number;
                clauses[clauseIndex].literals[1] = 0;
                clauseIndex++;
            } else {
                for (int num = 1; num <= SIZE; num++) {
                    for (int k = num + 1; k <= SIZE; k++) {
                        clauses[clauseIndex].literals[0] = -(row * SIZE * SIZE + col * SIZE + num);
                        clauses[clauseIndex].literals[1] = -(row * SIZE * SIZE + col * SIZE + k);
                        clauses[clauseIndex].literals[2] = 0;
                        clauseIndex++;
                    }
                }

                for (int c = 1; c <= SIZE; c++) {
                    clauses[clauseIndex].literals[0] = row * SIZE * SIZE + col * SIZE + c;
                    for (int k = col + 1; k < SIZE; k++) {
                        clauses[clauseIndex].literals[1] = -(row * SIZE * SIZE + k * SIZE + c);
                        clauses[clauseIndex].literals[2] = 0;
                        clauseIndex++;
                    }
                }

                for (int r = 1; r <= SIZE; r++) {
                    clauses[clauseIndex].literals[0] = row * SIZE * SIZE + col * SIZE + r;
                    for (int k = row + 1; k < SIZE; k++) {
                        clauses[clauseIndex].literals[1] = -(k * SIZE * SIZE + col * SIZE + r);
                        clauses[clauseIndex].literals[2] = 0;
                        clauseIndex++;
                    }
                }

                int startRow = row - row % 3;
                int startCol = col - col % 3;
                for (int r = 1; r <= SIZE; r++) {
                    clauses[clauseIndex].literals[0] = row * SIZE * SIZE + col * SIZE + r;
                    for (int k = startRow; k < startRow + 3; k++) {
                        for (int l = startCol; l < startCol + 3; l++) {
                            if (k != row || l != col) {
                                clauses[clauseIndex].literals[1] = -(k * SIZE * SIZE + l * SIZE + r);
                                clauses[clauseIndex].literals[2] = 0;
                                clauseIndex++;
                            }
                        }
                    }
                }
            }
        }
    }
int i;
   if (clauses[i].literals[2] != 0) {
    fprintf(cnfFile, "%d %d %d\n", clauses[i].literals[0], clauses[i].literals[1], clauses[i].literals[2]);
} else {
    fprintf(cnfFile, "%d %d\n", clauses[i].literals[0], clauses[i].literals[1]);
}


    for (int i = 0; i < numClauses; i++) {
        fprintf(cnfFile, "%d %d %d\n", clauses[i].literals[0], clauses[i].literals[1], clauses[i].literals[2]);
    }

    for (int i = 0; i < numClauses; i++) {
        free(clauses[i].literals);
    }

    free(clauses);
}

int sudoku_transform_cnf(Formula *sudoku_formula) {
    int sudoku[SIZE][SIZE] = {0};
    int clauseIndex = 0;

    // 从文件中读取数独问题
    FILE *file = fopen("sudoku_problem.txt", "r");
    if (file == NULL) {
        perror("打开文件时出错");
        return 1;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (clauseIndex < 0 || clauseIndex >= MAX_CLAUSES) {
                fprintf(stderr, "clauseIndex的值无效\n");
                exit(EXIT_FAILURE);
            }

            fscanf(file, "%d", &sudoku[i][j]);
        }
    }

    fclose(file);

    FILE *cnfFile = fopen("sudoku_problem.cnf", "w");
    if (cnfFile == NULL) {
        perror("打开CNF文件时出错");
        return 1;
    }

    sudokuToCNF(sudoku, cnfFile);

    fclose(cnfFile);

    return 0;
}


