
# Makefile

# 编译器
CC := gcc
# 编译选项
CFLAGS := -c -w -I. -g
# 静态库工具
AR := ar
# 目标文件
TARGET := solve

# 源文件目录
SRC_DIR := .
# 目标文件目录
OBJ_DIR := obj

# 列出所有源文件
SRCS := sat_parser.c learning_module.c propagation_module.c decision_module.c conflict_analysis_module.c backtracking_module.c solver.c observer.c sat_data_structure.c sat_parser.c sudoku_transform_cnf.c eightqueens_transform_cnf.c
# 生成对应的目标文件列表
#OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o)) $(OBJ_DIR)/sudoku_transform_cnf.o

# 自动生成依赖关系文件列表
DEPS := $(OBJS:.o=.d)

# 自动生成头文件依赖关系
DEPFLAGS = -MMD -MP

# 包含所有模块的目标文件
MODULE_OBJS := $(addprefix $(OBJ_DIR)/, sat_parser.o learning_module.o propagation_module.o decision_module.o conflict_analysis_module.o backtracking_module.o)
# 静态库文件
STATIC_LIB := total.a

.PHONY: all clean

all: $(TARGET)

# 链接生成可执行文件，同时链接静态库
$(TARGET): $(OBJS) $(STATIC_LIB)
	$(CC) $^ -o $@

# 编译每个源文件生成对应的目标文件，并生成依赖关系文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) $< -o $@

# 生成目标文件目录
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# 创建静态库
$(STATIC_LIB): $(MODULE_OBJS)
	$(AR) -r $@ $^

# 包含依赖关系文件
-include $(DEPS)

# 清理目标文件、可执行文件和依赖关系文件
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(STATIC_LIB)
