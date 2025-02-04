# 编译器
CC = gcc

# 编译选项
CFLAGS = -Wall -g -pthread

LDFLAGS += -pthread

# 目标文件
OBJ = mysm.o primer.o

# 可执行文件
TARGET = program

# 默认目标
all: $(TARGET)

# 生成目标文件
$(OBJ): %.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

# 生成可执行文件
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

# 清理目标
clean:
	rm -f $(OBJ) $(TARGET)

# 伪目标
.PHONY: all clean

