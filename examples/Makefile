IDIR =../include
CC=g++
CFLAGS=-I$(IDIR)

all: lambda

dir:
	mkdir -p build

lambda: dir
	$(CC) $(CFLAGS) lambda_fsm.cpp -o build/lambda_fsm

clean:
	rm -rf build

.PHONY: dir lambda all clean 