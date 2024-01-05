// propagation_module.h

#ifndef PROPAGATION_MODULE_H
#define PROPAGATION_MODULE_H

// #include "sat_data_structure.h"
// #include "decision_module.h"
// #include "observer.h"//监视
#include "state.h"

// // 传播模块的状态
// typedef struct {
//    Observable observable;
//    int score;//出现次数
//    int selected_variable;//基于VSIDS策略的变量选择
//    int num_learned_clauses; 
//    Clause conflict_clauses;//学习到的子句个数
//    Clause learned_clauses; 
//    int propagation_count;// 可以存储一些传播模块的状态信息
//    DecisionState*decision_state;
// } PropagationState;

// 初始化传播模块
void initialize_propagation_module(PropagationState* propagation_state);
void propagation_handler(EventType event_type, PropagationState* propagation_state, DecisionState* decision_state, Formula* formula) ;
// 在传播模块中触发事件
void propagate_complete_event(PropagationState* propagation_state);
// 根据变量赋值传播更新公式
void propagate(Formula* formula, VariableAssignment* assignment, PropagationState* propagation_state);


#endif // PROPAGATION_MODULE_H
