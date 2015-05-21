CC=g++
CFLAGS=  -Wall -pedantic -ansi -pthread -lgmpxx -lgmp -g -std=c++11
RM=rm -f

ep: mathFunctions.o cosseno.o 
	$(CC) $(CFLAGS) $^ -o $@

cosseno.o: cosseno.cpp
	$(CC) $(CFLAGS) -c $^

mathFunctions.o: mathFunctions.hpp mathFunctions.cpp
	$(CC) $(CFLAGS) -c $^

clean:
	$(RM) *.o *.gch *~
