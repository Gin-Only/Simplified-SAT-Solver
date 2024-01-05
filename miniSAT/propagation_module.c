// propagation_module.c
#include "propagation_module.h"

#include <stdio.h>
#include <stdlib.h>

void propagation_handler(EventType event_type, PropagationState* propagation_state, DecisionState* decision_state, Formula* formula) {
    switch (event_type) {
        case PROPAGATION_COMPLETE_EVENT:
            printf("Propagation module completed.\n");

            // 更新状态：传播次数增加
            propagation_state->propagation_count++;

            // 调用其他函数：检查是否需要进行决策
            if (need_decision(propagation_state->propagation_count, formula->num_variables)) {
                // 调用决策模块，并传递决策模块的状态
                decision_module(decision_state, formula);
            }

            // 处理数据：记录传播的变量
            int propagated_variable;  // 你需要设置变量的值
            record_propagated_variable(propagated_variable);

            break;
        default:
            printf("Unknown event type.\n");
            break;  // 处理传播模块的事件
    }
}

void propagate_complete_event(PropagationState* propagation_state) {
    notify_event(&(propagation_state->observable));
}

// 初始化函数
void initialize_propagation_module(PropagationState* propagation_state) {
    // 创建观察者对象
    Observer observer_propagation = {propagation_handler, propagation_state};

    // 注册观察者到被观察者
    propagation_state->observable.notify = observer_propagation.handle_event;

    // 其他初始化操作
    propagation_state->score = 0;
    propagation_state->selected_variable = 0;
    propagation_state->num_learned_clauses = 0;
    initialize_clause(&propagation_state->conflict_clauses);
    initialize_clause(&propagation_state->learned_clauses);
    propagation_state->propagation_count = 0;  // 初始化 propagation_count
}

void propagate(Formula* formula, VariableAssignment* assignment, PropagationState* propagation_state) {
    // 根据变量赋值传播更新公式

    // 遍历公式中的每个子句
    for (int i = 0; i < formula->num_clauses; ++i) {
        Clause* clause = &formula->clauses[i];

        // 遍历子句中的每个文字
        for (int j = 0; j < clause->size; ++j) {
            int literal = clause->literals[j];
            int variable_index = abs(literal) - 1;

            // 检查文字对应的变量是否有赋值
            if (assignment->values[variable_index] == 0) {
                // 变量没有赋值，检查文字是否是单子句
                if (is_unit_clause(clause)) {
                    // 单子句中的文字没有赋值，产生冲突
                    formula->is_conflict = 1;

                    // 触发 PROPAGATION_COMPLETE_EVENT 事件
                    propagate_complete_event(propagation_state);
                    return;
                }
            } else {
                // 变量有赋值，删除子句中包含这个文字的部分
                remove_literal_from_clause(clause, literal);
                --j; // 由于删除了一个文字，需要调整索引
            }
        }

        // 检查子句是否为空（冲突）
        if (is_empty_clause(clause)) {
            formula->is_conflict = 1;

            // 触发 PROPAGATION_COMPLETE_EVENT 事件
            propagate_complete_event(propagation_state);
            return;
        }
    }
}

void progation_module(PropagationState* propagation_state) {
    Formula formula;
    VariableAssignment assignment;

    // 先进行初始化
    initialize_propagation_module(propagation_state);

    propagate(&formula, &assignment, propagation_state);

    // 其他操作
}
