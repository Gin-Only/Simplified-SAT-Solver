#include "sat_data_structure.h"
#include "observer.h"//监视
#include <time.h>
typedef struct {
    Observable observable;
    Clause* conflict_clauses;
    Clause* learned_clauses;
    int num_conflict_clauses;  
    int num_learned_clauses; 
    int analysis_counter;  // 添加分析次数的计数器
    struct LearningState*learning_state; // 可以存储一些冲突分析模块的状态信�?
} ConflictAnalysisState;


typedef struct {
    Observable observable;
    int* decision_stack; // 存储变量的决策历史
    int top;             // 决策栈的栈顶指针,决策栈通常用于记录和管理在问题解空间中做出的各种决策。每当算法需要回溯（撤销之前的决策）时，可以通过弹出栈顶元素来恢复到之前的决策状态。这有助于在解空间中进行深度优先搜索，并且能够有效地跟踪和回溯算法的状态。
    int decision_count;
    Formula* formula;
    VariableAssignment* assignment; 
    ConflictAnalysisState*conflict_analysis_state;
     } DecisionState;

// 传播模块的状态
typedef struct {
   Observable observable;
   int score;//出现次数
   int selected_variable;//基于VSIDS策略的变量选择
   int num_learned_clauses; 
   Clause conflict_clauses;//学习到的子句个数
   Clause learned_clauses; 
   int propagation_count;// 可以存储一些传播模块的状态信息
   DecisionState*decision_state;
} PropagationState;

#define MAX_PROPAGATED_VARIABLES 100  // 根据实际情况选择合适的大小

typedef struct {
    Observable observable;
    Clause** learned_clauses;  // 存储学到的子句
    int num_learned_clauses;
    int propagation_count;  // 添加此行以跟踪传播次数
    // 新增成员用于存储传播的变量
    int propagated_variables[MAX_PROPAGATED_VARIABLES];
    int num_propagated_variables;
} LearningState;

#define MAX_BACKTRACK_LEVELS 100   // 适当定义回溯的最大层级
#define MAX_LEARNED_CLAUSES 1000    // 适当定义学到的子句的最大数量
#define MaxVariables 1000          // 适当定义变量的最大数量
// 特定需要传递的回溯模块信息
typedef struct {
    int backtrack_depth;  // 记录回溯的层级
    int conflict_variables[MaxVariables];  // 记录导致冲突的变量
    VariableAssignment* value;
    Formula* formula;
    VariableAssignment backtrack_levels[MAX_BACKTRACK_LEVELS];
    Clause learned_clauses[MAX_LEARNED_CLAUSES];
    int num_learned_clauses;
    int num_conflict_variables;  // 记录导致冲突的变量的数量
    clock_t backtrack_start_time;  // 记录回溯开始的时间
    clock_t backtrack_end_time;    // 记录回溯结束的时间
    size_t memory_usage;  // 记录回溯过程中的内存使用情况
    double cpu_utilization;  // 记录回溯过程中的 CPU 利用率
    int is_propagation_mode;
    void (*process_function)(Formula*, VariableAssignment*);//设置了标志位和函数指针，根据 is_propagation_mode 的值选择传播模式或决策模式
    // 其他成员，根据需要添加
} BacktrackInfo;

// 回溯模块的状态
typedef struct {
   Observable observable; // 可以存储一些回溯模块的状态信息
   Clause** learned_clauses;  // 存储学到的子句
   int num_learned_clauses;
   int backtrack_count;
   BacktrackInfo backtrack_info;
} BacktrackingState;