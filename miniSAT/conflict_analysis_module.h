// conflict_analysis_module.h

#ifndef CONFLICT_ANALYSIS_MODULE_H
#define CONFLICT_ANALYSIS_MODULE_H

// Forward declarations from learning_module.h
// #include "sat_parser.h"
// #include "sat_data_structure.h"
//#include "learning_module.h"
// #include "propagation_module.h"
// #include "observer.h"
#include "state.h"
// struct LearningState;

// typedef struct {
//     Observable observable;
//     Clause* conflict_clauses;
//     Clause* learned_clauses;
//     int num_conflict_clauses;  
//     int num_learned_clauses; 
//     int analysis_counter;  // 添加分析次数的计数器
//     struct LearningState*learning_state; // 可以存储一些冲突分析模块的状态信�?
// } ConflictAnalysisState;


// 在冲突分析模块中触发事件
void conflict_analysis_handler(EventType event_type, ConflictAnalysisState* conflict_analysis_state, Formula* formula);
// 检查是否需要学�?
 int need_learning(int analysis_counter, int num_variables, Formula* formula);

int record_conflict_analysis_info(Clause* conflict_clauses, Clause* learned_clauses);

// 初始化冲突分析模�?
void initialize_conflict_analysis_module(ConflictAnalysisState* conflict_analysis_state);

// 分析冲突
//void analyze_conflict(Formula* formula, VariableAssignment* assignment, LearningState* learning_state);

//得到冲突子句
 get_conflict_variable(Formula* formula, VariableAssignment* value);

// 函数声明
void conflict_analysis_module(ConflictAnalysisState* conflict_analysis_state, Formula* formula, VariableAssignment* value, int selected_variable) ;

// 学习子句
//Clause* learn_clause(Clause* conflict_clause, VariableAssignment* assignment);

// 更新学习状�?
//void update_learning_state(LearningState* learning_state, Clause* learned_clause);

#endif  // CONFLICT_ANALYSIS_MODULE_H


