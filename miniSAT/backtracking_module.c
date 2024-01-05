

#include <windows.h>
#include <psapi.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
// #include "decision_module.h"
#include "backtracking_module.h"

size_t get_memory_usage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;

    // 设置结构体大小
    pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);

    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(PROCESS_MEMORY_COUNTERS_EX))) {
        // 成功获取内存信息，可以使用 pmc 结构中的数据
        return pmc.PrivateUsage;
    } else {
        // 处理错误（例如，打印错误消息或返回一个默认值）
        fprintf(stderr, "获取内存使用情况时出错。\n");
        return 0; // 或其他适当的默认值
    }
}

double get_cpu_utilization() {
    FILETIME idleTime, kernelTime, userTime;
    GetSystemTimes(&idleTime, &kernelTime, &userTime);

    ULONGLONG idleTimeValue = ((ULONGLONG)idleTime.dwHighDateTime << 32) | idleTime.dwLowDateTime;
    ULONGLONG kernelTimeValue = ((ULONGLONG)kernelTime.dwHighDateTime << 32) | kernelTime.dwLowDateTime;
    ULONGLONG userTimeValue = ((ULONGLONG)userTime.dwHighDateTime << 32) | userTime.dwLowDateTime;

    ULONGLONG totalTime = kernelTimeValue + userTimeValue;
    ULONGLONG busyTime = totalTime - idleTimeValue;

    return ((double)busyTime / totalTime) * 100.0;
}

int find_last_assigned_variable(VariableAssignment* assignment) {
    for (int i = assignment->num_variables - 1; i >= 0; --i) {
        if (assignment->values[i] != UNASSIGNED) {
            return i;
        }
    }
    return -1; // 未找到已分配的变量
}

static Observable backtracking_module = {BACKTRACK_COMPLETE_EVENT, NULL};

BacktrackInfo backtrack_info;

int backtrack_count = 0;

void backtrack_start(VariableAssignment* assignment) {
    if (backtrack_info.backtrack_depth < MAX_BACKTRACK_LEVELS) {
        memcpy(&backtrack_info.backtrack_levels[backtrack_info.backtrack_depth], assignment->values, sizeof(int) * assignment->num_variables);
    }
}

void learn_clause1(Clause* new_clause) {
    if (backtrack_info.num_learned_clauses < MAX_LEARNED_CLAUSES) {
        memcpy(&backtrack_info.learned_clauses[backtrack_info.num_learned_clauses], new_clause, sizeof(Clause));
        backtrack_info.num_learned_clauses++;
    }
}

void update_performance_info() {
    backtrack_info.memory_usage = get_memory_usage();
    backtrack_info.cpu_utilization = get_cpu_utilization();
}

void record_backtrack_info() {
    printf("Recording backtrack information...\n");
    printf("Backtrack Depth: %d\n", backtrack_info.backtrack_depth);

    printf("Conflict Variables: ");
    for (int i = 0; i < backtrack_info.num_conflict_variables; ++i) {
        printf("%d ", backtrack_info.conflict_variables[i]);
    }
    printf("\n");

    clock_t elapsed_time = backtrack_info.backtrack_end_time - backtrack_info.backtrack_start_time;
    double elapsed_seconds = ((double)elapsed_time) / CLOCKS_PER_SEC;
    printf("Backtrack Time: %.4f seconds\n", elapsed_seconds);
}

void backtrack_handler(EventType event_type, DecisionState* decision_state) {
    switch (event_type) {
    case BACKTRACK_COMPLETE_EVENT:
        printf("Backtrack module completed.\n");
        backtrack_count++;

        // 检查是否需要传播
        if (backtrack_info.is_propagation_mode) {
            propagate(backtrack_info.formula, backtrack_info.value,&backtrack_info.is_propagation_mode);

        }

        // 更新性能信息
        update_performance_info();
        
        // 回溯到前一个决策点
        if (decision_state != NULL) {
            perform_backtrack(decision_state);
        }

        record_backtrack_info();
        break;
    default:
        printf("Unknown event type.\n");
        break;
    }
}

void initialize_backtracking_module(BacktrackingState* backtracking_state, int is_propagation_mode) {
    Observer observer_backtracking = {backtrack_handler};
    backtracking_state->observable.notify = observer_backtracking.handle_event;
    backtracking_state->learned_clauses = NULL;
    backtracking_state->num_learned_clauses = 0;

    // 初始化标志位和函数指针
    backtracking_state->backtrack_info.is_propagation_mode = is_propagation_mode;
    if (is_propagation_mode) {
        backtracking_state->backtrack_info.process_function = propagate;
    } else {
        backtracking_state->backtrack_info.process_function = backtrack;
    }
}

void backtrack(Formula* formula, VariableAssignment* assignment, PropagationState* propagation_state) {
    backtrack_info.backtrack_start_time = clock();

    int backtrack_level = 0;
    int last_assigned_variable = find_last_assigned_variable(assignment);

    while (last_assigned_variable != -1) {
        assignment->values[last_assigned_variable] = UNASSIGNED;
        formula->is_conflict = 0;
        propagate(formula, assignment, propagation_state);

        if (!formula->is_conflict) {
            break;
        }

        last_assigned_variable = find_last_assigned_variable(assignment);
        backtrack_level++;
    }

    backtrack_info.backtrack_depth = backtrack_level;
    backtrack_info.backtrack_end_time = clock();
    record_backtrack_info();
}

// 回溯函数
void perform_backtrack(DecisionState* decision_state) {
    // 在这里添加回溯的逻辑，根据需要回溯到前一个决策点
    backtrack(decision_state->formula, decision_state->assignment, decision_state->conflict_analysis_state);
}
