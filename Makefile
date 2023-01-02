CC = gcc
LD = gcc
#SRCS = $(wildcard *.c)
#OBJS = $(patsubst %c, %o, $(SRCS))
test_srcs = plugin.c netdev.c test_add.c test_main.c
test_objs = $(patsubst %c, %o, $(test_srcs))

pl_srcs = plugin.c test_plugin.c

# -I指定头文件目录
INCLUDE = -I./include
# -L指定库文件目录，-l指定静态库名字(去掉文件名中的lib前缀和.a后缀)
LIB = -L./libs -ldl
# 开启编译warning和设置优化等级
CFLAGS = -Wall -O2 -g

%.o:%.c
	$(CC) -c $^ $(INCLUDE) $(CFLAGS) 

.PHONY:all clean
plugin = plugin.so
test: $(test_objs)
	$(LD) -o $@ $^ $(LIB)
$(plugin): $(pl_srcs)
	$(LD) -o $@ $(INCLUDE) $(CFLAGS) -fPIC -shared $^ $(LIB)
all: test $(plugin)