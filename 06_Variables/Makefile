# 编译器与选项
CC = gcc
CFLAGS = -Wall -g -I./src/frontend -I./src/midend -I./src/backend -I./src

# 目录定义
BUILD_DIR = build
SRC_DIR = src

# 最终可执行文件路径
TARGET = $(BUILD_DIR)/compiler

# 源文件列表
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/frontend/scan.c \
       $(SRC_DIR)/midend/ast.c \
	   $(SRC_DIR)/midend/misc.c \
       $(SRC_DIR)/midend/statements.c \
       $(SRC_DIR)/midend/expr.c \
	   $(SRC_DIR)/midend/symbol.c \
       $(SRC_DIR)/backend/cgen.c \
       $(SRC_DIR)/backend/gen.c

# 将 src/xxx.c 映射为 build/xxx.o
# 例如 src/frontend/scan.c -> build/frontend/scan.o
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# 默认目标
all: $(TARGET)

# 链接阶段：依赖所有的 .o 文件
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJS) -o $(TARGET)

# 编译阶段：将 .c 编译到 build/ 对应的子目录下
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# 测试目标
test: $(TARGET)
	@echo "--- 正在运行测试: tests/input01 ---"
	./$(TARGET) tests/input01 $(BUILD_DIR)/out.s
	@echo "--- 正在将生成的汇编编译为可执行文件 ---"
	$(CC) $(BUILD_DIR)/out.s -o $(BUILD_DIR)/out
	@echo "--- 运行结果如下 ---"
	./$(BUILD_DIR)/out
	@echo "--- 测试完成 ---"

# 清理：直接干掉整个 build 目录和生成的汇编
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean test