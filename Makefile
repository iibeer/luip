export CC = gcc
export TOPDIR = $(shell pwd)
export LIB = -ldl
export INC = -I$(TOPDIR)/include

all:
	@echo "this is all project."
	@echo "CC=$(CC)"
	@echo "LIB=$(LIB)"
	@$(MAKE) -C ./plugin
	@$(MAKE) -C ./src
	@$(MAKE) -C ./test 

clean:
	$(MAKE) -C ./plugin clean
	$(MAKE) -C ./src clean
	$(MAKE) -C ./test clean
