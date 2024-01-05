#ifndef BACKTRACKING_MODULE_H
#define BACKTRACKING_MODULE_H

// #include <time.h>
#include <stddef.h>

// #include "sat_parser.h"
// #include "sat_data_structure.h"
#include "propagation_module.h"
// #include "observer.h"
// #include "state.h"

// #define MAX_BACKTRACK_LEVELS 100   // 适当定义回溯的最大层级
// #define MAX_LEARNED_CLAUSES 1000    // 适当定义学到的子句的最大数量
// #define MaxVariables 1000          // 适当定义变量的最大数量
//以上适度调整

// 替代定义 clock_t 和 size_t
#ifndef _CLOCK_T_DEFINED
typedef long clock_t;
#define _CLOCK_T_DEFINED
#endif

#ifndef _SIZE_T_DEFINED
typedef unsigned int size_t;
#define _SIZE_T_DEFINED
#endif

// // 特定需要传递的回溯模块信息
// typedef struct {
//     int backtrack_depth;  // 记录回溯的层级
//     int conflict_variables[MaxVariables];  // 记录导致冲突的变量
//     VariableAssignment* value;
//     Formula* formula;
//     VariableAssignment backtrack_levels[MAX_BACKTRACK_LEVELS];
//     Clause learned_clauses[MAX_LEARNED_CLAUSES];
//     int num_learned_clauses;
//     int num_conflict_variables;  // 记录导致冲突的变量的数量
//     clock_t backtrack_start_time;  // 记录回溯开始的时间
//     clock_t backtrack_end_time;    // 记录回溯结束的时间
//     size_t memory_usage;  // 记录回溯过程中的内存使用情况
//     double cpu_utilization;  // 记录回溯过程中的 CPU 利用率
//     int is_propagation_mode;
//     void (*process_function)(Formula*, VariableAssignment*);//设置了标志位和函数指针，根据 is_propagation_mode 的值选择传播模式或决策模式
//     // 其他成员，根据需要添加
// } BacktrackInfo;

// // 回溯模块的状态
// typedef struct {
//    Observable observable; // 可以存储一些回溯模块的状态信息
//    Clause** learned_clauses;  // 存储学到的子句
//    int num_learned_clauses;
//    int backtrack_count;
//    BacktrackInfo backtrack_info;
// } BacktrackingState;

void record_backtrack_info(void);
void initialize_backtracking_module(BacktrackingState* backtracking_state, int is_propagation_mode);
void backtrack_handler(EventType event_type, DecisionState* decision_state) ;
void backtrack_complete_event(void);
//void propagate(Formula* formula, VariableAssignment* assignment,PropagationState* propagation_state);
int find_last_assigned_variable(VariableAssignment* value);
void backtrack(Formula* formula, VariableAssignment* assignment, PropagationState* propagation_state);


#endif // BACKTRACKING_MODULE_H
