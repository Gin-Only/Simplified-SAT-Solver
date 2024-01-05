#include <stdio.h>
#include <stdlib.h>
#include "eightqueens_transform_cnf.h"

#define N 8
#define MAX_CLAUSES 1000



// 生成CNF子句，表示每行有且仅有一个皇后
int generateRowClauses(Clause clauses[MAX_CLAUSES], int clauseIndex) {
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            clauses[clauseIndex].literals[0] = -(i * N + i + 1);
            clauses[clauseIndex].literals[1] = -(j * N + j + 1);
            clauses[clauseIndex].literals[2] = 0;
            clauseIndex++;
        }
    }
    return clauseIndex;
}

// 生成CNF子句，表示每列有且仅有一个皇后
int generateColumnClauses(Clause clauses[MAX_CLAUSES], int clauseIndex) {
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            clauses[clauseIndex].literals[0] = -(i + N * i + 1);
            clauses[clauseIndex].literals[1] = -(j + N * j + 1);
            clauses[clauseIndex].literals[2] = 0;
            clauseIndex++;
        }
    }
    return clauseIndex;
}

// 生成CNF子句，表示每个对角线上有且仅有一个皇后
int generateDiagonalClauses(Clause clauses[MAX_CLAUSES], int clauseIndex) {
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            clauses[clauseIndex].literals[0] = -(i * N + i + 1);
            clauses[clauseIndex].literals[1] = -(j * N + j + 1);
            clauses[clauseIndex].literals[2] = 0;
            clauseIndex++;

            clauses[clauseIndex].literals[0] = -(i + N * i + 1);
            clauses[clauseIndex].literals[1] = -(j + N * j + 1);
            clauses[clauseIndex].literals[2] = 0;
            clauseIndex++;
        }
    }
    return clauseIndex;
}

// 生成CNF文件
void eightqueens_transform_cnf(Formula* eightqueens_formula) {
    FILE *file = fopen("eightqueens_problem.txt", "w");
    if (file == NULL) {
        perror("Error opening CNF file");
        return;
    }

    Clause clauses[MAX_CLAUSES];
    int clauseIndex = 0;

    // 生成CNF子句，表示每行有且仅有一个皇后
    clauseIndex = generateRowClauses(clauses, clauseIndex);

    // 生成CNF子句，表示每列有且仅有一个皇后
    clauseIndex = generateColumnClauses(clauses, clauseIndex);

    // 生成CNF子句，表示每个对角线上有且仅有一个皇后
    clauseIndex = generateDiagonalClauses(clauses, clauseIndex);

    // 写入CNF文件头
    fprintf(file, "p cnf %d %d\n", N * N, clauseIndex);

    // 写入CNF子句
    for (int i = 0; i < clauseIndex; i++) {
        fprintf(file, "%d %d %d 0\n", clauses[i].literals[0], clauses[i].literals[1], clauses[i].literals[2]);
    }

    fclose(file);
}

