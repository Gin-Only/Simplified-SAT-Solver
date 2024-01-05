// decision_module.c

#include <stddef.h>
#include <stdio.h>
#include "decision_module.h"
#include <stdlib.h>

// 全局的 Observable 对象
//static Observable decision_modules= {DECISION_COMPLETE_EVENT, NULL};

// 全局 LearningState 变量
DecisionState decision_state;
Formula* formula;
VariableAssignment* value;


// 观察者处理函数
void decision_handler(EventType event_type, ConflictAnalysisState* conflict_analysis_state, int selected_variable, Formula* formula, VariableAssignment* value) {
    switch (event_type) {
        case DECISION_COMPLETE_EVENT:
            printf("Decision module completed.\n");

            // 调用冲突分析模块
            conflict_analysis_module(conflict_analysis_state, formula, value, selected_variable);

            break;
        default:
            printf("Unknown event type.\n");
            break;
    }
}

void initialize_decision_module(DecisionState* decision_state, int num_variables) {
    // Create observer object
    Observer observer_decision = {decision_handler};

    // Register observer to the observable
    decision_state->observable.notify = observer_decision.handle_event;

    // Initialize decision count
    decision_state->decision_count = 0;

    decision_state->decision_stack = (int*)malloc(num_variables * sizeof(int));
    decision_state->top = -1;
}

int make_decision(DecisionState* decision_state, Formula* formula) {
    int variable_id = -1;

    // 选择一个未赋值的变量
    for (int i = 0; i < formula->num_variables; ++i) {
        if (formula->variables[i].value == 0) {
            variable_id = i + 1;
            break;
        }
    }

    // 如果找到未赋值的变量，则进行决策
    if (variable_id != -1) {
        // 将变量的正负两种赋值都尝试一遍
        decision_state->decision_stack[++decision_state->top] = variable_id;
        formula->variables[variable_id - 1].value = 1;

        return variable_id;
    } else {
        return 0; // 所有变量都已赋值，无法再进行决策
    }
}
void backtrack1(DecisionState* decision_state, Formula* formula) {
    // 回溯到上一个决策点
    if (decision_state->top >= 0) {
        int variable_id = decision_state->decision_stack[decision_state->top--];
        // 反转变量的赋值
        formula->variables[variable_id - 1].value = 0;
    }
}

int unit_propagation(Formula* formula) {
    // 执行单子句传播，返回值表示是否发生了冲突
    // 待修改为更复杂的单子句传播
    for (int i = 0; i < formula->num_clauses; ++i) {
        int unassigned_count = 0;
        int unassigned_variable = 0;

        for (int j = 0; j < formula->clauses[i].size; ++j) {
            int literal = formula->clauses[i].literals[j];
            int variable_id = abs(literal);
            int value = (literal > 0) ? 1 : -1;

            if (formula->variables[variable_id - 1].value == 0) {
                unassigned_count++;
                unassigned_variable = variable_id * value;
            }
        }

        // 如果只有一个未赋值的变量，对其进行赋值
        if (unassigned_count == 1) {
            formula->variables[abs(unassigned_variable) - 1].value = (unassigned_variable > 0) ? 1 : -1;
            return 1; // 单子句传播成功
        }
    }

    return 0; // 没有发生冲突
}

void decision_module(DecisionState* decision_state, Formula* formula) {
    // 初始化决策模块
    initialize_decision_module(decision_state, formula->num_variables);

    // 选择变量的赋值
    int variable_id = make_decision(decision_state, formula);

    // 回溯到上一个决策点
    backtrack(decision_state, formula);

    // 更新公式状态，执行单子句传播
    int conflict = unit_propagation(formula);

    // 如果发生冲突，处理冲突
    if (conflict) {
        handle_conflict(decision_state, formula);
    }

    // 释放决策模块的状态
    free(decision_state->decision_stack);
}

