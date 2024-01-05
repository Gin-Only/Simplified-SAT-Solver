// conflict_analysis_module.c

#include "conflict_analysis_module.h"
#include <stdio.h>
#include <stdlib.h>


// 全局 ConflictAnalysisState 变量
ConflictAnalysisState conflict_analysis_state;

int threshold1;//是阈值，待调整，用于筛选得分高于它的变量。如果得分高于阈值，就将对应的文字添加到学到的子句中。 
int threshold_value1; // 是阈值，待调整,与learning模块区分


 
    int need_learning(int analysis_counter, int num_variables, Formula* formula) {
    // 阈值与变量总数成正比关系，此处待实验调整
    int threshold1 = formula->num_variables / 10;

    if (analysis_counter > threshold1) {
        // 冲突分析次数超过阈值，需要进行学习
        return 1;
    } else {
        // 冲突分析次数未达到阈值，不需要进行学习
        return 0;
    }
}


int record_conflict_analysis_info(Clause* conflict_clauses, Clause* learned_clauses) {
    // 在 ConflictAnalysisState 中记录冲突子句和学到的子句

    // 直接将冲突子句添加到数组
    conflict_analysis_state.conflict_clauses = (Clause*)realloc(
        conflict_analysis_state.conflict_clauses,
        (conflict_analysis_state.num_conflict_clauses + 1) * sizeof(Clause)
    );

    if (conflict_analysis_state.conflict_clauses == NULL) {
        // 处理内存分配失败的情况
        return -1;
    }

    conflict_analysis_state.conflict_clauses[conflict_analysis_state.num_conflict_clauses] = *conflict_clauses;//方括号[conflict_analysis_state.num_conflict_clauses]表示在数组中的特定索引处进行赋值。
    conflict_analysis_state.num_conflict_clauses++;

    // 直接将学到的子句添加到数组
    conflict_analysis_state.learned_clauses = (Clause*)realloc(
        conflict_analysis_state.learned_clauses,
        (conflict_analysis_state.num_learned_clauses + 1) * sizeof(Clause)
    );

    if (conflict_analysis_state.learned_clauses == NULL) {
        // 处理内存分配失败的情况
        free(conflict_analysis_state.conflict_clauses);  // 避免内存泄漏
        return -1;
    }

    conflict_analysis_state.learned_clauses[conflict_analysis_state.num_learned_clauses] = *learned_clauses;
    conflict_analysis_state.num_learned_clauses++;

    return 0;  // 返回值可以根据实际情况调整
}

void conflict_analysis_handler(EventType event_type, ConflictAnalysisState* conflict_analysis_state, Formula* formula) {
    switch (event_type) {
        case CONFLICT_ANALYSIS_COMPLETE_EVENT:
            printf("Conflict analysis module completed.\n");

            // 更新状态：冲突分析次数增加
            conflict_analysis_state->analysis_counter++;

            // 调用其他函数：检查是否需要进行学习
            if (need_learning(conflict_analysis_state->analysis_counter, formula->num_variables, formula)) {
                // 传递 learning_state 给 learning_module 函数
                learning_module(&conflict_analysis_state->learning_state);
            }

            // 处理数据：记录冲突分析的信息
            record_conflict_analysis_info(conflict_analysis_state->conflict_clauses, conflict_analysis_state->learned_clauses);

            break;
        default:
            printf("Unknown event type.\n");
            break;
    }
}




void conflict_analysis_complete_event(void) {
    notify_event(&(conflict_analysis_state.observable));
}


// 初始化函数
void initialize_conflict_analysis_module(ConflictAnalysisState* conflict_analysis_state) {
    // 创建观察者对象
    Observer observer_conflict_analysis = {&conflict_analysis_handler};

    // 注册观察者到被观察者
    conflict_analysis_state->observable.notify = observer_conflict_analysis.handle_event;

    threshold_value1 = 50;

    // 其他初始化操作
    initialize_clause(&(conflict_analysis_state->conflict_clauses));
    initialize_clause(&(conflict_analysis_state->learned_clauses));
    conflict_analysis_state->analysis_counter = 0;
}

/*void increment_analysis_counter(ConflictAnalysisState* conflict_analysis_state) {
    conflict_analysis_state->analysis_counter++;
}

int get_analysis_counter(const ConflictAnalysisState* conflict_analysis_state) {
    return conflict_analysis_state->analysis_counter;
}*/



// 在适当的位置实现 get_conflict_clause 函数
get_conflict_variable(Formula* formula, VariableAssignment* assignment) {
    // 遍历公式中的每个子句
    for (int i = 0; i < formula->num_clauses; ++i) {
        Clause* clause = &formula->clauses[i];

        // 遍历子句中的每个文字
        for (int j = 0; j < clause->size; ++j) {
            int literal = clause->literals[j];
            int variable_index = abs(literal) - 1;

            // 检查文字对应的变量是否有赋值
            if (assignment->values[variable_index] == 0) {
                // 变量没有赋值，返回导致冲突的子句
                return clause;
            }
        }
    }

    // 如果没有找到导致冲突的子句，返回 NULL
    return NULL;
}

void conflict_analysis_module(ConflictAnalysisState* conflict_analysis_state, Formula* formula, VariableAssignment* value, int selected_variable) {
    // 初始化操作
    Clause* clause;
    initialize_conflict_analysis_module(conflict_analysis_state);

    // 调用冲突分析处理函数，传递 formula、value 和 selected_variable
    conflict_analysis_handler(CONFLICT_ANALYSIS_COMPLETE_EVENT, conflict_analysis_state, formula);

    conflict_analysis_complete_event();

    // 释放资源
    free(clause->literals);
}


/*和学习模块重复了Clause* learn_clause_common(Clause* conflict_clause, VariableAssignment* assignment) {
    // 学习新的子句
    // 待修改更加复杂的算法

    Clause* learned_clause = (Clause*)malloc(sizeof(Clause));
    initialize_clause(learned_clause);

    // 根据冲突子句和当前变量赋值，生成学到的子句
    for (int i = 0; i < conflict_clause->size; ++i) {
        int literal = conflict_clause->literals[i];
        int variable_index = abs(literal) - 1;

        // 假设这里有一个函数 calculate_variable_score，根据变量得分进行选择
        int variable_score = calculate_variable_score1(variable_index, assignment);

        // 这里可以根据得分或其他启发式规则来决定是否添加到学到的子句中
        if (variable_score > threshold_value) {
            learned_clause->literals = (int*)realloc(learned_clause->literals, (learned_clause->size + 1) * sizeof(int));
            learned_clause->literals[learned_clause->size++] = literal;
        }
    }

    return learned_clause;
}

Clause* learn_clause(Clause* conflict_clause, VariableAssignment* assignment) {
    return learn_clause_common(conflict_clause, assignment);
}

void update_learning_state(LearningState* learning_state, Clause* learned_clause) {
    // 将学到的子句添加到学习模块中
    // 待加入一些冗余性处理等逻辑
    if (!is_clause_redundant(learned_clause, learning_state)) {
        learning_state->learned_clauses = (Clause**)realloc(learning_state->learned_clauses, (learning_state->num_learned_clauses + 1) * sizeof(Clause*));
        learning_state->learned_clauses[learning_state->num_learned_clauses++] = learned_clause;
    } else {
        free_clause(learned_clause);
    }
}

void learn_and_update_state(Clause* conflict_clause, VariableAssignment* assignment, LearningState* learning_state) {
    // 学习新的子句并将其添加到学习模块中
    Clause* learned_clause = learn_clause_common(conflict_clause, assignment);
    update_learning_state(learning_state, learned_clause);
}*/