CC=g++
CFLAGS=  -Wall -pedantic -ansi -pthread -lgmpxx -lgmp -g -std=c++11
RM=rm -f

ep: cosseno.o
	$(CC) $(CFLAGS) $^ -o $@

cosseno.o: cosseno.cpp
	$(CC) $(CFLAGS) -c $^

clean:
	$(RM) *.o *.gch
