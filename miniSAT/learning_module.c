// learning_module.c

#include <stdio.h>
#include <stdlib.h>
#include "learning_module.h"
// #include "decision_module.h"

int propagated_variable;

// 全局 LearningState 变量
LearningState learning_state;

 

int threshold;//是阈值，待调整，用于筛选得分高于它的变量。如果得分高于阈值，就将对应的文字添加到学到的子句中。 
int threshold_value; // 是阈值，待调整

int need_decision(int propagation_count, int num_variables){
    int threshold = num_variables / 10;

    if (propagation_count > threshold) {
        // 冲突分析次数超过阈值，需要进行学习
        return 1;
    } else {
        // 冲突分析次数未达到阈值，不需要进行学习
        return 0;
    }
}


int record_propagated_variable(LearningState* learning_state, int propagated_variable) {
    if (learning_state->num_propagated_variables < MAX_PROPAGATED_VARIABLES) {
        learning_state->propagated_variables[learning_state->num_propagated_variables++] = propagated_variable;
    } else {
        // 处理数组已满的情况，可以选择适当的策略
        printf("Warning: Max propagated variables reached.\n");
    }

    // 返回传播的变量的数量，或者根据需要返回其他信息
    return learning_state->num_propagated_variables;
}


void learning_handler(EventType event_type, DecisionState* decision_state, Formula* formula) {
    switch (event_type) {
        case PROPAGATION_COMPLETE_EVENT:
            printf("Propagation module completed.\n");

            // 更新状态：传播次数增加
            learning_state.propagation_count++;

            // 调用其他函数：检查是否需要进行决策
            if (need_decision(learning_state.propagation_count, formula->num_variables)) {
                // 调用决策模块，并传递决策模块的状态
                decision_module(decision_state, formula);
            }

            // 处理数据：记录传播的变量
            int propagated_variable;  // 你需要设置变量的值
            record_propagated_variable(&learning_state, propagated_variable);

            break;
        default:
            printf("Unknown event type.\n");
            break;  // 处理学习模块的事件
    }
}

void learning_complete_event(void) {
    notify_event(&(learning_state.observable));
}

void initialize_learning_module(LearningState* learning_state) {
    // 创建观察者对象
    Observer observer_learning = {learning_handler};

    // 注册观察者到被观察者
    learning_state->observable.notify = observer_learning.handle_event;

    // 其他初始化操作
    learning_state->learned_clauses = NULL;
    learning_state->num_learned_clauses = 0;
    learning_state->propagation_count = 0;
}

// 更新学习模块，添加学到的子句
void update_learning_state(LearningState* learning_state, Clause* learned_clause) {
    // 冗余性处理
    if (!is_clause_redundant(learned_clause, learning_state)) {
        learning_state->learned_clauses = (Clause**)realloc(
            learning_state->learned_clauses, (learning_state->num_learned_clauses + 1) * sizeof(Clause*)
        );
        learning_state->learned_clauses[learning_state->num_learned_clauses++] = learned_clause;
    } else {
        free_clause(learned_clause);
    }
}

// Watched literals 机制
void update_watched_literals(Formula* formula, int variable_assignment) {
    for (int i = 0; i < formula->num_clauses; ++i) {
        Clause* clause = &formula->clauses[i];

        // 检查 watched literals
        if (clause->watch1 == variable_assignment || clause->watch2 == variable_assignment) {
            // 子句已满足
            clause->satisfied = 1;
        } else {
            // 找到新的 watched literals
            for (int j = 0; j < clause->size; ++j) {
                int literal = clause->literals[j];
            if (!is_literal_assigned(literal, formula)) {
                    clause->watch1 = literal;
                    break;
                }
            }
        }
    }
}

// 使用 VSIDS 策略的变量选择
int select_variable(Formula* formula) {
    int max_score = 0;
    int selected_variable = 0;

    for (int i = 0; i < formula->num_variables; ++i) {
        int variable_score = calculate_variable_score(i, formula);
        if (variable_score > max_score) {
            max_score = variable_score;
            selected_variable = i + 1; // 变量索引从 1 开始
        }
    }

    return selected_variable;
}

// 单子句传播
void unit_clause_propagation(Formula* formula, int assigned_literal) {
    for (int i = 0; i < formula->num_clauses; ++i) {
        Clause* clause = &formula->clauses[i];

        if (!clause->satisfied) {
            for (int j = 0; j < clause->size; ++j) {
                int literal = clause->literals[j];
                if (literal == assigned_literal) {
                    // 单子句中的文字被赋值，传播更新
                    update_watched_literals(formula, assigned_literal);
                    break;
                }
            }
        }
    }
}

void handle_conflict(LearningState* learning_state, Formula* formula) {
    // 处理冲突，学习新的子句
    // 这里只是一个简化的示例
    // 实际中可能需要更复杂的学习算法

    Clause conflict_clause;
    initialize_clause(&conflict_clause);

    // 根据冲突情况生成学到的子句
    // 这里假设直接将未赋值的变量添加到学到的子句中
    for (int i = 0; i < formula->num_variables; ++i) {
        if (formula->variables[i].value == 0) {
            int literal = (i + 1) * -1; // 将未赋值的变量的负值添加到学到的子句中
            conflict_clause.literals = (int*)realloc(conflict_clause.literals, (conflict_clause.size + 1) * sizeof(int));
            conflict_clause.literals[conflict_clause.size++] = literal;
        }
    }

    // 更新学到的子句
    update_learning_state(learning_state, &conflict_clause);
}



// 判断学到的子句是否冗余
int is_clause_redundant(Clause* learned_clause, LearningState* learning_state) {
    for (int i = 0; i < learning_state->num_learned_clauses; ++i) {
        Clause* existing_clause = learning_state->learned_clauses[i];

        // 检查子句是否与已学到的子句重复
        if (are_clauses_identical(learned_clause, existing_clause)) {
            return 1;  // 学到的子句是冗余的
        }
    }

    return 0;  // 学到的子句不冗余
}

// void free_clause(Clause* clause) {
//     // 实现释放子句内存的逻辑
//     free(clause->literals);
//     // 其他释放内存的逻辑
// }



void learning_module(LearningState* learning_state) {
    // 初始化学习模块的状态
    initialize_learning_module(learning_state); // 使用函数参数传入的 learning_state

    Formula formula;
    // TODO: 在这里进行单子句传播等操作

    // 选择变量
    int selected_variable = select_variable(&formula);

    // 更新 watched literals
    update_watched_literals(&formula, selected_variable);

    // 执行单子句传播
    unit_clause_propagation(&formula, selected_variable);

    // 处理冲突
    handle_conflict(learning_state, &formula);
     
    learning_complete_event();  // 修正这行

    // 释放学习模块的状态
    for (int i = 0; i < learning_state->num_learned_clauses; ++i) {
        free_clause(learning_state->learned_clauses[i]);
    }
    free(learning_state->learned_clauses);
}