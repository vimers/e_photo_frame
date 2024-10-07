# 编译器设置
CC := gcc
CXX := g++
CFLAGS := -Wall -Wextra -O0 -g -DRPI -DUSE_LGPIO_LIB
CXXFLAGS := $(CFLAGS)
LDFLAGS := -llgpio -lm -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui

# 目标可执行文件名
TARGET := e_photo_frame

# 源文件目录
SRC_DIR := src

# 使用 wildcard 函数查找所有的 .c 和 .cpp 文件
SRCS := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c) \
        $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)

# 头文件目录
INC_DIR := include /usr/include/opencv4
# 构建目录
BUILD_DIR := build

# 生成对应的 .o 文件列表，放在 build 目录中
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SRCS))) \
        $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(filter %.cpp,$(SRCS)))

# 头文件搜索路径
INC_FLAGS := $(addprefix -I,$(INC_DIR))

# 默认目标
all: $(TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# 编译 .c 文件为 .o 文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# 编译 .cpp 文件为 .o 文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@

# 清理编译产物
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# 伪目标
.PHONY: all clean
