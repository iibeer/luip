TARGET = libsample_plugin.so

SRC = $(wildcard *.c )
OBJ = $(patsubst %c,%o,$(SRC))

FLAGE = -g -Wall -fPIC $(INC)
SHARE = -shared -o

all:$(OBJ)
	@echo "start complie $(TARGET)..."
	$(CC) $(FLAGE) $(SHARE) $(TARGET) $^
	mv -f $(TARGET) ../output

.c.o:
	$(CC) $(FLAGE) -c $<

clean:
	rm -f *.o
	rm -f ../output/$(TARGET)