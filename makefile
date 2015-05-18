CC=gcc
CFLAGS=-Wall -pedantic -ansi -pthread -g
RM=rm -f

ep: cosseno.o
	$(CC) $(CFLAGS) $^ -o $@

cosseno.o: cosseno.c
	$(CC) $(CFLAGS) -c $^

clean:
	$(RM) *.o *.gch