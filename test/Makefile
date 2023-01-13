TARGET = test

SRC = $(wildcard *.c)
OBJ = $(patsubst %c,%o,$(SRC))

NET_OBJ = $(wildcard $(TOPDIR)/src/*.o)

FLAGE = -g -Wall $(INC)

all:$(OBJ)
	@echo "start complie $(TARGET)..."
	$(CC) $(FLAGE) $(LIB) -o $(TARGET) $^ $(NET_OBJ)
	mv $(TARGET) output

.c.o:
	$(CC) $(FLAGE) -c $<

clean:
	rm -f *.o
	rm -f output/$(TARGET)