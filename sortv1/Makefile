CC = gcc
CFLAGS = -Wall -g

sort: my_sort.o sort.o

sort.o: sort.c my_sort.h

my_sort.o: my_sort.c my_sort.h

clean:
	rm -f *.o sort
