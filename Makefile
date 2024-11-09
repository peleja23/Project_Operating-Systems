CC = gcc
CFLAGS = -Wall -g

sort: my_sort.o sort.o
stats: my_stats.o stats.o

sort.o: sort.c my_sort.h
stats.o: stats.c my_stats.h

my_sort.o: my_sort.c my_sort.h
my_stats.o: my_stats.c my_stats.h

clean:
	rm -f *.o sort stats
