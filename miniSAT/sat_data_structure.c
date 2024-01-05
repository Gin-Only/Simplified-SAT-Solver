// sat_data_structure.c

#include "sat_data_structure.h"
#include <stdio.h>
#include <stdlib.h>

void initialize_variable(Variable* var, int id) {
    var->id = id;
    var->value = 0;
}

void initialize_clause(Clause* clause) {
    clause->literals = NULL;
    clause->size = 0;
    clause->watch1 = 0;
    clause->watch2 = 0;
    clause->satisfied = 0;
    // Initialize other members if necessary
}

Formula initialize_formula(int num_variables, int num_clauses) {
    Formula formula;
    formula.num_variables = num_variables;
    formula.num_clauses = num_clauses;

    formula.variables = (Variable*)malloc(num_variables * sizeof(Variable));
    formula.clauses = (Clause*)malloc(num_clauses * sizeof(Clause));

    for (int i = 0; i < num_variables; ++i) {
        initialize_variable(&formula.variables[i], i + 1);
    }

    for (int i = 0; i < num_clauses; ++i) {
        initialize_clause(&formula.clauses[i]);
    }

    return formula;
}

// 判断是否为单子句
int is_unit_clause(Clause* clause) {
    return (clause->size == 1 && !clause->satisfied);
}

// 复制一个子句的内容到另一个子句
void copy_clause(Clause* dest, const Clause* src) {
    // 初始化目标子句
    dest->literals = (int*)malloc(src->size * sizeof(int));
    dest->size = src->size;
    dest->watch1 = src->watch1;
    dest->watch2 = src->watch2;
    dest->satisfied = src->satisfied;

    // 复制文字到目标子句
    for (int i = 0; i < src->size; ++i) {
        dest->literals[i] = src->literals[i];
    }
}

// 从子句中移除文字
void remove_literal_from_clause(Clause* clause, int literal) {
    // 创建一个新的子句来存储移除文字后的结果
    Clause new_clause;
    initialize_clause(&new_clause);

    // 遍历原始子句，将不是要移除的文字添加到新的子句中
    for (int i = 0; i < clause->size; ++i) {
        if (clause->literals[i] != literal) {
            clause->literals = (int*)realloc(clause->literals, (clause->size + 1) * sizeof(int));
            clause->literals[clause->size++] = literal;
        }
    }

    // 释放原始子句的内存
    free_clause(clause);

    // 将新子句的内容复制回原始子句
    copy_clause(clause, &new_clause);
}

    

// 判断子句是否为空
int is_empty_clause(Clause* clause) {
    return (clause->size == 0 && !clause->satisfied);
}

/*int is_literal_assigned(int literal, Formula* formula, VariableAssignment* assignment) {
    int variable_id = abs(literal);
    int assigned_value = assignment->values[variable_id];

    if (literal > 0) {
        return assigned_value == 1;
    } else {
        return assigned_value == -1;
    }
}*/

int is_literal_assigned(int literal, Formula* formula) {
    int variable_index = abs(literal) - 1;
    int variable_value = formula->variables[variable_index].value;

    if ((literal > 0 && variable_value == 1) || (literal < 0 && variable_value == -1)) {
        // 正文字对应的变量被赋值为1，或负文字对应的变量被赋值为-1
        return 1;  // 被赋值
    } else {
        return 0;  // 未被赋值
    }
}

// 判断两个子句是否相同
int are_clauses_identical(Clause* clause1, Clause* clause2) {
    // 检查子句大小是否相同
    if (clause1->size != clause2->size) {
        return 0;  // 子句大小不同，不相同
    }

    // 检查子句中的每个文字是否相同
    for (int i = 0; i < clause1->size; ++i) {
        if (clause1->literals[i] != clause2->literals[i]) {
            return 0;  // 子句中的文字不同，不相同
        }
    }

    // 子句相同
    return 1;
}



void free_variable(Variable* var) {
    // 没有额外需要释放的内存
}

void free_clause(Clause* clause) {
    free(clause->literals);
}

void free_formula(Formula* formula) {
    for (int i = 0; i < formula->num_clauses; ++i) {
        free_clause(&formula->clauses[i]);
    }

    free(formula->variables);
    free(formula->clauses);
}


