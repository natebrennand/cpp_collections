
CC=g++
CFLAGS=-std=c++11 -Wall



collections.o: collections.h
	$(CC) $(CFLAGS) -o collections.o -c collections.h


lib: collections.o


test: lib


default: test



