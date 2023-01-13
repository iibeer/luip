SRC = $(wildcard *.c)
OBJ = $(patsubst %c,%o,$(SRC))

FLAGE = -g -Wall $(INC)

all:$(OBJ)

.c.o:
	$(CC) $(FLAGE) -c $<

clean:
	rm -f *.o
