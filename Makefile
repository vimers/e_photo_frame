# 编译器设置
CC := gcc
CFLAGS := -Wall -Wextra -O0 -g -DRPI -DUSE_LGPIO_LIB
LDFLAGS := -llgpio -lm

# 目标可执行文件名
TARGET := e_photo_frame

# 源文件目录
SRC_DIR := src

# 使用 wildcard 函数查找所有的 .c 文件
SRCS := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c)

# 头文件目录
INC_DIR := include
# 构建目录
BUILD_DIR := build

# 生成对应的 .o 文件列表，放在 build 目录中
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# 头文件搜索路径
INC_FLAGS := $(addprefix -I,$(INC_DIR))

# 默认目标
all: $(TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

# 编译 .c 文件为 .o 文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# 清理编译产物
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# 伪目标
.PHONY: all clean
