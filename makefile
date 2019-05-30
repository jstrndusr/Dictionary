CC=g++
CFLAGS= -std=c++11 -Wall
OBJ= test.o

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o test

test.o: test.cpp dictionary.cpp
	$(CC) $(CFLAGS) -c test.cpp

clean:
	rm -rf *.o
