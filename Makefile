CC = gcc
LD = gcc
SRCS = $(wildcard *.c)
OBJS = $(patsubst %c, %o, $(SRCS))
# -I指定头文件目录
INCLUDE = -I./include
# -L指定库文件目录，-l指定静态库名字(去掉文件名中的lib前缀和.a后缀)
LIB = -L./libs -ldl
# 开启编译warning和设置优化等级
CFLAGS = -Wall -O2

%.o:%.c
	$(CC) -c $^ $(INCLUDE) $(CFLAGS) 

.PHONY:all clean
test: $(OBJS)
	$(LD) -o $@ $^ $(LIB)
all: test
	make -C $^
clean: $(dir)
	make -C $^ clean