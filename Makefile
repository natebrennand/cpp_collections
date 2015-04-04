
CC=g++
CFLAGS=-std=c++11 -Wall


lib: collections.h

test: lib
	@./run_tests.sh

default: test

clean:
	rm -f *.o a.out


