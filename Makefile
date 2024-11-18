CC = gcc
CFLAGS = -Wall -g

all: sort stats

sort: my_sort.o sort.o
stats: my_stats.o stats.o

sort.o: sort.c
stats.o: stats.c

my_sort.o: my_sort.c my_sort.h def_struct.h
my_stats.o: my_stats.c my_stats.h def_struct.h

clean:
	rm -f *.o sort stats
