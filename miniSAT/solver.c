#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// #include "sat_parser.h"
// #include "sat_data_structure.h"
// #include "observer.h"
// #include "learning_module.h"
// #include "propagation_module.h"
// #include "decision_module.h"
// #include "conflict_analysis_module.h"
#include "backtracking_module.h"
#include "sudoku_transform_cnf.h"
#include "eightqueens_transform_cnf.h"
// #include "state.h"



// 定义 Solver 结构，包含需要的状态和数据结构
typedef struct {
    InputParserState input_parser_state;
    LearningState learning_state;
    PropagationState propagation_state;
    DecisionState decision_state;
    ConflictAnalysisState conflict_analysis_state;
    BacktrackingState backtracking_state;
    BacktrackInfo backtrack_info;
} Solver;

typedef struct {
    double propagation_weight;
    double conflict_weight;
    double backtrack_depth_weight;
    double time_weight;
    double memory_usage_weight;
} WeightInfo;



// 初始化 Solver 模块
void initialize_solver(Solver* solver, Formula* formula) {
    initialize_input_parsing_module(&solver->input_parser_state);
    initialize_learning_module(&solver->learning_state);
    initialize_propagation_module(&solver->propagation_state);
    initialize_decision_module(&solver->decision_state,solver->input_parser_state.formula.num_variables);
    initialize_conflict_analysis_module(&solver->conflict_analysis_state);
    initialize_backtracking_module(&solver->backtracking_state, solver->backtrack_info.is_propagation_mode);
}

// 计算变量得分
int calculate_variable_score(int variable_index, Formula* formula, WeightInfo* weight_info) {
    int score = 0;
    Solver* solver;

    // 基于变量的出现次数更新分数
    for (int i = 0; i < formula->num_clauses; ++i) {
        Clause* clause = &formula->clauses[i];

        for (int j = 0; j < clause->size; ++j) {
            int literal = clause->literals[j];
            int variable_id = abs(literal);

            if (variable_id == variable_index) {
                // 假设每次出现都增加分数
                score++;
            }
        }
    }

    // 根据传播次数更新分数
    score += solver->learning_state.propagation_count +solver->propagation_state.propagation_count * weight_info->propagation_weight;

    // 根据冲突次数更新分数
    score += solver->conflict_analysis_state.analysis_counter * weight_info->conflict_weight;

    // 根据回溯深度更新分数
    score +=solver->backtrack_info.backtrack_depth * weight_info->backtrack_depth_weight;

    // 根据回溯所用处理时间更新分数
    score += (solver->backtrack_info.backtrack_end_time -solver->backtrack_info.backtrack_start_time) * weight_info->time_weight;

    // 根据当前回溯过程中的私有内存使用量更新分数
    score += solver->backtrack_info.cpu_utilization * weight_info->memory_usage_weight;

    return score;
}

// 根据指定标准更新变量分数的函数
void update_variable_scores(Formula* formula, WeightInfo* weight_info) {
    for (int i = 1; i <= formula->num_variables; ++i) {
        int score = calculate_variable_score(i, formula, weight_info);
        // 在你的数据结构中更新变量分数
        formula->variables[i - 1].score = score;
    }
}

// 执行加权随机选择的函数
int weighted_random_selection(Formula* formula) {
    int total_score = 0;
    for (int i = 1; i <= formula->num_variables; ++i) {
        total_score += formula->variables[i - 1].score;
    }

    if (total_score == 0) {
        // 处理除零错误的情况，可能需要选择其他适当的返回值或采取其他措施
        return -1;
    }

    int random_value = rand() % total_score;  // 生成一个介于 0 和 total_score 之间的随机值
    int cumulative_score = 0;

    for (int i = 1; i <= formula->num_variables; ++i) {
        cumulative_score += formula->variables[i - 1].score;
        if (cumulative_score >= random_value) {
            return i;  // 返回所选变量的索引
        }
    }

    // 在正常情况下，不应到达这一点
    return -1;
}

// 定义变量的权重调整函数
void adjust_weight_for_variable(WeightInfo* weight_info, int conflict_variable) {
    // 根据实际需求来调整权重能需要根据具体情况进行调整

    // 降低冲突变量的冲突权重
    weight_info->conflict_weight *= 0.9;

    // 如果有其他需要调整的权重，可以在这里添加相应的逻辑
}

