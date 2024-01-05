#include <stdio.h>
#include <stdlib.h>
#include "sat_parser.h"

//初始化模块状态
void initialize_input_parsing_module(InputParserState* input_parser_state) 
{
    // 初始化变量
    input_parser_state->variable.id = 0; // 适当的初始值
    input_parser_state->variable.value = 0; // 适当的初始值

    // 初始化子句
    input_parser_state->clause.literals = NULL; // 初始化为 NULL，具体根据需要分配内存
    input_parser_state->clause.size = 0;
    input_parser_state->clause.watch1 = 0; // 适当的初始值
    input_parser_state->clause.watch2 = 0; // 适当的初始值
    input_parser_state->clause.satisfied = 0; // 适当的初始值

    // 初始化公式
    input_parser_state->formula.variables = NULL; // 初始化为 NULL，具体根据需要分配内存
    input_parser_state->formula.num_variables = 0;
    input_parser_state->formula.clauses = NULL; // 初始化为 NULL，具体根据需要分配内存
    input_parser_state->formula.num_clauses = 0;
    input_parser_state->formula.is_conflict = 0; // 适当的初始值
}


ParseResult parse_DIMACS_CNF(const char* file_path, Formula* cnf) {
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file\n");
        return FILE_OPEN_ERROR;
    }

    // Initialize variables
    cnf->num_variables = 0;
    cnf->num_clauses = 0;
    cnf->clauses = NULL;

    char line[256];
    char *ptr = line;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == 'c' || line[0] == '\n') {
            continue; // Ignore comments and empty lines
        } else if (line[0] == 'p') {
            if (sscanf(line, "p cnf %d %d", &(cnf->num_variables), &(cnf->num_clauses)) != 2) {
                fprintf(stderr, "Error: Invalid format in 'p cnf' line\n");
                fclose(file);
                return INVALID_FORMAT_ERROR;
            }

            cnf->clauses = (Clause*)malloc(cnf->num_clauses * sizeof(Clause));//使用malloc函数动态分配了一块内存，其大小为cnf->num_clauses * sizeof(Clause) 字节。sizeof(Clause) 表示结构体 Clause 的大小，而 cnf->num_clauses 是要分配的结构体数组的元素数量。

malloc; //返回一个指向新分配内存起始位置的指针，并将该指针赋值给 cnf->clauses。这样，cnf->clauses 就指向了一个能够存储 cnf->num_clauses 个 Clause 结构体的连续内存块。
            if (cnf->clauses == NULL) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                fclose(file);
                return MEMORY_ALLOCATION_ERROR;
            }
        } else {
            int var;
            int lit_count = 0;
            Clause clause;

            while (sscanf(line, "%d", &var) > 0) {
                if (var == 0) {
                    break;
                }

                clause.literals = (int*)realloc(clause.literals, (lit_count + 1) * sizeof(int));
                if (clause.literals == NULL) {
                    fprintf(stderr, "Error: Memory allocation failed\n");
                    fclose(file);
                    return MEMORY_ALLOCATION_ERROR;
                }
    clause.literals[lit_count++] = var;

                // 移动到下一个字面量
while (*ptr != ' ' && *ptr != '\0') {
    ptr++; //增加ptr指针
}
if (*ptr == ' ') {
    ptr++; //增加ptr指针
}

            }

            // 在CNFInstance中存储子句
            cnf->clauses[cnf->num_clauses - 1].literals = clause.literals;
        }
    }

    fclose(file);
    return PARSE_SUCCESS;
}            