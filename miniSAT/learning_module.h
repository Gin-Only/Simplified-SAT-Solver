// learning_module.h

#ifndef LEARNING_MODULE_H
#define LEARNING_MODULE_H

// #include "sat_parser.h"
// #include "sat_data_structure.h"
// #include "decision_module.h"
// #include "conflict_analysis_module.h"
// #include "propagation_module.h" 
#include "observer.h"
#include "state.h"

#define MAX_PROPAGATED_VARIABLES 100  // 根据实际情况选择合适的大小

// typedef struct {
//     Observable observable;
//     Clause** learned_clauses;  // 存储学到的子句
//     int num_learned_clauses;
//     int propagation_count;  // 添加此行以跟踪传播次数
//     // 新增成员用于存储传播的变量
//     int propagated_variables[MAX_PROPAGATED_VARIABLES];
//     int num_propagated_variables;
// } LearningState;

int need_decision(int propagation_count, int num_variables);
void learning_handler(EventType event_type, DecisionState* decision_state, Formula* formula);
void initialize_learning_module(LearningState* learning_state);
void learning_complete_event(void);
// 在学习模块中触发事件
void update_watched_literals(Formula* formula, int variable_assignment);
int select_variable(Formula* formula);
void unit_clause_propagation(Formula* formula, int assigned_literal);
Clause* learn_clause(Clause* conflict_clause, VariableAssignment* assignment);
void handle_conflict(LearningState* learning_state, Formula* formula);
//int are_clauses_identical(Clause* clause1, Clause* clause2);
int is_clause_redundant(Clause* learned_clause, LearningState* learning_state);
void free_clause(Clause* clause);
void update_learning_state(LearningState* learning_state, Clause* learned_clause);
void learning_module(LearningState* learning_state);

#endif // LEARNING_MODULE_H