// 释放 Solver 模块的资源
/*void release_solver_resources(Solver* solver) {
    Clause* clause;
    free(clause->literals);
}*/
void release_solver_resources(Solver* solver) {
    Clause* clause = malloc(sizeof(Clause));  // 根据实际情况分配内存
    // 执行其他释放资源的操作
    free(clause);  // 释放内存
}


//判定是否解决
int is_solved(Solver* solver) {
    for (int i = 0; i < solver->input_parser_state.formula.num_variables; ++i) {
        if (solver->input_parser_state.formula.variables[i].value == 0) {
            return 0;  // 存在未赋值的变量，未解决
        }
    }

    return 1;  // 所有变量都被赋值，已解决
}

// 打印数独格局的函数
void print_sudoku_grid(Formula* formula) {
    printf("Sudoku Grid:\n");

    // 创建一个数组来存储数独格局
    int sudoku_grid[9][9];

    // 遍历每个变量，根据变量值填充数独格局数组
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int variable_index = i * 9 + j;
            int variable_value = formula->variables[variable_index].value;

            // 将变量值映射到数独格局数组，变量值为0表示未填数字
            sudoku_grid[i][j] = (variable_value == 1) ? (j + 1) : 0;
        }
    }

    // 打印数独格局数组
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            printf("%2d ", sudoku_grid[i][j]);
        }
        printf("\n");
    }
}

// 判断是否为数独问题的函数
int is_sudoku_problem(Formula* formula) {
    // 检查变量和子句的数量
    if (formula->num_variables != 81 || formula->num_clauses <= 0) {
        return false;
    }

    // 检查每个数字在每行、每列和每个九宫格内都不能重复出现
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            for (int num = 1; num <= 9; ++num) {
                // 每个数字在每行只能出现一次
                int count_row = 0;
                for (int i = 0; i < 9; ++i) {
                    int variable_index = row * 9 + i;
                    if (formula->variables[variable_index].value == num) {
                        count_row++;
                    }
                }
                if (count_row > 1) {
                    return false;
                }

                // 每个数字在每列只能出现一次
                int count_col = 0;
                for (int i = 0; i < 9; ++i) {
                    int variable_index = i * 9 + col;
                    if (formula->variables[variable_index].value == num) {
                        count_col++;
                    }
                }
                if (count_col > 1) {
                    return false;
                }

                // 每个数字在每个九宫格内只能出现一次
                int count_square = 0;
                int start_row = 3 * (row / 3);
                int start_col = 3 * (col / 3);
                for (int i = start_row; i < start_row + 3; ++i) {
                    for (int j = start_col; j < start_col + 3; ++j) {
                        int variable_index = i * 9 + j;
                        if (formula->variables[variable_index].value == num) {
                            count_square++;
                        }
                    }
                }
                if (count_square > 1) {
                    return false;
                }
            }
        }
    }

    // 所有条件都满足，是数独问题
    return true;
}

// 打印八皇后问题的解法
void print_eight_queens_solution(Formula* formula) {
    printf("Eight Queens Solution:\n");

    // 创建一个数组来存储八皇后问题的棋盘
    int chessboard[8][8];

    // 遍历每个变量，根据变量值填充棋盘数组
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            int variable_index = i * 8 + j;
            int variable_value = formula->variables[variable_index].value;

            // 将变量值映射到棋盘数组，变量值为1表示该格子放置皇后
            chessboard[i][j] = variable_value;
        }
    }

    // 打印八皇后问题的棋盘数组
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (chessboard[i][j]) {
                printf("Q ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

// 判断是否为八皇后问题的函数
int is_eight_queens_problem(Formula* formula) {
    // 检查变量和子句的数量
    if (formula->num_variables != 64 || formula->num_clauses <= 0) {
        return false;
    }

    // 检查每行只能有一个皇后
    for (int row = 0; row < 8; ++row) {
        int count_row = 0;
        for (int col = 0; col < 8; ++col) {
            int variable_index = row * 8 + col;
            if (formula->variables[variable_index].value) {
                count_row++;
            }
        }
        if (count_row != 1) {
            return false;
        }
    }

    // 检查每列只能有一个皇后
    for (int col = 0; col < 8; ++col) {
        int count_col = 0;
        for (int row = 0; row < 8; ++row) {
            int variable_index = row * 8 + col;
            if (formula->variables[variable_index].value) {
                count_col++;
            }
        }
        if (count_col != 1) {
            return false;
        }
    }

    // 检查每个对角线只能有一个皇后
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (formula->variables[row * 8 + col].value) {
                for (int i = 1; i < 8; ++i) {
                    // 检查主对角线
                    if (row + i < 8 && col + i < 8) {
                        int variable_index = (row + i) * 8 + (col + i);
                        if (formula->variables[variable_index].value) {
                            return false;
                        }
                    }
                    // 检查副对角线
                    if (row + i < 8 && col - i >= 0) {
                        int variable_index = (row + i) * 8 + (col - i);
                        if (formula->variables[variable_index].value) {
                            return false;
                        }
                    }
                }
            }
        }

    }

    // 所有条件都满足，是八皇后问题
    return true;
}

// 执行 MiniSAT 求解
void solve(Formula* formula) {
    Solver solver;
    VariableAssignment* value;

    ParseResult parse_DIMACS_CNF(const char* file_path, Formula* cnf);

    initialize_solver(&solver, formula);

    // 初始化权重信息
    WeightInfo weight_info = {1.0, 1.0, 1.0, 1.0, 1.0};

    // 开始求解
    while (!is_solved(&solver)) {
        // 执行学习模块
        learning_handler(PROPAGATION_COMPLETE_EVENT, &solver.decision_state, formula);

        // 执行传播模块
        propagation_handler(PROPAGATION_COMPLETE_EVENT, &solver.propagation_state, &solver.decision_state, formula);

        // 更新变量得分
        update_variable_scores(formula, &weight_info);

        // 执行决策模块
        int selected_variable = weighted_random_selection(formula);
        decision_handler(DECISION_COMPLETE_EVENT, &solver.decision_state, selected_variable, formula, value);

        // 执行冲突分析模块
        conflict_analysis_handler(CONFLICT_ANALYSIS_COMPLETE_EVENT, &solver.conflict_analysis_state, formula);

        // 执行回溯模块
        backtrack_handler(BACKTRACK_COMPLETE_EVENT, &solver.decision_state);

        // 获取冲突信息
        int conflict_variable = get_conflict_variable(&solver.conflict_analysis_state);

        // 如果某个变量被频繁选择导致冲突，降低其权重
        if (conflict_variable != -1) {
            adjust_weight_for_variable(&weight_info, conflict_variable);
        }

        // 其他自适应调整逻辑...
    }

    // 输出最终解决方案或结果
    // 如果是一般 CNF 问题，打印变量值
    if (!is_sudoku_problem(formula) && !is_eight_queens_problem(formula)) {
        printf("Final Solution:\n");
        for (int i = 1; i <= formula->num_variables; ++i) {
            printf("Variable %d: %s\n", i, formula->variables[i - 1].value ? "True" : "False");
        }
    }
    // 如果是数独问题，打印数独格局
    else if (is_sudoku_problem(formula)) {
        printf("Final Solution for Sudoku:\n");
        print_sudoku_grid(formula);
    }
    // 如果是八皇后问题，打印八皇后的解法
    else if (is_eight_queens_problem(formula)) {
        printf("Final Solution for Eight Queens:\n");
        print_eight_queens_solution(formula);
    }

    // 释放 Solver 模块的资源
    release_solver_resources(&solver);
}

// 主函数，读取 CNF 文件并调用 solve 函数
int main() {
    // 使用动态内存分配
Formula* formula = malloc(sizeof(Formula));
if (formula == NULL) {
    // 处理内存分配失败的情况
    exit(EXIT_FAILURE);
}

    

    printf("Select a mode:\n");
    printf("1. General CNF Problem\n");
    printf("2. Sudoku Problem\n");
    printf("3. Eight Queens Problem\n");
    printf("Enter your choice: ");

    int choice;
do {
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > 3) {
        printf("Invalid choice. Please enter a valid option (1, 2, or 3).\n");
    }
} while (choice < 1 || choice > 3);//用户将被迫一直输入，直到提供有效的选项，防止乱输

    scanf("%d", &choice);

    switch (choice) {
        case 1:
            // 一般 CNF 问题
            solve(formula);
            break;
        case 2:
            // 数独问题
            sudoku_transform_cnf(formula);
            solve(formula);
            break;
        case 3:
            // 八皇后问题
            eightqueens_transform_cnf(formula);
            solve(formula);
            break;
        default:
            printf("Invalid choice. Exiting.\n");
            return 1;
    }

    return 0;
}
