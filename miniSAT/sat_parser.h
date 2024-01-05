// sat_parser.h

#ifndef SAT_PARSER_H
#define SAT_PARSER_H

// 定义变量结构
typedef struct {
    int id;
    int value; // 0表示未赋值，1表示true，-1表示false
    int score;//变量得分
} Variable;

// 定义子句结构
typedef struct {
    int* literals; // 存储子句中的文字
    int size;       // 记录子句中包含的文字数量
    int watch1;
    int watch2;
    int satisfied;
} Clause;

// 定义公式结构
typedef struct {
    Variable* variables; // 存储变量
    int num_variables;
    Clause* clauses; // 存储子句
    int num_clauses;
    int is_conflict;
} Formula;

// 定义模块结构
typedef struct {
    Variable variable;
    Clause clause;
    Formula formula;
} InputParserState;

typedef enum {
    PARSE_SUCCESS,
    FILE_OPEN_ERROR,
    INVALID_FORMAT_ERROR,
    MEMORY_ALLOCATION_ERROR
} ParseResult;



void initialize_input_parsing_module(InputParserState* input_parser_state);

ParseResult parse_DIMACS_CNF(const char* file_path, Formula* cnf);

#endif // SAT_PARSER_H



