// sat_data_structure.h

#ifndef SAT_DATA_STRUCTURE_H
#define SAT_DATA_STRUCTURE_H

#include "sat_parser.h"
#define  MAX_BACKTRACK_LEVELS 100
// 定义变量结构
/*typedef struct {
    int id;
    int value; // 0表示未赋值，1表示true，-1表示false
} Variable;*/

// 定义子句结构
/*typedef struct {
    int* literals; // 存储子句中的文字
    int size;       // 记录子句中包含的文字数量
    int watch1;
    int watch2;
    int satisfied;
} Clause;*/

// 定义公式结构
/*typedef struct {
    Variable* variables; // 存储变量
    int num_variables;
    Clause* clauses; // 存储子句
    int num_clauses;
} Formula;*/

typedef struct {
   int* values;//指向整数数组的指针，用于存储每个变量的赋值状态。
   int num_variables;//表示变量的数量。
   int backtrack_levels[MAX_BACKTRACK_LEVELS];
} VariableAssignment;

//定义赋值状态
typedef enum {
    UNASSIGNED,
    ASSIGNED_TRUE,
    ASSIGNED_FALSE
} AssignmentStatus;


// 初始化变量
void initialize_variable(Variable* var, int id);

// 初始化子句
void initialize_clause(Clause* clause);

// 初始化公式
Formula initialize_formula(int num_variables, int num_clauses);

int is_unit_clause(Clause* clause);

int are_clauses_identical(Clause* clause1, Clause* clause2);

void copy_clause(Clause* dest, const Clause* src);

void remove_literal_from_clause(Clause* clause, int literal);

 int is_empty_clause(Clause* clause);

 int is_literal_assigned(int literal, Formula* formula);

// 释放变量的内存
void free_variable(Variable* var);

// 释放子句的内存
void free_clause(Clause* clause);

// 释放公式的内存
void free_formula(Formula* formula);

#endif // SAT_DATA_STRUCTURE_H
