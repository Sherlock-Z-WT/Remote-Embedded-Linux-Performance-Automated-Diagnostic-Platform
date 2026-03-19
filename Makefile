# 编译器设置
CC=gcc
CFLAGS=-Iinclude -Wall -Wextra -std=c99
LDFLAGS=

# 源文件和目标文件
SRC_DIR=src
INCLUDE_DIR=include
BUILD_DIR=build
SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))
TARGET=test_framework

# 默认目标
all: $(TARGET)

# 创建build目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 编译目标文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 链接可执行文件
$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

# 运行测试框架
run:
	./$(TARGET)

# 运行测试框架（带debug模式）
run-debug:
	./$(TARGET) --debug

# 运行测试框架（带quiet模式）
run-quiet:
	./$(TARGET) --quiet

# 清理生成文件
clean:
	rm -f $(TARGET)
	rm -rf $(BUILD_DIR)

# 清理所有生成文件，包括临时文件
distclean: clean
	rm -f log.txt testfile.tmp
	rm -rf report/

# 显示帮助信息
help:
	@echo "Makefile targets:"
	@echo "  make          - 编译测试框架"
	@echo "  make run      - 运行测试框架"
	@echo "  make run-debug - 运行测试框架（带debug模式）"
	@echo "  make run-quiet - 运行测试框架（带quiet模式）"
	@echo "  make clean    - 清理生成文件"
	@echo "  make distclean - 清理所有生成文件"
	@echo "  make help     - 显示帮助信息"

# 依赖关系
$(BUILD_DIR)/main.o: $(INCLUDE_DIR)/logger.h $(INCLUDE_DIR)/device.h $(INCLUDE_DIR)/test_runner.h $(INCLUDE_DIR)/config.h
$(BUILD_DIR)/logger.o: $(INCLUDE_DIR)/logger.h
$(BUILD_DIR)/device.o: $(INCLUDE_DIR)/device.h
$(BUILD_DIR)/config.o: $(INCLUDE_DIR)/config.h
$(BUILD_DIR)/test_runner.o: $(INCLUDE_DIR)/test_runner.h $(INCLUDE_DIR)/logger.h $(INCLUDE_DIR)/report.h $(INCLUDE_DIR)/tests.h $(INCLUDE_DIR)/test_registry.h $(INCLUDE_DIR)/metrics.h
$(BUILD_DIR)/report.o: $(INCLUDE_DIR)/report.h
$(BUILD_DIR)/cpu_test.o: $(INCLUDE_DIR)/tests.h $(INCLUDE_DIR)/logger.h
$(BUILD_DIR)/mem_test.o: $(INCLUDE_DIR)/tests.h $(INCLUDE_DIR)/logger.h
$(BUILD_DIR)/disk_test.o: $(INCLUDE_DIR)/tests.h $(INCLUDE_DIR)/logger.h