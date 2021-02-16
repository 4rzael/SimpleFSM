IDIR =../include
CC=g++
CFLAGS=-I$(IDIR) -std=c++17

all: switch_lambda elevator_lambda

dir:
	mkdir -p build

switch_lambda: dir
	$(CC) $(CFLAGS) switch_lambda.cpp -o build/switch_lambda

elevator_lambda: dir
	$(CC) $(CFLAGS) elevator_lambda.cpp -o build/elevator_lambda

clean:
	rm -rf build

.PHONY: dir switch_lambda elevator_lambda all clean
