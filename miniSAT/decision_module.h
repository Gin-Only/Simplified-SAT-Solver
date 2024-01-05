// decision_module.h

#ifndef DECISION_MODULE_H
#define DECISION_MODULE_H

// #include "sat_data_structure.h"
// #include "conflict_analysis_module.h"
// #include "backtracking_module.h"
// #include "observer.h"
#include "state.h"
// 决策模块的状态
// typedef struct {
//     Observable observable;
//     int* decision_stack; // 存储变量的决策历史
//     int top;             // 决策栈的栈顶指针,决策栈通常用于记录和管理在问题解空间中做出的各种决策。每当算法需要回溯（撤销之前的决策）时，可以通过弹出栈顶元素来恢复到之前的决策状态。这有助于在解空间中进行深度优先搜索，并且能够有效地跟踪和回溯算法的状态。
//     int decision_count;
//     Formula* formula;
//     VariableAssignment* assignment; 
//     ConflictAnalysisState*conflict_analysis_state;
//      } DecisionState;

void decision_handler(EventType event_type, ConflictAnalysisState* conflict_analysis_state, int selected_variable, Formula* formula, VariableAssignment* value);

// 在决策模块中触发事件
void decision_complete_event(void) ;
// 初始化决策模块
void initialize_decision_module(DecisionState* decision_state, int num_variables);

// 选择变量的赋值
int make_decision(DecisionState* decision_state, Formula* formula);

// 回溯到上一个决策点
void backtrack1(DecisionState* decision_state, Formula* formula);

void decision_module(DecisionState* decision_state, Formula* formula);

// 更新公式状态，执行单子句传播
int unit_propagation(Formula* formula);

#endif // DECISION_MODULE_H
